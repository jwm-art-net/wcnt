#ifndef CONSTMOD_H
#define CONSTMOD_H

#include "synthmod.h"

class constmod : public synthmod::base
{
public:
    constmod(const char*);
    ~constmod();
    void run(){};
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    double output;
    void register_ui();
};

#endif
