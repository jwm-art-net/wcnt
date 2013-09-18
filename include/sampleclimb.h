#ifndef SAMPLECLIMB_H
#define SAMPLECLIMB_H

#include "synthmod.h"


class sample_climb : public synthmod
{
public:
    sample_climb(char const*);
    ~sample_climb();
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double rate;
    double target;
    void init_first();
    
};

#endif
