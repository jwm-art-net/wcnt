#ifndef TRIANGLEWAVE_H
#define TRIANGLEWAVE_H

#include "synthmodule.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class triangle_wave : public synthmod
{
public:
    triangle_wave(char const*);
    ~triangle_wave();
    void set_input_phase_trig(STATUS const* pt){ in_phase_trig = pt;}
    void set_input_deg_size(double const* ds){ in_deg_size = ds;}
    STATUS const* get_input_phase_trig(){ return in_phase_trig;}
    double const* get_input_deg_size(){ return in_deg_size;}
    double const* get_output(){ return &output;}
    STATUS const* get_play_state(){ return &play_state;}
    void set_recycle_mode(STATUS rm){ recycle = rm;}
    void set_zero_retrigger_mode(STATUS zrm){ zero_retrigger_mode = zrm;}
    // virtual funcs
    void run();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
private:
    STATUS const* in_phase_trig;
    double const* in_deg_size;
    double output;
    STATUS play_state;
    short sect;
    double sect_spanlvl;
    double sect_startlvl;
    double sectmaxsamps;
    double sectsample;
    double counter_ratio;
    STATUS recycle;
    STATUS zero_retrigger_mode;
    static short triangle_wave_count;
    void create_params();
    static bool done_params;
};

#endif
