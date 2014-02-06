#ifndef COMBINER_H
#define COMBINER_H

#include "synthmod.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class wcnt_signal;

class combiner: public synthmod::base,
                public linked_list<synthmod::base>
{
public:
    combiner(const char*);
    ~combiner();

    friend synthmod::base*
        duplicate_list_module<combiner, synthmod::base>
                            (combiner*, synthmod::base*,
                            const char* uname, synthmod::base::DUP_IO);
    void run();
    void init();
    errors::TYPE validate();
    // Modules are not a data objects! A dobj wrapper class (dobjmod)
    // is used which contains a pointer to a module to be added.
    dobj::base* add_dobj(dobj::base*);
    const void* get_out(int output_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    synthmod::base* duplicate_module(const char* uname, DUP_IO);

private:
    double out_output;
    STATUS meantotal;
    double total;
    double const** sigs;
    int sigcount;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
