#ifndef TRIGSWITCHER_H
#define TRIGSWITCHER_H

#include "wcnttrigger.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class trigswitcher: public synthmod::base,
                    public linked_list<synthmod::base>
{
 public:
    trigswitcher(const char*);
    ~trigswitcher();

    friend synthmod::base*
        duplicate_list_module<trigswitcher, synthmod::base>
                            (trigswitcher*, synthmod::base*,
                            const char* uname, synthmod::base::DUP_IO);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    synthmod::base* duplicate_module(const char*, synthmod::base::DUP_IO);
    dobj::base* add_dobj(dobj::base*);

 private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS const** trigs;
    long trig_ix;
    STATUS const* trig;
    void register_ui();
};

#endif
