#ifndef LOGICTRIGGER_H
#include "../include/logictrigger.h"

logictrigger::logictrigger(char const* uname) :
 synthmod(synthmodnames::MOD_LOGICTRIGGER, logictrigger_count, uname),
 in_trig1(0), in_trig2(0), out_trig(OFF), logicfunc(AND), precision(0),
 t1_samps(0), t2_samps(0), trig1(OFF), trig2(OFF)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG1);
    get_inputlist()->add_input(this, inputnames::IN_TRIG2);
    logictrigger_count++;
    create_params();
}

logictrigger::~logictrigger()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* logictrigger::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* logictrigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG1:
        i = in_trig1 = (STATUS*)o;
        break;
    case inputnames::IN_TRIG2:
        i = in_trig2 = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool logictrigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_LOGICFUNC:
        set_logicfunc((LOGIC_FUNC)(*(int*)data));
        retv = true;
        break;
    case paramnames::PAR_PRECISION:
        set_precision(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* logictrigger::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_LOGICFUNC:
        return &logicfunc;
    case paramnames::PAR_PRECISION:
        return &precision;
    default:
        return 0;
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

int logictrigger::logictrigger_count = 0;

bool logictrigger::done_params = false;

void logictrigger::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_LOGICTRIGGER, paramnames::PAR_LOGICFUNC);
    get_fxsparamlist()->add_param("and/or/xor/xornot", 
     paramnames::PAR_LOGICFUNC);
    get_paramlist()->add_param(
     synthmodnames::MOD_LOGICTRIGGER, paramnames::PAR_PRECISION);
    done_params = true;
}
#endif
