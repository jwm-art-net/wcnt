#ifndef TRIGECHO_H
#define TRIGECHO_H

#include "synthmod.h"

class trigecho : public synthmod
{
public:
    trigecho(char const*);
    ~trigecho();
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
    const STATUS* in_trig;
    // outputs
    STATUS out_trig;
    double out_velocity;
    short  out_count;
    // params
    double delay_time;
    short count;
    STATUS send_input_out;
    // working
    short* past_trigs; //array
    unsigned long pastmax;
    unsigned long pastpos;
    double vel_count_ratio;
    void create_params();
    static bool done_params;
};

#endif
