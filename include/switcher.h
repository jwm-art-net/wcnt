#ifndef SWITCHER_H
#define SWITCHER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class switcher: public synthmod, public linked_list<synthmod>
{
public:
    switcher(const char*);
    ~switcher();

    friend synthmod*
        duplicate_list_module<switcher, synthmod>
            (switcher* sm, synthmod* _data,
                const char* uname, synthmod::DUP_IO dupio);

    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);
private:
    STATUS const* in_trig;
    double xfadetime;
    double out_output;
    unsigned long xfade_samp;
    unsigned long xfade_max_samps;
    double xfade_stpsz;
    double xfade_size;
    double const** sigs;
    long sig_ix;
    double const* sig;
    double const* prevsig;
    double zero;// for initial stuff
    
    void init_first();
};

#endif
