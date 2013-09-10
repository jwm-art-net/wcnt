#ifndef LOGICTRIGGER_H
#include "../include/logictrigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

logictrigger::logictrigger(char const* uname) :
 synthmod(synthmodnames::LOGICTRIGGER, uname),
 in_trig1(0), in_trig2(0), out_trig(OFF), logicfunc(AND), precision(0),
 t1_samps(0), t2_samps(0), trig1(OFF), trig2(OFF)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG1);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG2);
    create_params();
}

logictrigger::~logictrigger()
{
}

void const* logictrigger::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG: return &out_trig;
        default: return 0;
    }
}

void const* logictrigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG1: return in_trig1 = (STATUS*)o;
        case inputnames::IN_TRIG2: return in_trig2 = (STATUS*)o;
        default: return 0;
    }
}

void const* logictrigger::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG1: return in_trig1;
        case inputnames::IN_TRIG2: return in_trig2;
        default: return 0;
    }
}

bool logictrigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::LOGICFUNC:
            logicfunc = (LOGIC_FUNC)(*(int*)data);
            return true;
        case paramnames::PRECISION:
            precision = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* logictrigger::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::LOGICFUNC: return &logicfunc;
        case paramnames::PRECISION: return &precision;
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

bool logictrigger::done_params = false;

void logictrigger::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(synthmodnames::LOGICTRIGGER,
        paramnames::LOGICFUNC);
    jwm.get_fxsparamlist()->add_param("and/or/xor/xornot",
        paramnames::LOGICFUNC);
    jwm.get_paramlist()->add_param(synthmodnames::LOGICTRIGGER,
        paramnames::PRECISION);
    done_params = true;
}
#endif
