#ifndef TRIGSWITCHER_H
#define TRIGSWITCHER_H

#include "wcnttrigger.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"
#include "dobjlist.h"

class trigswitcher: public synthmod
{
public:
    trigswitcher(char const*);
    ~trigswitcher();
    wcnt_trigger* add_trigger(wcnt_trigger* s) {
        return wcnttrig = (wcnt_trigger*)
         (wcnttrig_item = wcnttriglist->add_at_tail(s))->get_data();
    }
    wcnt_trigger* goto_first() {
        return wcnttrig = (wcnt_trigger*)
         (wcnttrig_item = wcnttriglist->goto_first())->get_data();
    }
    wcnt_trigger* goto_last() {
        return wcnttrig = (wcnt_trigger*)
         (wcnttrig_item = wcnttriglist->goto_last())->get_data();
    }
    wcnt_trigger* goto_prev() {
        return wcnttrig = (wcnt_trigger*)
         (wcnttrig_item = wcnttriglist->goto_prev())->get_data();
    }
    wcnt_trigger* goto_next() {
        return wcnttrig = (wcnt_trigger*)
         (wcnttrig_item = wcnttriglist->goto_next())->get_data();
    }
    void set_in_trig(STATUS const* it){ in_trig = it; }
    STATUS const* get_in_trig(){ return in_trig;}
    STATUS const* get_out_trig(){ return &out_trig;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    dobj* add_dobj(dobj*);

private:
    STATUS const* in_trig;
    STATUS out_trig;
    linkedlist* wcnttriglist;
    ll_item* wcnttrig_item;
    wcnt_trigger* wcnttrig;
    STATUS const* trig;
    static int trigswitcher_count;
    static bool done_moddobj;
    void create_moddobj();
};

#endif
