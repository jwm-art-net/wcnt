#ifndef MODIFIER_H
#define MODIFIER_H

#include "synthmod.h"

class modifier : public synthmod::base
{
public:
    modifier(const char*);
    ~modifier();
    enum FUNC { ADD, SUB, MUL};
    enum TYPE { M1, M2 };
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
    const double* in_bias;
    double out_output;
    FUNC func;
    TYPE type;
    void register_ui();
};

#endif
