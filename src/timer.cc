#include "../include/timer.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

timer::timer(const char* uname) :
 synthmod(module::TIMER, uname, SM_UNDUPLICABLE | SM_UNGROUPABLE
                                                | SM_HAS_OUT_TRIG),
 out_count(0), out_trig(OFF),
 timings(0), time_ix(0),
 samples(0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_COUNT);
    init_first();
}

timer::~timer()
{
    if (timings)
        destroy_array_moved_from_list(timings);
}

timing* timer::add_timing(double secs)
{
    timing* newtime = 0;
    if (!(newtime = new timing(secs)))
        return 0;
    return add_at_tail(newtime)->get_data();
}

timing* timer::add_timing(timing* t)
{
    if (!t) return 0;
    return add_at_tail(t)->get_data();
}

void timer::init()
{
    if(!(timings = move_to_array(this))){
        invalidate();
        return;
    }
    time_ix = 0;
    timing* t = timings[time_ix];
    if (t)
        samples = (unsigned long)
            (t->get_time() * jwm.samplerate());
    else
        samples = (unsigned long) -1;
    out_count = -1;
}

void timer::run()
{
    if (samples == 0) {
        out_count++;
        out_trig = ON;
        time_ix++;
        timing* t = timings[time_ix];
        if (t)
            samples = (unsigned long)
                (t->get_time() * jwm.samplerate());
        else
            samples = (unsigned long)-1;
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        samples--;
    }
}

const void* timer::get_out(output::TYPE ot) const
{
    switch (ot)
    {
        case output::OUT_TRIG: return &out_trig;
        case output::OUT_COUNT:return &out_count;
        default: return 0;
    }
}

dobj* timer::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dataobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dataobj::SIN_TIME:
        if (!(retv = add_timing((timing*)dbj)))
            sm_err("Could not add timing to %s", get_username());
        break;
    default:
    sm_err("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
        retv = 0;
    }
    return retv;
}

void timer::init_first()
{
    if (done_first())
        return;
    register_moddobj(dataobj::LST_TIMINGS, dataobj::SIN_TIME);
}

