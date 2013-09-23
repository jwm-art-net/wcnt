#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmod.h"

// wcnt_signal
//  if level == 1.0, then output == input == use empty_run method
//  else output = input * level == use run method
//  during init (init method required only for this)
//  connectorlist::remake_connections is called.

class wcnt_signal : public synthmod
{
public:
    wcnt_signal(const char*);
    ~wcnt_signal();

    // virtual funcs
    void run() { out_output = *in_signal * level; }
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    const double* in_signal;
    double out_output;
    double level;
    void init_first();
    
};

#endif
