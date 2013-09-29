#ifndef MULTIPLIER_H
#define MULTIPLIER_H

// very simple module to multiply to signals together

#include "synthmod.h"

class multiplier : public synthmod
{
public:
    multiplier(const char*);
    ~multiplier();
    void run() { out_output = *in_signal1 * *in_signal2; }
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
};
#endif
