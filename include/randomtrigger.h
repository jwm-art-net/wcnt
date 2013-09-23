#ifndef RANDOMTRIGGER_H
#define RANDOMTRIGGER_H

#include "synthmod.h"

class randomtrigger: public synthmod
{
public:
    randomtrigger(const char*);
    ~randomtrigger();
    void run();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    STATUS const* in_trig;
    STATUS out_trig;
    STATUS out_not_trig;
    double probability;
    double not_probability;
    void init_first();
    
};

#endif
