#ifndef TRIGSWITCHER_H
#include "../include/trigswitcher.h"

trigswitcher::trigswitcher(char const* uname) :
 synthmod(synthmodnames::MOD_TRIGSWITCHER, trigswitcher_count, uname),
 in_trig(0), out_trig(OFF), wcnttriglist(0), wcnttrig_item(0),
 wcnttrig(0), trig(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    wcnttriglist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    trigswitcher_count++;
    create_moddobj();
}

trigswitcher::~trigswitcher()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    delete wcnttriglist;
}

void const* trigswitcher::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* trigswitcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
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
        if (sm->get_module_type() != synthmodnames::MOD_WCNTTRIGGER) {
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
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

void trigswitcher::init()
{
    goto_first();
    trig = wcnttrig->get_out_trig();
}

void trigswitcher::run()
{
    if (*in_trig == ON) {
        if (!(wcnttrig = goto_next()))
            wcnttrig = goto_first();
        trig = wcnttrig->get_out_trig();
    }
    out_trig = *trig;
}

int trigswitcher::trigswitcher_count = 0;

bool trigswitcher::done_moddobj = false;

void trigswitcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_TRIGSWITCHER, dobjnames::LST_TRIGGERS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_TRIGGERS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
