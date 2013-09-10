#ifndef TIMER_H
#include "../include/timer.h"

timer::timer(char const* uname) :
 synthmod(synthmodnames::MOD_TIMER, timer_count, uname),
 out_count(0), out_trig(OFF), t_item(0), t_list(0), current(0),
 samples(0)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this,outputnames::OUT_TRIG);
    get_outputlist()->add_output(this,outputnames::OUT_COUNT);
#endif
    t_list =
        new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    timer_count++;
#ifndef BARE_MODULES
    create_moddobj();
#endif
}

timer::~timer()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
#endif
    goto_first();
    while(current) {
        delete current;
        goto_next();
    }
    delete t_list;
}

timing* timer::add_timing(double secs)
{
    if (!(current = new timing(secs))) return 0;
    if (!t_list->add_at_tail(current)) return 0;
    return current;
}

timing* timer::add_timing(timing* t)
{
    if (!t) return 0;
    if (!t_list->add_at_tail(t)) return 0;
    return current = t;
}

void timer::init()
{
    goto_first();
    if (current)
        samples = (unsigned long)(current->get_time() * audio_samplerate);
    else
        samples = (unsigned long) -1;
    out_count = -1;
}

void timer::run()
{
    if (samples == 0) {
        out_count++;
        out_trig = ON;
        goto_next();
        if (current)
            samples
             = (unsigned long)(current->get_time() * audio_samplerate);
        else
            samples = (unsigned long) -1;
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        samples--;
    }
}

#ifndef BARE_MODULES
void const* timer::get_out(outputnames::OUT_TYPE ot)
{
    void const *o = 0;
    switch (ot) {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    case outputnames::OUT_COUNT:
        o = &out_count;
        break;
    default:
        o = 0;
    }
    return o;
}

dobj* timer::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_TIME:
        if (!(retv = add_timing((timing*)dbj)))
            *err_msg = "\ncould not add timing to "
                       + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
        retv = 0;
    }
    return retv;
}


bool timer::done_moddobj = false;
void timer::create_moddobj()
{
    if (done_moddobj == true)
        return;
    get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_TIMER, dobjnames::LIN_TIMINGS);
    dobj::get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LIN_TIMINGS, dobjnames::SIN_TIME);
    done_moddobj = true;
}

#endif

short timer::timer_count = 0;

#endif
