#ifndef SAMPLECLIMB_H
#define SAMPLECLIMB_H

#include "synthmod.h"


class sample_climb : public synthmod
{
public:
    sample_climb(const char*);
    ~sample_climb();
    void run();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double rate;
    double target;
    void init_first();
    
};

#endif
