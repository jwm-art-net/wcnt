#ifndef ECHO_H
#define ECHO_H

#include "synthmod.h"

class echo : public synthmod
{
public:
    echo(char const*);
    ~echo();
    // virtual funcs
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
    const double* in_gainmod;
    const double* in_feedback;
    const double* in_feed_mod;
    // output
    double output;
    double wet_output;
    // params
    double delay_time;
    double gain;
    double gain_modsize;
    double feed_level;
    double feed_modsize;
    double wetdry;
    // working
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    double gainamount;
    double feedamount;
    // synthmod kind of stuff
    void init_first();
    
};

#endif
