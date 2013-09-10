#ifndef WCNTTRIGGER_H
#define WCNTTRIGGER_H

#include "synthmod.h"

class wcnt_trigger : public synthmod
{
public:
    wcnt_trigger(char const*);
    ~wcnt_trigger();
    // this method needed by trigswitcher (etc?)...
    const STATUS* get_out_trig() { return in_trig; }
    void run(){};
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
private:
    const STATUS* in_trig;
};

#endif
