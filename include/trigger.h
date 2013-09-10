#ifndef TRIGGER_H
#define TRIGGER_H

#include "synthmod.h"

class trigger: public synthmod
{
public:
    trigger(char const*);
    ~trigger();
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
private:
    double const* in_signal;
    STATUS out_trig;
    STATUS out_not_trig;
    STATUS out_wait_state; // waiting for signal to drop back down
    double delay_time;
    double trigger_level;
    unsigned long delay_samps;
    void create_params();
    static bool done_params;
};

#endif
