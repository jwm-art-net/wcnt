#ifndef IMPULSE_H
#define IMPULSE_H

#include "synthmod.h"

class impulse : public synthmod
{
public:
    impulse(char const*);
    ~impulse();
    void run();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
private:
    const STATUS* in_trig;
    double out_output;
};

#endif
