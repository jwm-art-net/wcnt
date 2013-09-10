#ifndef TIMER_H
#define TIMER_H

#include "synthmodule.h"
#include "timing.h"
#include "linkedlist.h"

/*
    timer
    =====
    output trigger when specified duration(s) elapse.
    output count of how many durations have elapsed.
*/

class timer : public synthmod
{
public:
    timer(char const*);
    ~timer();
    // list insertion and navigation methods
    timing* add_timing(double secs);
    timing* add_timing(timing*);
    timing* goto_first() {
        return current = (timing*)
         (t_item = t_list->goto_first())->get_data();
    }
    timing* goto_last() {
        return current = (timing*)
         (t_item = t_list->goto_last())->get_data();
    }
    timing* goto_prev() {
        return current = (timing*)
         (t_item = t_list->goto_prev())->get_data();
    }
    timing* goto_next() {
        return current = (timing*)
         (t_item = t_list->goto_next())->get_data();
    }
    timing* get_next(){
        return (timing*)(t_item->get_next())->get_data();
    }
    timing* get_prev(){
        return (timing*)(t_item->get_prev())->get_data();
    }
    // virtual methods
    void run();
    void init();
    void const* get_out(outputnames::OUT_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);
private:
    // outputs
    short out_count;
    STATUS out_trig;
    // list variables
    ll_item* t_item;
    linkedlist* t_list;
    // working variables
    timing* current;
    unsigned long samples;
    // synthmod stuff for keeping things cushdy.
    static bool done_moddobj;
    void create_moddobj();
};

#endif
