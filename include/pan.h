#ifndef PAN_H
#define PAN_H

#include "synthmod.h"

class pan : public synthmod
{
public:
    pan(const char*);
    ~pan();
    void run();
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const double* in_signal;
    const double* in_pan_mod;
    // outputs
    double out_l;
    double out_r;
    // params
    double panpos;
    double pan_modsize;
    // working
    double pan_mod;
    double pan_pos;
    void init_first();
    
};

#endif
