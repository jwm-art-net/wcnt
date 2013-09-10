#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmodule.h"
#include "dobj.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class wcnt_signal : public synthmod
{
public:
    wcnt_signal(char const*);
    ~wcnt_signal();
    void set_input_signal(const double* is){ in_signal = is;}
    const double* get_input_signal(){ return in_signal;}
    void set_level(double l){ level = l;}
    double get_level(){return level;}
    const double* get_output(){ return &out_output;}
    // virtual funcs
    void run()
    { out_output = *in_signal * level; }
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
private:
    const double* in_signal;
    double out_output;
    double level;
    static int wcnt_signal_count;
    void create_params();
    static bool done_params;
};

#endif
