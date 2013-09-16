#include "../include/patterntrig.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

patterntrig::patterntrig(char const* uname) :

 synthmod(
    synthmodnames::PATTERNTRIG,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), out_trig(OFF), out_start_trig(OFF),
 out_end_trig(OFF), pattern(0), ptr(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_START_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_END_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    create_params();
}

patterntrig::~patterntrig()
{
    if (pattern) delete [] pattern;
}

void patterntrig::set_pattern_string(char* pat)
{
    if (!pat) return;
    if (pattern) delete [] pattern;
    pattern = new char[strlen(pat) + 1];
    strcpy(pattern, pat);
}

stockerrs::ERR_TYPE patterntrig::validate()
{
    if (!pattern || strlen(pattern) < 1) {
        sm_err("%s", "No pattern - oops.");
        invalidate();
        return stockerrs::ERR_ERROR;
    }

    int bc = 0;
    ptr = pattern;
    while (*ptr != '\0') {
        switch (*ptr) {
            case '0':
            case '1':
                bc = 0;
                break;
            case '-':
                bc++;
                break;
            default:
                sm_err("Invalid character %c in pattern.", *ptr);
                invalidate();
        }
        if (bc > 1) {
            sm_err("%s", "Error, adjacent seperators in pattern.");
            invalidate();
        }
        ptr++;
    }
    ptr = 0;
    if (bc > 0) {
        sm_err("%s", "Cannot have seperator as last symbol in pattern.");
        invalidate();
    }
    return
        (flag(SM_VALID))
            ? stockerrs::ERR_NO_ERROR
            : stockerrs::ERR_ERROR;
}

void const* patterntrig::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:         return &out_trig;
        case outputnames::OUT_START_TRIG:   return &out_start_trig;
        case outputnames::OUT_END_TRIG:     return &out_end_trig;
        default: return 0;
    }
}

void const* patterntrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* patterntrig::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool patterntrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::TRIG_STRING:
            set_pattern_string((char*)data);
            return true;
        default:
            return false;
    }
}

void const* patterntrig::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::TRIG_STRING: return pattern;
        default: return 0;
    }
}

void patterntrig::run()
{
    if (*in_trig == ON) {
        if (!ptr) {
            ptr = pattern;
            out_start_trig = ON;
        }
        if (*ptr == '-') ptr++;
        if (*ptr == '1') out_trig = ON;
        ptr++;
        if (*ptr == '\0') {
            out_end_trig = ON;
            ptr = 0;
        }
    }
    else {
        if (out_trig == ON) out_trig = OFF;
        if (out_start_trig == ON) out_start_trig = OFF;
        if (out_end_trig == ON) out_end_trig = OFF;
    }
}



void patterntrig::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::PATTERNTRIG, paramnames::TRIG_STRING);
    
}

