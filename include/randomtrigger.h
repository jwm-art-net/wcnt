#ifndef RANDOMTRIGGER_H
#define RANDOMTRIGGER_H

#include <stdlib.h>
#include <time.h>

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class randomtrigger: public synthmod
{
public:
    randomtrigger(char const*);
    ~randomtrigger();
    void set_input_trig(STATUS const* it){ in_trig = it; }
    STATUS const* get_input_trig(){ return in_trig;}
    STATUS const* get_output_trig(){ return &out_trig;}
    STATUS const* get_output_not_trig(){ return &out_not_trig;}
    void set_probability(double p){ probability = p;}
    void set_not_probability(double p){not_probability = p;}
    double get_probability(){ return probability;}
    double get_not_probability(){ return not_probability;}
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_not_trig;
    double probability;
    double not_probability;
    static int randomtrigger_count;
    void create_params();
    static bool done_params;
};

#endif
