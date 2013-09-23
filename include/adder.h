#ifndef ADDER_H
#define ADDER_H

#include "synthmod.h"

class adder : public synthmod
{
public:
    adder(const char*);
    ~adder();
    void run() { out_output = *in_signal1 + *in_signal2; }
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
};

#endif
