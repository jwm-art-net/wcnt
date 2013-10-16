#ifndef TRIGCOUNTER_H
#define TRIGCOUNTER_H

#include "synthmod.h"

class trigcounter : public synthmod::base
{
public:
    trigcounter(const char*);
    ~trigcounter();
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
    STATUS out_not_trig;
    wcint_t  out_pre_count;
    wcint_t  out_count;
    STATUS out_play_state;
    // params
    wcint_t pre_count;
    wcint_t count;
    STATUS wrap;
    void register_ui();
};

#endif
