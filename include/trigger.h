#ifndef TRIGGER_H
#define TRIGGER_H

#include "synthmod.h"

class trigger: public synthmod
{
public:
    trigger(const char*);
    ~trigger();
    void run();
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
private:
    double const* in_signal;
    STATUS out_trig;
    STATUS out_not_trig;
    STATUS out_wait_state; // waiting for signal to drop back down
    double delay_time;
    double trigger_level;
    unsigned long delay_samps;
    void init_first();
    
};

#endif
