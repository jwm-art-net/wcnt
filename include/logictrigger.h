#ifndef LOGICTRIGGER_H
#define LOGICTRIGGER_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

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
    logictrigger(char const*);
    ~logictrigger();
    void set_input_trig1(STATUS const* it){ in_trig1 = it; }
    void set_input_trig2(STATUS const* it){ in_trig2 = it; }
    STATUS const* get_input_trig1(){ return in_trig1;}
    STATUS const* get_input_trig2(){ return in_trig2;}
    STATUS const* get_output_trig(){ return &out_trig;}
    void set_logicfunc(LOGIC_FUNC ft) { logicfunc = ft;}
    LOGIC_FUNC get_logicfunc(){ return logicfunc;}
    void set_precision(short p){ precision = p;}
    // virtual funcs
    void run();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

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
    static int logictrigger_count;
    void create_params();
    static bool done_params;
};

#endif
