#ifndef SIMPLEDELAY_H
#define SIMPLEDELAY_H

#include "synthmodule.h"

class simple_delay : public synthmod
{
public:
    simple_delay(char const*);
    ~simple_delay();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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
    void create_params();
    static bool done_params;
};

#endif
