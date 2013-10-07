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
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const STATUS* in_trig;
    const STATUS* in_reset_trig;
    // outputs
    STATUS out_trig;
    double out_velocity;
    wcint_t  out_index;
    // params
    double delay_time;
    wcint_t count;
    STATUS send_input_out;
    // working
    wcint_t* past_trigs; //array
    samp_t pastmax;
    samp_t pastpos;
    double vel_count_ratio;
    wcint_t last;
    void init_first();
};

#endif
