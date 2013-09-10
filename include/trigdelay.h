#ifndef TRIGDELAY_H
#define TRIGDELAY_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class trigdelay : public synthmod
{
public:
    trigdelay(char const*);
    ~trigdelay();
    void set_input_trig(const STATUS* in){ in_trig = in;}
    const STATUS* get_input_trig(){ return in_trig;}
    const STATUS* get_out_trig(){ return &out_trig;}
    void set_delay_time(double dt){ delay_time = dt;}
    double get_delay_time(){ return delay_time;}
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
    // inputs
    const STATUS* in_trig;
    // outputs
    STATUS out_trig;
    // params
    double delay_time;
    // working
    STATUS* past_trigs; //array
    long pastmax;
    long pastpos;
    static int trigdelay_count;
    void create_params();
    static bool done_params;
};

#endif
