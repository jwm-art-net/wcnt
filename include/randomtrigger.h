#ifndef RANDOMTRIGGER_H
#define RANDOMTRIGGER_H

#include "synthmod.h"

class randomtrigger: public synthmod
{
public:
    randomtrigger(const char*);
    ~randomtrigger();
    void run();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_not_trig;
    double probability;
    double not_probability;
    void init_first();
    
};

#endif
