#ifndef TIMER_H
#define TIMER_H

#include "synthmodule.h"
#include "conversions.h"
#include "linkedlist.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "dobjdobjlist.h"
#endif

#include "timing.h"

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
    // outputs access:
    short const* get_out_count(){return &out_count;}
    STATUS const* get_out_trig(){return &out_trig;}
    // virtual methods
    void run();
    void init();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    dobj* add_dobj(dobj*);
#endif
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
    static short timer_count;
#ifndef BARE_MODULES
    static bool done_moddobj;
    void create_moddobj();
#endif
};

#endif
