#ifndef CONSTMOD_H
#define CONSTMOD_H

#include "synthmod.h"

class constmod : public synthmod
{
public:
    constmod(char const*);
    ~constmod();
    void run(){};
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    double output;
    void create_params();
    static bool done_params;
};

#endif
