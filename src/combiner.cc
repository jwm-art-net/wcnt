#ifndef COMBINER_H
#include "../include/combiner.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodulelist.h"
#include "../include/dobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjmod.h"

#include <iostream>

combiner::combiner(char const* uname) :
 synthmod(synthmodnames::COMBINER, uname),
 out_output(0), sigcount(0), meantotal(ON), wcntsiglist(0),
 wcntsig_item(0), wcntsig(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    wcntsiglist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    create_params();
    create_moddobj();
}

combiner::~combiner()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
//  wcntsig is a synthmodule which would have been created
//  before this module.  don't need to delete here.
    delete wcntsiglist;
}

void const* combiner::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

bool combiner::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::MEAN_TOTAL:
            meantotal = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* combiner::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MEAN_TOTAL: return &meantotal;
        default: return 0;
    }
}

synthmod* combiner::duplicate_module(const char* uname, DUP_IO dupio)
{
    combiner* dup = new combiner(uname);
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
        std::cout << "\n----------\nadding to duplicated combiner "
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
                        std::cout << 
                            "\nin combiner::duplicate, an attempt to";
                        std::cout << " fetch a wcnt_signal named " <<
                            grpsigname;
                        std::cout << "resulted in finding ";
                        std::cout << grpsig->get_username();
                        std::cout << " which is not a wcnt_signal!?!?";
                    }
                }
                else if (jwm.is_verbose()) {
                    std::cout << "\nWarning! combiner " << uname;
                    std::cout << " was expecting to find " << grpsigname;
                    std::cout << " but could not.\nCheck the order of"
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

stockerrs::ERR_TYPE combiner::validate()
{
    if (!goto_first()) {
        *err_msg = " signals to combine";
        invalidate();
        return stockerrs::ERR_ATLEAST2;
    } else if (!goto_next()){
        *err_msg = " signals to combine";
        invalidate();
        return stockerrs::ERR_ATLEAST2;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* combiner::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::WCNTSIGNAL) {
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_signal";
            return 0;
        }
        if (!add_signal((wcnt_signal*)sm)) {
            *err_msg = "could not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into combiner";
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

void combiner::init()
{
    goto_first();
    sig = wcntsig->get_output();
}

void combiner::run()
{
    total = 0;
    goto_first();
    while(wcntsig){
        total += *(wcntsig->get_output());
        goto_next();
    }
    if (meantotal == ON)
        out_output = total / sigcount;
    else
        out_output = total;
}

bool combiner::done_params = false;
void combiner::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().
    add_param(synthmodnames::COMBINER, paramnames::MEAN_TOTAL);
    done_params = true;
}

bool combiner::done_moddobj = false;
void combiner::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::COMBINER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
