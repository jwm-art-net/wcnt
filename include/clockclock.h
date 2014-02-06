#ifndef CLOCKCLOCK_H
#define CLOCKCLOCK_H

#include "synthmod.h"

class clockclock : public synthmod::base
{
 public:
    clockclock(const char* uname);
    ~clockclock();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
 private:
    STATUS out_phase_trig;
    double out_premod_phase_step;
    double out_phase_step;
    const double* in_freq_mod1;
    double hrtz_freq;
    double freq_mod1size;
    double mod1size;
    double degs;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
