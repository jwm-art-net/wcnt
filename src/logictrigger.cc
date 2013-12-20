#include "../include/logictrigger.h"

logictrigger::logictrigger(const char* uname) :

 synthmod::base(synthmod::LOGICTRIGGER, uname, SM_HAS_OUT_TRIG),
 in_trig1(0), in_trig2(0), out_trig(OFF), logicfunc(AND), precision(0),
 next_trig(0), t1_samps(0), t2_samps(0), trig1(OFF), trig2(OFF)
{
    register_output(output::OUT_TRIG);
}

void logictrigger::register_ui()
{
    register_input(input::IN_TRIG1);
    register_param(param::LOGICFUNC, "and|or|xor|xornot");
    register_input(input::IN_TRIG2);
    register_param(param::PRECISION);
}

ui::moditem_list* logictrigger::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
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
            precision = *(wcint_t*)data;
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

errors::TYPE logictrigger::validate()
{
    if (!validate_param(param::PRECISION, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;
    return errors::NO_ERROR;
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
