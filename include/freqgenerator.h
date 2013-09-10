#ifndef FREQGENERATOR_H
#define FREQGENERATOR_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class freq_generator : public synthmod
{
public:
    freq_generator(char const*);
    ~freq_generator();
    void set_signal_in(const double* insig){ in_signal = insig;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_output_freq(){ return &out_freq;}
    const double* get_output_deg_size(){ return &out_deg_size;}
    void set_signal_range_hi(double sh){ sig_range_hi = sh;}
    void set_signal_range_lo(double sl){ sig_range_lo = sl;}
    void set_freq_range_hi(double fh){ freq_range_hi = fh;}
    void set_freq_range_lo(double fl){ freq_range_lo = fl;}
    void set_step_count(short fs){ step_count = fs;}
    double get_signal_range_lo(){ return sig_range_hi;}
    double get_signal_range_hi(){ return sig_range_lo;}
    double get_freq_range_lo(){ return freq_range_hi;}
    double get_freq_range_hi(){ return freq_range_lo;}
    short get_step_count(){ return step_count;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    const double* in_signal;
    double	out_freq,
    out_deg_size,
    sig_range_hi,
    sig_range_lo,
    freq_range_hi,
    freq_range_lo,
    sig_step_size,
    freq_step_size;
    short step_count;
    static int freq_generator_count;
    void create_params();
    static bool done_params;
};

#endif
