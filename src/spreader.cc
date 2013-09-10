#ifndef SPREADER_H
#include "../include/spreader.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodule.h"
#include "../include/synthmodulelist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <math.h>

spreader::spreader(char const* uname) :
 synthmod(synthmodnames::SPREADER, uname),
 in_mod(0), out_output(0), start_level(0), end_level(0),
 level_dif(0), sig_count(0), wcntsiglist(0), wcntsig_item(0),
 wcntsig(0), sig(0), prevsig(0), zero(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_MODULATION);
    wcntsiglist =
     new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    create_params();
    create_moddobj();

}

spreader::~spreader()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
/* wcntsig is a synthmodule of type wcnt_signal, created before
    this module was, so don't delete it, idiot. (speaking to myself)
*/
    delete wcntsiglist;
}

void const* spreader::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* spreader::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_MODULATION: return in_mod = (double*)o;
        default: return 0;
    }
}

void const* spreader::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_MODULATION: return in_mod;
        default: return 0;
    }
}

bool spreader::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::START_LEVEL:
            start_level = *(double*)data;
            return true;
        case paramnames::END_LEVEL:
            end_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* spreader::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::START_LEVEL:   return &start_level;
        case paramnames::END_LEVEL:     return &end_level;
        default: return 0;
    }
}

synthmod* spreader::duplicate_module(const char* uname, DUP_IO dupio)
{
    spreader* dup = new spreader(uname);
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
        std::cout << "\n----------\nadding to duplicated spreader "
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
                        std::cout << "\nin spreader::duplicate, an "
                            "attempt to fetch a wcnt_signal named "
                             << grpsigname << "resulted in finding "
                             << grpsig->get_username()
                             << "which is not a wcnt_signal!?!?";
                    }
                }
                else if (jwm.is_verbose()) {
                    std::cout << "\nWarning! spreader " << uname
                        << " was expecting to find " << grpsigname
                        << " but could not. \nCheck the order of "
                        "grouping in original group definition.";
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

stockerrs::ERR_TYPE spreader::validate()
{
    if (!goto_first()) {
        *err_msg = "must be at least two signals to spread across";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg= "must be at least two signals to spread across";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* spreader::add_dobj(dobj* dbj)
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
            *err_msg += " into spreader";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist().add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to " + *get_username();
    return 0;
}

void spreader::init()
{
    goto_first();
    sig_count = 0;
    while (wcntsig) {
        goto_next();
        if (wcntsig)
            sig_count++;
    }
    level_dif = end_level - start_level;
    seg_lvl = level_dif / sig_count;
}

void spreader::run()
{
    out_output = 0;
    short sig_no = 0;
    double im_rl = sqrt(*in_mod * *in_mod);
    if (im_rl < start_level) im_rl = start_level;
    else if (im_rl > end_level) im_rl = end_level;
    goto_first();
    while (wcntsig) {
        double lvl = sig_no * seg_lvl;
        double vol = 0;
        if (im_rl >= lvl && im_rl <= lvl + seg_lvl)
            vol = (seg_lvl - (im_rl - lvl)) / seg_lvl;
        if (im_rl + seg_lvl >= lvl && im_rl + seg_lvl < lvl + seg_lvl)
            vol = (im_rl + seg_lvl - lvl) / seg_lvl;
        out_output += *(wcntsig->get_output()) * vol;
        sig_no++;
        goto_next();
    }
}

bool spreader::done_params = false;

void spreader::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
        synthmodnames::SPREADER, paramnames::START_LEVEL);
    jwm.get_paramlist().add_param(
        synthmodnames::SPREADER, paramnames::END_LEVEL);
    done_params = true;
}

bool spreader::done_moddobj = false;

void spreader::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::SPREADER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
