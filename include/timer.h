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
    const void* get_out(outputnames::OUT_TYPE) const;
    dobj* add_dobj(dobj*);

 private:
    short out_count;
    STATUS out_trig;

    timing** timings;
    long time_ix;
    unsigned long samples;

    void init_first();
};

#endif
