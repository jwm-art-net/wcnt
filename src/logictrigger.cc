#include "../include/logictrigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

logictrigger::logictrigger(const char* uname) :

 synthmod(module::LOGICTRIGGER, uname, SM_HAS_OUT_TRIG),
 in_trig1(0), in_trig2(0), out_trig(OFF), logicfunc(AND), precision(0),
 next_trig(0), t1_samps(0), t2_samps(0), trig1(OFF), trig2(OFF)
{
    register_input(input::IN_TRIG1);
    register_input(input::IN_TRIG2);
    register_output(output::OUT_TRIG);
    init_first();
}

logictrigger::~logictrigger()
{
}

const void* logictrigger::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG: return &out_trig;
        default: return 0;
    }
}

const void* logictrigger::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG1: return in_trig1 = (STATUS*)o;
        case input::IN_TRIG2: return in_trig2 = (STATUS*)o;
        default: return 0;
    }
}

const void* logictrigger::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG1: return in_trig1;
        case input::IN_TRIG2: return in_trig2;
        default: return 0;
    }
}

bool logictrigger::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::LOGICFUNC:
            logicfunc = (LOGIC_FUNC)(*(int*)data);
            return true;
        case param::PRECISION:
            precision = *(short*)data;
            return true;
        default:
            return false;
    }
}

const void* logictrigger::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::LOGICFUNC: return &logicfunc;
        case param::PRECISION: return &precision;
        default: return 0;
    }
}

void logictrigger::run()
{
    if (logicfunc == AND)
    {
        if (t1_samps == 0) {
            if (*in_trig1 == ON && *in_trig2 == ON)
                out_trig = ON;
            else {
                if (out_trig == ON) out_trig = OFF;
                if (*in_trig1 == ON) {
                    t1_samps = precision;
                    next_trig = in_trig2;
                }
                else if (*in_trig2 == ON) {
                    t1_samps = precision;
                    next_trig = in_trig1;
                }
            }
        }
        else {
            if (*next_trig == ON) {
                out_trig = ON;
                t1_samps = 0;
            }
            else t1_samps--;
        }
    } 
    else if (logicfunc == OR)
    {
        if (t1_samps == 0) {
            if (*in_trig1 == ON || *in_trig2 == ON) {
                out_trig = ON;
                t1_samps = precision;
            } else if (precision == 0 && out_trig == ON)
                out_trig = OFF;
        }
        else {
            t1_samps--;
            if (out_trig == ON) out_trig = OFF;
        }
    }
    else if (logicfunc == XOR)
    {
        if (t1_samps == 0) {
            if (*in_trig1 == ON && *in_trig2 == OFF) {
                if (precision == 0)
                    out_trig = ON;
                else {
                    t1_samps = precision;
                    next_trig = in_trig2;
                }
            }
            else if (*in_trig1 == OFF && *in_trig2 == ON) {
                if (precision == 0)
                    out_trig = ON;
                else {
                    t1_samps = precision;
                    next_trig = in_trig1;
                }
            }
            else if (out_trig == ON) out_trig = OFF;
        }
        else {
            if (*next_trig == ON)
                t1_samps = 0;
            else {
                t1_samps--;
                if (t1_samps == 0)
                    out_trig = ON;
            }
        }
    }
    else if (logicfunc == XORNOT)
    {
        if (t1_samps == 0) {
            if (*in_trig1 == ON && *in_trig2 == OFF) {
                if (precision == 0)
                    out_trig = ON;
                else {
                    t1_samps = precision;
                    next_trig = in_trig2;
                }
            }
            else if (*in_trig1 == OFF && *in_trig2 == ON) {
                t1_samps = precision;
                next_trig = in_trig1;
            }
            else if (out_trig == ON) out_trig = OFF;
        }
        else {
            if (*in_trig2 == ON)
                t1_samps = 0;
            else {
                t1_samps--;
                if (t1_samps == 0 && next_trig == in_trig2)
                    out_trig = ON;
            }
        }
    }
}



void logictrigger::init_first()
{
    if (done_first())
        return;
    register_param(param::LOGICFUNC, "and/or/xor/xornot");
    register_param(param::PRECISION);
}
