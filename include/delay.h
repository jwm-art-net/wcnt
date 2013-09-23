#ifndef DELAY_H
#define DELAY_H

#include "synthmod.h"

class delay : public synthmod
{
public:
    delay(const char*);
    ~delay();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    // inputs
    const double* in_signal;
    const double* in_gainmod;
    // outputs
    double out_output;
    // params
    double delay_time;
    double gain;
    double gain_modsize;
    double wetdry;
    // working
    double output;
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    double gainamount;
    void init_first();
    
};

#endif
