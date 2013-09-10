#ifndef STATEGATETRIG_H
#define STATEGATETRIG_H

#include "synthmodule.h"
#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"


/*    only outputs the input trigger when the input state is ON */


class stategatetrig: public synthmod
{
public:
    stategatetrig(char const*);
    ~stategatetrig();
    void set_input_trig(STATUS const* i){ in_trig = i; }
    void set_input_state(STATUS const* i){ in_state = i; }
    STATUS const* get_input_trig(){ return in_trig;}
    STATUS const* get_input_state(){ return in_state;}
    STATUS const* get_output_trig(){ return &out_trig;}
    // virtual funcs
    void run();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);

private:
    STATUS const* in_trig;
    STATUS const* in_state;
    STATUS out_trig;
    static int stategatetrig_count;
};

#endif
