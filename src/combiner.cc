#ifndef COMBINER_H
#include "../include/combiner.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjmod.h"
#include "../include/wcntsignal.h"
#include "../include/duplicate_list_module.h"

combiner::combiner(char const* uname) :
 synthmod(synthmodnames::COMBINER, uname),
 linkedlist(MULTIREF_OFF, PRESERVE_DATA),
 out_output(0), meantotal(OFF),
 total(0), wcntsigs(0), wcntsig(0), sigcount(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    create_params();
    create_moddobj();
}

combiner::~combiner()
{
    if (wcntsigs) // just delete the array, as the contents will reside
        delete [] wcntsigs; // in the main (or group_control) modlist
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
    return duplicate_list_module(this, goto_first(), uname, dupio);
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
        if (!add_at_tail((wcnt_signal*)sm)) {
            *err_msg = " it is possible the module ";
            *err_msg += sm->get_username();
            *err_msg += " has already been added";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to " + *get_username();
    return 0;
}

void combiner::init()
{
    sigcount = get_count();
    wcntsigs = move_to_array(this);
}

void combiner::run()
{
    total = 0;
    long ix = 0;
    while((wcntsig = wcntsigs[ix])){
        total += wcntsig->out_output;
        ix++;
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
    jwm.get_paramlist()->
        add_param(synthmodnames::COMBINER, paramnames::MEAN_TOTAL);
    done_params = true;
}

bool combiner::done_moddobj = false;
void combiner::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::COMBINER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
