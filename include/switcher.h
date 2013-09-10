#ifndef SWITCHER_H
#define SWITCHER_H

#include "wcntsignal.h"
#include "linkedlist.h"
#include "duplicate_list_module.h"

class switcher: public synthmod, public linked_list<wcnt_signal>
{
public:
    switcher(char const*);
    ~switcher();

    friend synthmod*
        duplicate_list_module<switcher, wcnt_signal>
            (switcher* sm, wcnt_signal* _data,
                const char* uname, synthmod::DUP_IO dupio);

    wcnt_signal* add_signal(wcnt_signal* s) {
        return add_at_tail(s)->get_data();
    }
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
    dobj* add_dobj(dobj*);
private:
    STATUS const* in_trig;
    double xfadetime;
    double out_output;
    unsigned long xfade_samp;
    unsigned long xfade_max_samps;
    double xfade_stpsz;
    double xfade_size;
    wcnt_signal** wcntsigs;
    long sig_ix;
    double const* sig;
    double const* prevsig;
    double zero;// for initial stuff
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
