#ifndef SPREADER_H
#define SPREADER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class spreader: public synthmod, public linked_list<synthmod>
{
public:
    spreader(const char*);
    ~spreader();

    friend synthmod*
        duplicate_list_module<spreader, synthmod>
            (spreader* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    // wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class
    // - dobjmod, is passed which contains a pointer to the wcnt_signal
    dobj* add_dobj(dobj*);
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
    
    void init_first();
};

#endif
