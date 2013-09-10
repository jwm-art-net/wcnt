#ifndef ADDER_H
#define ADDER_H

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class adder : public synthmod
{
public:
    adder(char const*);
    ~adder();
    void set_input_signal(const double* s){ in_signal = s;}
    void set_input_modifier(const double* md){ in_mod = md;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_input_modifier(){ return in_mod;}
    // output
    const double * get_output(){return &out_output;}
    // virtual funcs
    void run();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);

private:
    const double* in_signal;
    const double* in_mod;
    double out_output;
    static int adder_count;
    void create_params();
    static bool done_params;
};

#endif
