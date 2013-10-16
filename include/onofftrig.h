#ifndef ONOFFTRIG_H
#define ONOFFTRIG_H

#include "synthmod.h"

class onofftrig: public synthmod::base
{
public:
    onofftrig(const char*);
    ~onofftrig();
    void run();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

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
    samp_t attack_samps;
    samp_t release_samps;
    bool do_attack;// false == do_release
    void register_ui();
};

#endif
