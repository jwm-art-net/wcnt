#ifndef TRIGDELAY_H
#define TRIGDELAY_H

#include "synthmod.h"

class trigdelay : public synthmod
{
public:
    trigdelay(char const*);
    ~trigdelay();
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
    // params
    double delay_time;
    // working
    STATUS* past_trigs; //array
    long pastmax;
    long pastpos;
    void create_params();
    
};

#endif
