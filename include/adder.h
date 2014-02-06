#ifndef ADDER_H
#define ADDER_H

#include "synthmod.h"

class adder : public synthmod::base
{
 public:
    adder(const char*);
    ~adder();
    void run() { out_output = *in_signal1 + *in_signal2; }
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;

 private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
