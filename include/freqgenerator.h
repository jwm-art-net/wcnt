#ifndef FREQGENERATOR_H
#define FREQGENERATOR_H

#include "synthmod.h"

class freq_generator : public synthmod::base
{
public:
    freq_generator(const char*);
    ~freq_generator();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    const double* in_signal;
    double	out_freq,
    out_phase_step,
    sig_range_hi,
    sig_range_lo,
    freq_range_hi,
    freq_range_lo,
    sig_step_size,
    freq_step_size;
    wcint_t step_count;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
