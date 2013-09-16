#ifndef ADDER_H
#define ADDER_H

#include "synthmod.h"

class adder : public synthmod
{
public:
    adder(char const*);
    ~adder();
    void run() { out_output = *in_signal1 + *in_signal2; }
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    void create_params();
    
};

#endif
