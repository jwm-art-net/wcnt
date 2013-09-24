#ifndef TRIGECHO_H
#define TRIGECHO_H

#include "synthmod.h"

class trigecho : public synthmod
{
public:
    trigecho(const char*);
    ~trigecho();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

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
    samp_t pastmax;
    samp_t pastpos;
    double vel_count_ratio;
    void init_first();
    
};

#endif
