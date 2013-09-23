#ifndef IMPULSE_H
#define IMPULSE_H

#include "synthmod.h"

class impulse : public synthmod
{
public:
    impulse(const char*);
    ~impulse();
    void run();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
private:
    const STATUS* in_trig;
    double out_output;
};

#endif
