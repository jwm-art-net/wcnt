#ifndef SAMPLECLIMB_H
#define SAMPLECLIMB_H

#include "synthmod.h"


class sample_climb : public synthmod::base
{
 public:
    sample_climb(const char*);
    ~sample_climb();
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double rate;
    double target;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
