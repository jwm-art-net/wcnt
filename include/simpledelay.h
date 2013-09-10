#ifndef SIMPLEDELAY_H
#define SIMPLEDELAY_H

#include <math.h>

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class simple_delay : public synthmod
{
public:
    simple_delay(char const*);
    ~simple_delay();
    void set_input_signal(const double* in){ in_signal = in;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_output(){ return &output;}
    void set_delay_time(double dt){ delay_time = dt;}
    double get_delay_time(){ return delay_time;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    // inputs
    const double* in_signal;
    // outputs
    double out_output;
    // params
    double delay_time;
    // working
    double output;
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    static int simple_delay_count;
    void create_params();
    static bool done_params;
};

#endif
