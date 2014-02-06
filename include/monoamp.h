#ifndef MONOAMP_H
#define MONOAMP_H

#include "gain.h"
#include "synthmod.h"


class mono_amp : public synthmod::base, public gain
{
 public:
    mono_amp(const char*);
    ~mono_amp();
    void run();
    void init() { gain::init(); }
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    // inputs
    const double* in_signal;
    const double* in_amp_eg;

    // outputs
    double out_output;

    // params
    double clip_level;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
