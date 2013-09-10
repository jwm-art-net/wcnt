#ifndef TRIGSWITCHER_H
#define TRIGSWITCHER_H

#include "wcnttrigger.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class trigswitcher: public synthmod, public linked_list<wcnt_trigger>
{
public:
    trigswitcher(char const*);
    ~trigswitcher();

    friend synthmod*
        duplicate_list_module<trigswitcher, wcnt_trigger>
            (trigswitcher* sm, wcnt_trigger* _data,
                const char* uname, synthmod::DUP_IO dupio);

    wcnt_trigger* add_trigger(wcnt_trigger* s) {
        return add_at_tail(s)->get_data();
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
    wcnt_trigger** wcnttrigs;
    long trig_ix;
    STATUS const* trig;
    static bool done_moddobj;
    void create_moddobj();
};

#endif
