#ifndef IMPULSE_H
#define IMPULSE_H

#include "synthmod.h"

class impulse : public synthmod::base
{
public:
    impulse(const char*);
    ~impulse();
    void run();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
private:
    const STATUS* in_trig;
    double out_output;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
