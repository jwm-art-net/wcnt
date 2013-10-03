#ifndef SYNCCLOCK_H
#define SYNCCLOCK_H

#include "synthmod.h"

class sync_clock : public synthmod
{
public:
    sync_clock(const char*);
    ~sync_clock();
    void run();
    void init();
    errors::TYPE validate();
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const double* in_bpm;
    const double* in_pos_stepsz;
    const wcint_t* in_beats_per_bar;
    const wcint_t* in_beat_value;
    const STATUS* in_phase_trig;
    const double* in_freq_mod1;
    const double* in_freq_mod2;
    // outputs
    STATUS out_phase_trig;
    double out_phase_step;
    // parameters
    double freq_mod1size;
    double freq_mod2size;
    wcint_t  tpqn;
    double note_length;
    double snap_to;
    // working
    double premod_nlen;
    double mod1size;
    double mod2size;
    double posconv;
    double phaselen;
    double beatlen;
    double phasepos;
    double snapto;
    void init_first();
};

#endif
