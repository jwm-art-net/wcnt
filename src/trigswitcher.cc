#ifndef TRIGSWITCHER_H
#include "../include/trigswitcher.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/dobjdobjlist.h"
#include "../include/duplicate_list_module.h"

trigswitcher::trigswitcher(char const* uname) :
 synthmod(synthmodnames::TRIGSWITCHER, uname),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_trig(0), out_trig(OFF),
 wcnttrigs(0), trig_ix(0), trig(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    create_moddobj();
}

trigswitcher::~trigswitcher()
{
    if (wcnttrigs)
        delete [] wcnttrigs;
}

void const* trigswitcher::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG: return &out_trig;
        default: return 0;
    }
}

void const* trigswitcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* trigswitcher::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

synthmod* trigswitcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE trigswitcher::validate()
{
    if (!goto_first()) {
        *err_msg = "must be at least two triggers to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg = "must be at least two triggers to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* trigswitcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::WCNTTRIGGER) {
            *err_msg = "\n";
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_trigger";
            return 0;
        }
        if (!add_trigger((wcnt_trigger*)sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into trigswitcher";
            return 0;
        }
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

void trigswitcher::init()
{
    if (!(wcnttrigs = move_to_array(this))){
        invalidate();
        return;
    }
    trig = wcnttrigs[trig_ix = 0]->get_out_trig();
}

void trigswitcher::run()
{
    if (*in_trig == ON) {
        trig_ix++;
        if (trig_ix == get_count())
            trig_ix = 0;
        trig = wcnttrigs[trig_ix]->get_out_trig();
    }
    out_trig = *trig;
}

bool trigswitcher::done_moddobj = false;

void trigswitcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::TRIGSWITCHER, dobjnames::LST_TRIGGERS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRIGGERS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
