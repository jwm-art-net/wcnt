#ifndef TRIGGER_H
#define TRIGGER_H

#include "synthmod.h"

class trigger: public synthmod::base
{
 public:
    trigger(const char*);
    ~trigger();
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
    STATUS out_wait_state; // waiting for signal to drop back down
    double delay_time;
    double trigger_level;
    samp_t delay_samps;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
