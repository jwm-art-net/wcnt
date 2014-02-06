#ifndef STATEGATETRIG_H
#define STATEGATETRIG_H

#include "synthmod.h"

/*    only outputs the input trigger when the input state is ON */

class stategatetrig: public synthmod::base
{
public:
    stategatetrig(const char*);
    ~stategatetrig();
    void run();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;

private:
    STATUS const* in_trig;
    STATUS const* in_state;
    STATUS out_trig;
    STATUS out_not_trig;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
