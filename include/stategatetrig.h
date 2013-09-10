#ifndef STATEGATETRIG_H
#define STATEGATETRIG_H

#include "synthmod.h"

/*    only outputs the input trigger when the input state is ON */

class stategatetrig: public synthmod
{
public:
    stategatetrig(char const*);
    ~stategatetrig();
    void run();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    STATUS const* in_trig;
    STATUS const* in_state;
    STATUS out_trig;
};

#endif
