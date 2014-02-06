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
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

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
    ui::moditem_list* get_ui_items();
};

#endif
