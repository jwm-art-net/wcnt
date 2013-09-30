#ifndef SAMPLECLIMB_H
#define SAMPLECLIMB_H

#include "synthmod.h"


class sample_climb : public synthmod
{
public:
    sample_climb(const char*);
    ~sample_climb();
    void run();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double rate;
    double target;
    void init_first();
    
};

#endif
