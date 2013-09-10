#ifndef SWITCHER_H
#include "../include/switcher.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodule.h"
#include "../include/synthmodulelist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/conversions.h"
#include "../include/dobjdobjlist.h"

#include <iostream>

switcher::switcher(char const* uname) :
 synthmod(synthmodnames::SWITCHER, uname),
 in_trig(0), xfadetime(25), out_output(0), xfade_samp(0),
 xfade_max_samps(0), xfade_stpsz(0), xfade_size(0), wcntsiglist(0),
 wcntsig_item(0), wcntsig(0), sig(0), prevsig(0), zero(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_TRIG);
    wcntsiglist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    create_params();
    create_moddobj();
}

switcher::~switcher()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
/*
    wcntsig is a synthmodule which would have been created
    before this module.  don't need to delete here.
*/
    delete wcntsiglist;
}

void const* switcher::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* switcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* switcher::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool switcher::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::XFADE_TIME: xfadetime = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* switcher::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::XFADE_TIME: return &xfadetime;
        default: return 0;
    }
}

synthmod* switcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    switcher* dup = new switcher(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);

    const char* const current_grp = get_groupname(get_username());
    const char* const new_grp = get_groupname(uname);
    bool regroup_wcnt_sigs = false;
    if (current_grp && new_grp) {
        if (strcmp(current_grp, new_grp) != 0) {
            regroup_wcnt_sigs = true;
        }
    }
    synthmodlist& modlist = jwm.get_modlist();
    if (jwm.is_verbose())
        std::cout << "\n----------\nadding to duplicated switcher "
            << uname;
    goto_first();
    while (wcntsig) {
        const char* const sig_grp =
            get_groupname(wcntsig->get_username());
        synthmod* sig_to_add = wcntsig;
        if (sig_grp && regroup_wcnt_sigs == true) {
            if (strcmp(sig_grp, current_grp) == 0) {
                const char* const grpsigname =
                        set_groupname(new_grp, wcntsig->get_username());
                synthmod* grpsig =
                            modlist.get_synthmod_by_name(grpsigname);
                if (grpsig) {
                    if (grpsig->get_module_type() ==
                                            synthmodnames::WCNTSIGNAL)
                        sig_to_add = grpsig;
                    else {
                        std::cout << "\nin switcher::duplicate, an "
                            "attempt to fetch a wcnt_signal named "
                            << grpsigname << "resulted in finding "
                            << grpsig->get_username()
                            << " which is not a wcnt_signal!?!?";
                    }
                }
                else if (jwm.is_verbose()) {
                    std::cout << "\nWarning! switcher " << uname
                        << " was expecting to find " << grpsigname
                        << " but could not.\nCheck the order of grouping"
                        << " in original group definition.";
                }
                delete [] grpsigname;
            }
            delete [] sig_grp;
        }
        dup->add_signal((wcnt_signal*)sig_to_add);
        if (jwm.is_verbose())
            std::cout << "\nadded " << sig_to_add->get_username();
        goto_next();
    }
    delete [] current_grp;
    delete [] new_grp;
    if (jwm.is_verbose())
        std::cout << "\n----------";
    return dup;
}

stockerrs::ERR_TYPE switcher::validate()
{
    if (!goto_first()) {
        *err_msg = "must be at least two signals to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg = "must be at least two signals to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist().validate(this, paramnames::XFADE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::XFADE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* switcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::WCNTSIGNAL) {
            *err_msg = "\n";
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_signal";
            return 0;
        }
        if (!add_signal((wcnt_signal*)sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into switcher";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist().add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

void switcher::init()
{
    goto_first();
    sig = wcntsig->get_output();
    xfade_samp = xfade_max_samps = ms_to_samples(xfadetime);
    xfade_stpsz = 1 / (double)xfade_samp;
    xfade_size = 0;
    prevsig = &zero;
}

void switcher::run()
{
    if (*in_trig == ON) {
        prevsig = sig;
        if (!(wcntsig = goto_next()))
            wcntsig = goto_first();
        sig = wcntsig->get_output();
        xfade_samp = xfade_max_samps;
        xfade_size = 0;
    }
    if (xfade_samp > 0) {
        out_output = *prevsig * (1 - xfade_size) + *sig * xfade_size;
        xfade_samp--;
        xfade_size += xfade_stpsz;
    } else
        out_output = *sig;
}

bool switcher::done_params = false;

void switcher::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
     synthmodnames::SWITCHER, paramnames::XFADE_TIME);
    done_params = true;
}

bool switcher::done_moddobj = false;

void switcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::SWITCHER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
