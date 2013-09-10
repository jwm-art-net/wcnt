#ifndef TRIGGER_H
#define TRIGGER_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class trigger: public synthmod
{
public:
    trigger(char const*);
    ~trigger();
    void set_input_signal(double const* is){ in_signal = is; }
    double const* get_input_signal(){ return in_signal;}
    STATUS const* get_output_trig(){ return &out_trig;}
    STATUS const* get_output_not_trig(){ return &out_not_trig;}
    void set_attack_time(double ms){ attack_time = ms; }
    void set_release_time(double ms){ release_time = ms; }
    void set_attack_level(double tl){ attack_level = tl; }
    void set_release_level(double tl){ release_level = tl; }
    void set_independant_mode(STATUS im) { ind_mode = im; }
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif
private:
    double const* in_signal;
    STATUS out_trig;
    STATUS out_not_trig;
    double attack_time;
    double release_time;
    double attack_level;
    double release_level;
    STATUS ind_mode;
    unsigned long attack_samps;
    unsigned long release_samps;
    static int trigger_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
