#ifndef STATEGATETRIG_H
#define STATEGATETRIG_H

#include "synthmod.h"

/*    only outputs the input trigger when the input state is ON */

class stategatetrig: public synthmod
{
public:
    stategatetrig(const char*);
    ~stategatetrig();
    void run();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;

private:
    STATUS const* in_trig;
    STATUS const* in_state;
    STATUS out_trig;
    STATUS out_not_trig;
};

#endif
