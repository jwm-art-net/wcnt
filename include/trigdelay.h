#ifndef TRIGDELAY_H
#define TRIGDELAY_H

#include "synthmod.h"

class trigdelay : public synthmod
{
public:
    trigdelay(const char*);
    ~trigdelay();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

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
    void init_first();
    
};

#endif
