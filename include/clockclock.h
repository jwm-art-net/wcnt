#ifndef CLOCKCLOCK_H
#define CLOCKCLOCK_H

#include "synthmod.h"

class clockclock : public synthmod
{
 public:
    clockclock(char const* uname);
    ~clockclock();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
 private:
    STATUS out_phase_trig;
    double out_premod_phase_step;
    double out_phase_step;
    const double* in_freq_mod1;
    double hrtz_freq;
    double freq_mod1size;
    double mod1size;
    double degs;
    void create_params();
    
};

#endif
