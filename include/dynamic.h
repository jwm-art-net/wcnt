#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "synthmodule.h"
#include "dynvertex.h"
#include "linkedlist.h"

/*  dynamic - applies amplitude dynamics to the input signal
              remaps the amplitude of input signal.

    dynamic contains linked list of dynvertex.  referenced to dobj via
    dobjnames::LIN_DYNAMIC.  

    to create the dynamic map shape, insert dynvertex into it.
    dynvertex has an sigin_level which represents a given amplitude
    of the input signal.it has also, two levels, both of which represent
    either the level to use to replace input_signal, or the amount to 
    multiply the input_signal by.  The lower level acts as an expander, 
    the upper level as a compressor.

     sil - signal_in_level
    usol - upper_signal_out_level
    lsol - lower_signal_out_level                   */

class dynamic : public synthmod
{
public:
    dynamic(char const*);
    ~dynamic();
    dynvertex* add_dvertex(dynvertex* dv);
    dynvertex* add_dvertex(double sil, double usol, double lsol);
    bool delete_dvertex(dynvertex* dv);
    dynvertex* goto_first() {
        return dvtx = (dynvertex*)
         (dvtx_item = dynamics->goto_first())->get_data();
    }
    dynvertex* goto_last() {
        return dvtx = (dynvertex*)
         (dvtx_item = dynamics->goto_last())->get_data();
    }
    dynvertex* goto_prev() {
        return dvtx = (dynvertex*)
         (dvtx_item = dynamics->goto_prev())->get_data();
    }
    dynvertex* goto_next() {
        return dvtx = (dynvertex*)
         (dvtx_item = dynamics->goto_next())->get_data();
    }
    dynvertex* get_first() {
        return (dynvertex*)(dynamics->sneak_first())->get_data();
    }
    dynvertex* get_last() {
        return (dynvertex*)(dynamics->sneak_last())->get_data();
    }
    dynvertex* get_prev() {
        return (dynvertex*)(dynamics->sneak_prev())->get_data();
    }
    dynvertex* get_next() {
        return (dynvertex*)(dynamics->sneak_next())->get_data();
    }
    // returns dynvertex with sil > passed sil, and sets
    // current to the previous dynvertex with sil <= passed sil.
    dynvertex* goto_dvertex(double sil);
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    dobj* add_dobj(dobj*);
    synthmod* duplicate_module(const char* uname, DUP_IO);

private:
// inputs
    double const* in_signal;
    double const* in_mod;
// outputs
    double out_output;
    STATUS play_state;
// params
    double up_thresh;
    double lo_thresh;
    STATUS posnegmirror;
    STATUS use_ratios;
// working
    linkedlist* dynamics;
    ll_item* dvtx_item;
    ll_item* dvtx_item1;
    dynvertex* dvtx;
    dynvertex* dvtx1;
    double thresh_range;
    void create_params();
    void create_dobj();
    static bool done_params;
    static bool done_dobj;
};

#endif
