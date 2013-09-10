#ifndef MULTIPLIER_H
#define MULTIPLIER_H

// very simple module to multiply to signals together

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class multiplier : public synthmod
{
public:
    multiplier(char const*);
    ~multiplier();
    // inputs
    void set_input_signal(const double* s){ in_signal = s;}
    void set_input_modifier(const double* md){ in_mod = md;}
    const double* get_signal_in(){ return in_signal;}
    const double* get_modifier(){ return in_mod;}
    // output
    const double * get_output(){return &out_output;}
    // virtual funcs - because you're worth it.
    void run() { out_output = *in_signal * *in_mod;	}
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);

private:
    const double* in_signal;
    const double* in_mod;
    double out_output;
    static int multiplier_count;
};
#endif
