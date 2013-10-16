#ifndef CONTRASTER_H
#define CONTRASTER_H

#include "synthmod.h"

// SND ___inspired____ this one. (SND is a sound editor)
//
// it multiplies a given signal by x amount.  if the result is not
// within the range (-1 ~ 1) it mirrors it until it is.
// something like that.
//
// yeah, um, right, my version does not do quite the same thing, but it
// does something to the sound which is enough in my book....

class contraster : public synthmod::base
{
public:
    contraster(const char*);
    ~contraster();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    double out_output;
    const double* in_signal;
    const double* in_power_mod;
    const STATUS* in_rude_switch_trig;
    double power_min;
    double power_max;
    STATUS rude_mode;
    double wetdry;
    double power;
    double power_mod;
    double powerrad;
    double output;
    void register_ui();
};

#endif
