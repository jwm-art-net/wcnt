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

class timer : public synthmod::base, public linked_list<timing>
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
    dobj::base* add_dobj(dobj::base*);
    errors::TYPE validate();

 private:
    wcint_t out_count;
    STATUS out_trig;
    wcint_t out_index;
    STATUS time_is_relative;
    STATUS recycle_mode;

    timing** timings;
    wcint_t reset_ix;
    wcint_t next_ix;
    double curtime;
    samp_t samples;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
