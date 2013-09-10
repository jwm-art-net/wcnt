#ifndef WCNTTRIGGER_H
#define WCNTTRIGGER_H

#include "synthmodule.h"
#include "dobj.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class wcnt_trigger : public synthmod
{
public:
    wcnt_trigger(char const*);
    ~wcnt_trigger();
    void set_in_trig(const STATUS* is){ in_trig = is;}
    const STATUS* get_in_trig(){ return in_trig;}
    const STATUS* get_out_trig(){ return in_trig;}
    // virtual funcs
    void run(){};
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
private:
    const STATUS* in_trig;
    static int wcnt_trigger_count;
};

#endif
