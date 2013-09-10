#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmodule.h"

class wcnt_signal : public synthmod
{
public:
    wcnt_signal(char const*);
    ~wcnt_signal();
    // this method used by combiner/switcher etc to get output...
    const double* get_output() const { return &out_output; }
    // virtual funcs
    void run() { out_output = *in_signal * level; }
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
private:
    const double* in_signal;
    double out_output;
    double level;
    void create_params();
    static bool done_params;
};

#endif
