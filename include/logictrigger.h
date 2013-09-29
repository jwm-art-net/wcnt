#ifndef LOGICTRIGGER_H
#define LOGICTRIGGER_H

#include "synthmod.h"

/*   new logic function - XORNOT
    -----------------------------
        only triggers when trig1 triggers but not if
        both trigger simultaneously

     new parameter - precision
    ---------------------------
        how many samples after a trigger occurrs to
        pretend it has only just triggered
        this is here to deal with synchronisation issues
        with time_map, sequencer, and sync_clock, as I've
        failed to achieve 100% sample accuracy, boooo.  */

class logictrigger: public synthmod
{
public:
    enum LOGIC_FUNC{ AND, OR, XOR, XORNOT };
    logictrigger(const char*);
    ~logictrigger();
    void run();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    STATUS const* in_trig1;
    STATUS const* in_trig2;
    STATUS out_trig;
    // params
    LOGIC_FUNC logicfunc;
    short precision;
    // working
    STATUS const* next_trig;
    short t1_samps;
    short t2_samps;
    STATUS trig1;
    STATUS trig2;
    void init_first();
    
};

#endif
