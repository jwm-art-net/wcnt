#ifndef TRIGSWITCHER_H
#define TRIGSWITCHER_H

#include "wcnttrigger.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class trigswitcher: public synthmod, public linked_list<synthmod>
{
public:
    trigswitcher(const char*);
    ~trigswitcher();

    friend synthmod*
        duplicate_list_module<trigswitcher, synthmod>
            (trigswitcher* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);

private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS const** trigs;
    long trig_ix;
    STATUS const* trig;
    void init_first();
};

#endif
