#ifndef SIMPLEDELAY_H
#define SIMPLEDELAY_H

#include "synthmod.h"

class simple_delay : public synthmod::base
{
 public:
    simple_delay(const char*);
    ~simple_delay();
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
    const double* in_signal;
    // outputs
    double out_output;
    // params
    double delay_time;
    // working
    double output;
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
