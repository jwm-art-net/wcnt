#ifndef IMPULSE_H
#define IMPULSE_H

#include "synthmod.h"

class impulse : public synthmod::base
{
public:
    impulse(const char*);
    ~impulse();
    void run();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
private:
    const STATUS* in_trig;
    double out_output;
    void register_ui();
};

#endif
