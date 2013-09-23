#ifndef CONSTMOD_H
#define CONSTMOD_H

#include "synthmod.h"

class constmod : public synthmod
{
public:
    constmod(const char*);
    ~constmod();
    void run(){};
    const void* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    double output;
    void init_first();
    
};

#endif
