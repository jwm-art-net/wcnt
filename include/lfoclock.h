#ifndef LFOCLOCK_H
#define LFOCLOCK_H

#include "synthmod.h"

class lfo_clock : public synthmod
{
public:
    lfo_clock(const char*);
    ~lfo_clock();
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    STATUS out_phase_trig;
    double out_phase_step;
    double out_premod_phase_step;
    int note_length_freq;
    double hrtz_freq;
    const STATUS* in_phase_trig;
    const double* in_freq_mod1;
    const double* in_freq_mod2;
    double freq_mod1size;
    double freq_mod2size;
    double mod1size;
    double mod2size;
    double degs;
    double degsize1;
    double degsize2;
    void init_first();
    
};

#endif
