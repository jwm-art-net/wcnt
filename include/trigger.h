#ifndef TRIGGER_H
#define TRIGGER_H

#include "conversions.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class trigger: public synthmod
{
public:
    trigger(char const*);
    ~trigger();
    void set_input_signal(double const* is){ in_signal = is; }
    double const* get_input_signal(){ return in_signal;}
    STATUS const* get_output_trig(){ return &out_trig;}
    void set_delay_time(double ms){ delay_time = ms; }
    void set_trigger_level(double tl){ trigger_level = tl; }
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
private:
    double const* in_signal;
    STATUS out_trig;
    STATUS out_not_trig;
    STATUS out_wait_state; // waiting for signal to drop back down
    double delay_time;
    double trigger_level;
    unsigned long delay_samps;
    static int trigger_count;
    void create_params();
    static bool done_params;
};

#endif
