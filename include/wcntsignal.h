#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmod.h"

class wcnt_signal : public synthmod
{
public:
    wcnt_signal(char const*);
    ~wcnt_signal();

    // virtual funcs
    void run() { out_output = *in_signal * level; }
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

    /*
    // make out_output public for those modules which contain lists of
    // wcnt_signal
    */
    double out_output;

private:
    const double* in_signal;
    double level;
    void create_params();
    static bool done_params;
};

#endif
