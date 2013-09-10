#ifndef INVERT_H
#define INVERT_H

// very simple module to invert a signal

#include "synthmod.h"

class invert : public synthmod
{
public:
    invert(char const*);
    ~invert();
    void run() { out_output = -*in_signal; }
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    const double* in_signal;
    double out_output;
};
#endif
