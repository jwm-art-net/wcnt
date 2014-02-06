#ifndef RANDOMTRIGGER_H
#define RANDOMTRIGGER_H

#include "synthmod.h"

class randomtrigger: public synthmod::base
{
public:
    randomtrigger(const char*);
    ~randomtrigger();
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_not_trig;
    double probability;
    double not_probability;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
