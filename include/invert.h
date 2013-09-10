#ifndef INVERT_H
#define INVERT_H

// very simple module to invert a signal

#ifdef BARE_MODULES
#include "synthmodule.h"
#else
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class invert : public synthmod
{
public:
    invert(char const*);
    ~invert();
    // inputs
    void set_input_signal(const double* s){ in_signal = s;}
    const double* get_signal_in(){ return in_signal;}
    // output
    const double * get_output(){return &out_output;}
    // virtual funcs - because you're worth it.
    void run() { out_output = -*in_signal; }
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
#endif

private:
    const double* in_signal;
    double out_output;
    static int invert_count;
};
#endif