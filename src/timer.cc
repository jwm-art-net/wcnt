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
 out_count(0), out_trig(OFF), out_index(-1), time_is_relative(OFF),
 recycle_mode(OFF), timings(0), reset_ix(0), next_ix(0),
 curtime(0.0f), samples(0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_COUNT);
    register_output(output::OUT_INDEX);
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
#include <iostream>

void timer::init()
{
    if (!time_is_relative)
        sort_list(this, &timing::get_time);

    if (!(timings = move_to_array(this))){
        invalidate();
        return;
    }

    next_ix = 1; // potentially next timing

    if (!timings[0]) {
        // give up, there are no timings.
        samples = SAMP_T_MAX;
        next_ix = -1;
    }
    else if (timings[0]->get_time() == 0.0f) {
        // first starts right out the gate at zero seconds.
        samples = 0;
        reset_ix = 1; // skip the zero timing when recycling
    }
    else {
        // first starts some point in the future...
        next_ix = 1;
        timing* t = timings[0];
        curtime = t->get_time();
        samples = curtime * jwm.samplerate();
    }

    out_count = -1;
    out_index = -1;
}

void timer::run()
{
    if (samples == 0) {
        if (next_ix == -1)
            return;
        // we have reached the time...
        out_count++;
        out_index++;
        out_trig = ON;
        timing* next = timings[next_ix];
        if (!next && recycle_mode == ON)
            next = timings[out_index = next_ix = reset_ix];
        if (next) {
            if (time_is_relative)
                samples = next->get_time() * jwm.samplerate();
            else {
                double nexttime = next->get_time();
                samples = (nexttime - curtime) * jwm.samplerate();
                curtime = nexttime;
            }
            ++next_ix;
        }
        else {
            samples = SAMP_T_MAX;
            next_ix = -1;
        }
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        samples--;
    }
}


const void* timer::get_out(output::TYPE ot) const
{
    switch (ot) {
    case output::OUT_TRIG: return &out_trig;
    case output::OUT_COUNT:return &out_count;
    case output::OUT_INDEX:return &out_index;
    default:
        return 0;
    }
}

bool timer::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
    case param::TIME_IS_RELATIVE: time_is_relative = *(STATUS*)data; break;
    case param::RECYCLE_MODE:     recycle_mode = *(STATUS*)data; break;
    default:
        return false;
    }
    return true;
}

const void* timer::get_param(param::TYPE pt) const
{
    switch(pt) {
    case param::TIME_IS_RELATIVE: return &time_is_relative;
    case param::RECYCLE_MODE:     return &recycle_mode;
    default:
        return 0;
    }
}

errors::TYPE timer::validate()
{
    if (time_is_relative) {
        llitem* tmp = sneak_first()->get_next();
        while (tmp) {
            llitem* rm = 0;
            timing* t = tmp->get_data();
            if (t->get_time() == 0.0f) {
                rm = unlink_item(tmp);
                if (jwm.is_verbose())
                    std::cout << "In module " << get_username()
                              << " pruning extraneous timing of 0.0 "
                                            " seconds." << std::endl;
            }
            tmp = tmp->get_next();
            if (rm) {
                delete t;
                delete rm;
            }
        }
    }
    else {
        llitem* chk = sneak_first();
        while(chk) {
            double s1 = chk->get_data()->get_time();
            llitem* tmp = chk->get_next();
            while(tmp) {
                llitem* rm = 0;
                timing* t2 = tmp->get_data();
                double s2 = t2->get_time();
                if (s1 == s2) {
                    rm = unlink_item(tmp);
                    if (jwm.is_verbose())
                        std::cout << "In module " << get_username()
                                  << " pruning extraneous timing of "
                                  << s2 << " seconds." << std::endl;
                }
                tmp = tmp->get_next();
                if (rm) {
                    delete t2;
                    delete rm;
                }
            }
            chk = chk->get_next();
        }
    }

    return errors::NO_ERROR;
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
    register_param(param::TIME_IS_RELATIVE);
    register_param(param::RECYCLE_MODE);
    register_moddobj(dataobj::LST_TIMINGS, dataobj::SIN_TIME);
}

