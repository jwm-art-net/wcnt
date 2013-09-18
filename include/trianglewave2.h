#ifndef TRIANGLEWAVE2_H
#define TRIANGLEWAVE2_H

#include "synthmod.h"

class triangle_wave2 : public synthmod
{
public:
    triangle_wave2(char const*);
    ~triangle_wave2();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
private:
    STATUS const* in_phase_trig;
    double const* in_phase_step;
    double const* in_normal_mod;
    double output;
    STATUS play_state;
    double normal_freq;
    double norm_modsize;
    STATUS recycle;
    STATUS zero_retrigger_mode;
    double nf_phase_step;
    double nf_pre_phase_step;
    double nf_sectmaxsamps;
    double nf_sectsample;
    double nf_counter_ratio;
    int sect;
    double sect_spanlvl;
    double sect_startlvl;
    double old_maxsamps;
    double sectmaxsamps;
    double sectsample;
    double counter_ratio;
    void init_first();
    
};

#endif
