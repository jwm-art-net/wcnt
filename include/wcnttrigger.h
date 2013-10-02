#ifndef WCNTTRIGGER_H
#define WCNTTRIGGER_H

#include "synthmod.h"

class wcnt_trigger : public synthmod
{
public:
    wcnt_trigger(const char*);
    ~wcnt_trigger();
    // this method needed by trigswitcher (etc?)...
    const STATUS* get_out_trig() const { return in_trig; }
    void run(){};
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
private:
    const STATUS* in_trig;
};

#endif
