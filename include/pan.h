#ifndef PAN_H
#define PAN_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class pan : public synthmod
{
public:
    pan(char const*);
    ~pan();
    void set_input_signal(const double* in){ in_signal = in; }
    void set_input_pan_mod(const double* pm){ in_pan_mod = pm; }
    const double* get_input_signal(){ return in_signal; }
    const double* get_input_pan_mod(){ return in_pan_mod; }
    const double* get_output_l(){ return &out_l; }
    const double* get_output_r(){ return &out_r; }
    void set_pan(double p){ panpos = p; }
    void set_pan_modsize(double pms){ pan_modsize = pms; }
    double get_pan(){ return panpos; }
    double get_pan_modsize(){ return pan_modsize; }
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    // inputs
    const double* in_signal;
    const double* in_pan_mod;
    // outputs
    double out_l;
    double out_r;
    // params
    double panpos;
    double pan_modsize;
    // working
    double pan_mod;
    double pan_pos;
    static int pan_count;
    void create_params();
    static bool done_params;
};

#endif
