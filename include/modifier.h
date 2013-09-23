#ifndef MODIFIER_H
#define MODIFIER_H

#include "synthmod.h"

class modifier : public synthmod
{
public:
    modifier(const char*);
    ~modifier();
    enum FUNC { ADD, SUB, MUL};
    enum TYPE { M1, M2 };
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
    const double* in_bias;
    double out_output;
    FUNC func;
    TYPE type;
    void init_first();
    
};

#endif
