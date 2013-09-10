#ifndef TRIANGLEWAVE2_H
#define TRIANGLEWAVE2_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class triangle_wave2 : public synthmod
{
public:
    triangle_wave2(char const*);
    ~triangle_wave2();
    void set_input_phase_trig(STATUS const* pt){ in_phase_trig = pt;}
    void set_input_deg_size(double const* cs){ in_deg_size = cs;}
    void set_input_normal_mod(double const* nm){ in_normal_mod = nm;}
    STATUS const* get_input_phase_trig(){ return in_phase_trig;}
    double const* get_input_deg_size(){ return in_deg_size;}
    double const* get_input_normal_mod(){ return in_normal_mod;}
    double const* get_output(){ return &output;}
    STATUS const* get_play_state(){ return &play_state;}
    void set_normal_frequency(double nf){ normal_freq = nf;}
    void set_normal_modsize(double nhi){ norm_modsize = nhi;}
    void set_recycle_mode(STATUS rm){ recycle = rm;}
    void set_zero_retrigger_mode(STATUS zrm){ zero_retrigger_mode = zrm; }
    double get_normal_frequency(){ return normal_freq;}
    double get_normal_modsize(){ return norm_modsize;}
    STATUS get_recycle_mode(){ return recycle;}
    STATUS get_zero_retrigger_mode(){ return zero_retrigger_mode; }
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif
private:
    STATUS const* in_phase_trig;
    double const* in_deg_size;
    double const* in_normal_mod;
    double output;
    STATUS play_state;
    double normal_freq;
    double norm_modsize;
    STATUS recycle;
    STATUS zero_retrigger_mode;
    double nf_deg_size;
    double nf_pre_deg_size;
    double nf_sectmaxsamps;
    double nf_sectsample;
    double nf_counter_ratio;
    int	sect;
    double sect_spanlvl;
    double sect_startlvl;
    double old_maxsamps;
    double sectmaxsamps;
    double sectsample;
    double counter_ratio;
    static int triangle_wave2_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
