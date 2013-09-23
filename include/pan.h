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
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

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
