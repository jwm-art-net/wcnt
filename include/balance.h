#ifndef BALANCE_H
#define BALANCE_H

#include "synthmod.h"

class balance : public synthmod::base
{
public:
    balance(const char*);
    ~balance();
    enum FUNC { ADD, SUB, MUL};
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
    double out_output;
    FUNC func;
    double bias;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
