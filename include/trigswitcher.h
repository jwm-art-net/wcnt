#ifndef TRIGSWITCHER_H
#define TRIGSWITCHER_H

#include "wcnttrigger.h"
#include "linkedlist.h"

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
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);

private:
    STATUS const* in_trig;
    STATUS out_trig;
    linkedlist* wcnttriglist;
    ll_item* wcnttrig_item;
    wcnt_trigger* wcnttrig;
    STATUS const* trig;
    static bool done_moddobj;
    void create_moddobj();
};

#endif
