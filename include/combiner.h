#ifndef COMBINER_H
#define COMBINER_H

#include "wcntsignal.h"
#include "linkedlist.h"

class combiner: public synthmod
{
public:
    combiner(char const*);
    ~combiner();
    wcnt_signal* add_signal(wcnt_signal* s) {
        sigcount++;
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
    // wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class
    // - dobjmod, is passed which contains a pointer to the wcnt_signal
    dobj* add_dobj(dobj*);
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);

private:
    double out_output;
    double total;
    int sigcount;
    STATUS meantotal;
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
