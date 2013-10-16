#ifndef SPREADER_H
#define SPREADER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class spreader: public synthmod::base, public linked_list<synthmod::base>
{
 public:
    spreader(const char*);
    ~spreader();

    friend synthmod::base*
        duplicate_list_module<spreader, synthmod::base>
                                (spreader*, synthmod::base*,
                                const char* uname, synthmod::base::DUP_IO);
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod::base* duplicate_module(const char*, synthmod::base::DUP_IO);
    // wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class
    // - dobjmod, is passed which contains a pointer to the wcnt_signal
    dobj::base* add_dobj(dobj::base*);
 private:
    // inputs
    double const* in_mod;
    // outputs
    double out_output;
    // params
    double start_level;
    double end_level;
    // working
    double seg_lvl;
    double const** sigs;
    void register_ui();
};

#endif
