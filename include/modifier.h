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
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    const double* in_bias;
    double out_output;
    FUNC func;
    TYPE type;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
