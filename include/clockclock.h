#ifndef CLOCKCLOCK_H
#define CLOCKCLOCK_H

#include "synthmodule.h"
#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class clockclock : public synthmod
{
public:
    clockclock(char const* uname);
    ~clockclock();
    void set_input_freq_mod1(const double* fm){in_freq_mod1 = fm;}
    const double* get_input_freq_mod1(){ return in_freq_mod1; }
    const STATUS* get_output_phase_trig(){ return &out_phase_trig;}
    const double* get_output_premod_deg_size(){
        return &out_premod_deg_size;
    }
    const double* get_output_deg_size(){ return &out_deg_size;}
    void set_frequency(double f);
    void set_freq_mod1size(double fms){ freq_mod1size = fms; }
    double get_frequency(){ return hrtz_freq; }
    double get_freq_mod1size()	{ return freq_mod1size; }
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    STATUS out_phase_trig;
    double out_premod_deg_size;
    double out_deg_size;
    const double* in_freq_mod1;
    double hrtz_freq;
    double freq_mod1size;
    double mod1size;
    double degs;
    static int clockclock_count;
    void create_params();
    static bool done_params;
};

#endif
