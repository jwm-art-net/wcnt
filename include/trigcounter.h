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
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

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
    ui::moditem_list* get_ui_items();
};

#endif
