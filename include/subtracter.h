#ifndef SUBTRACTER_H
#define SUBTRACT_H

#include "synthmod.h"

class subtracter : public synthmod::base
{
public:
    subtracter(const char*);
    ~subtracter();
    void run(){ out_output = *in_signal1 - *in_signal2; }
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

private:
    const double* in_signal1;
    const double* in_signal2;
    double out_output;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
