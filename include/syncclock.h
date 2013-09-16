#ifndef SYNCCLOCK_H
#define SYNCCLOCK_H

#include "synthmod.h"

class sync_clock : public synthmod
{
public:
    sync_clock(char const*);
    ~sync_clock();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    // inputs
    const double* in_bpm;
    const double* in_pos_stepsz;
    const short* in_beats_per_bar;
    const short* in_beat_value;
    const STATUS* in_phase_trig;
    const double* in_freq_mod1;
    const double* in_freq_mod2;
    // outputs
    STATUS out_phase_trig;
    double out_phase_step;
    // parameters
    double freq_mod1size;
    double freq_mod2size;
    short quarter_val;
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
    void create_params();
    
};

#endif
