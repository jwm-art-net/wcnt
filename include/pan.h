#ifndef PAN_H
#define PAN_H

#include "synthmodule.h"

class pan : public synthmod
{
public:
    pan(char const*);
    ~pan();
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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
    void create_params();
    static bool done_params;
};

#endif
