#ifndef BALANCE_H
#define BALANCE_H

#include "synthmodule.h"

class balance : public synthmod
{
public:
    balance(char const*);
    ~balance();
    enum FUNC { ADD, SUB, MUL};
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    FUNC func;
    double bias;
    void create_params();
    static bool done_params;
};

#endif
