#ifndef MULTIPLIER_H
#define MULTIPLIER_H

// very simple module to multiply to signals together

#include "synthmod.h"

class multiplier : public synthmod
{
public:
    multiplier(char const*);
    ~multiplier();
    void run() { out_output = *in_signal1 * *in_signal2; }
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
};
#endif
