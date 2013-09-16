#ifndef SPREADER_H
#define SPREADER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class spreader: public synthmod, public linked_list<synthmod>
{
public:
    spreader(char const*);
    ~spreader();

    friend synthmod*
        duplicate_list_module<spreader, synthmod>
            (spreader* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
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
    
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
