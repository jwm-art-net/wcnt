#ifndef SWITCHER_H
#define SWITCHER_H

#include "wcntsignal.h"
#include "linkedlist.h"

class switcher: public synthmod
{
public:
    switcher(char const*);
    ~switcher();
    wcnt_signal* add_signal(wcnt_signal* s) {
        return wcntsig = (wcnt_signal*)
         (wcntsig_item = wcntsiglist->add_at_tail(s))->get_data();
    }
    wcnt_signal* goto_first() {
        return wcntsig = (wcnt_signal*)
         (wcntsig_item = wcntsiglist->goto_first())->get_data();
    }
    wcnt_signal* goto_last() {
        return wcntsig = (wcnt_signal*)
         (wcntsig_item = wcntsiglist->goto_last())->get_data();
    }
    wcnt_signal* goto_prev() {
        return wcntsig = (wcnt_signal*)
         (wcntsig_item = wcntsiglist->goto_prev())->get_data();
    }
    wcnt_signal* goto_next() {
        return wcntsig = (wcnt_signal*)
         (wcntsig_item = wcntsiglist->goto_next())->get_data();
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
    linkedlist* wcntsiglist;
    ll_item* wcntsig_item;
    wcnt_signal* wcntsig;
    double const* sig;
    double const* prevsig;
    double zero;// for initial stuff
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
