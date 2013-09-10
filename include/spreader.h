#ifndef SPREADER_H
#define SPREADER_H

#include "wcntsignal.h"
#include "conversions.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"
#include "dobjlist.h"

class spreader: public synthmod
{
public:
    spreader(char const*);
    ~spreader();
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
    void set_input_modulation(double const* i){ in_mod = i;}
    double const* get_input_modulation(){ return in_mod;}
    double const* get_output(){ return &out_output;}
    void set_start_level(double sl) { start_level = sl;}
    void set_end_level(double el){ end_level = el;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
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
    double level_dif;
    double seg_lvl;
    short sig_count;
    linkedlist* wcntsiglist;
    ll_item*	wcntsig_item;
    wcnt_signal* wcntsig;
    double const* sig;
    double const* prevsig;
    double zero;// for initial stuff
    static int spreader_count;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
