#ifndef BALANCE_H
#define BALANCE_H

#include "synthmod.h"

class balance : public synthmod
{
public:
    balance(const char*);
    ~balance();
    enum FUNC { ADD, SUB, MUL};
    void run();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    FUNC func;
    double bias;
    void init_first();
};

#endif
