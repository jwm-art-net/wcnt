#ifndef NONEZERO_H
#define NONEZERO_H

#include "synthmod.h"

// All inputs in wcnt need to be connected, because inputs are simple pointers,
// which, despite what the user thinks when they set an input 'off', internally,
// the pointers need to point to something.
// This module provides that something!

class nonezero: public synthmod::base
{
public:
    nonezero(const char*);
    ~nonezero();
    // virtual funcs
    void run(){};
    const void* get_out(output::TYPE) const;

private:
    const double out_none_double;
    const double out_one_double;
    const wcint_t out_none_wcint_t;
    const samp_t out_none_samp_t;
    const STATUS out_none_STATUS;
    char* out_none_string;
    void register_ui(){};
    ui::moditem_list* get_ui_items() { return 0; }
};

#endif
