#ifndef TIMER_H
#define TIMER_H

#include "synthmod.h"
#include "timing.h"
#include "linkedlist.h"

/*
    timer
    =====
    output trigger when specified duration(s) elapse.
    output count of how many durations have elapsed.
*/

class timer : public synthmod, public linked_list<timing>
{
 public:
    timer(const char*);
    ~timer();

    timing* add_timing(double secs);
    timing* add_timing(timing*);

    void run();
    void init();
    const void* get_out(output::TYPE) const;
    bool        set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    dobj*       add_dobj(dobj*);

 private:
    wcint_t out_count;
    STATUS out_trig;
    STATUS time_is_relative;

    timing** timings;
    long time_ix;
    samp_t samples;

    void init_first();
};

#endif
