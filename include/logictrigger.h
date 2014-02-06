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

class logictrigger: public synthmod::base
{
public:
    enum LOGIC_FUNC{ AND, OR, XOR, XORNOT };
    logictrigger(const char*);
    ~logictrigger();
    void run();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    errors::TYPE validate();

private:
    STATUS const* in_trig1;
    STATUS const* in_trig2;
    STATUS out_trig;
    // params
    LOGIC_FUNC logicfunc;
    wcint_t precision;
    // working
    STATUS const* next_trig;
    wcint_t t1_samps;
    wcint_t t2_samps;
    STATUS trig1;
    STATUS trig2;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
