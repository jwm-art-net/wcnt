#ifndef ONOFFTRIG_H
#define ONOFFTRIG_H

#include "synthmod.h"

class onofftrig: public synthmod
{
public:
    onofftrig(char const*);
    ~onofftrig();
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
    STATUS out_attack_state;
    STATUS out_release_state;
    double attack_time;
    double release_time;
    double attack_level;
    double release_level;
    STATUS check_levels;
    unsigned long attack_samps;
    unsigned long release_samps;
    bool do_attack;// false == do_release
    void init_first();
    
};

#endif
