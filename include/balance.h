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
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    FUNC func;
    double bias;
    void init_first();
    
};

#endif
