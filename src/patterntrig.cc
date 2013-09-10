#ifndef PATTERNTRIG_H
#include "../include/patterntrig.h"

patterntrig::patterntrig(char const* uname) :
 synthmod(synthmodnames::MOD_PATTERNTRIG, patterntrig_count, uname),
 in_trig(0), out_trig(OFF), out_start_trig(OFF), 
 out_end_trig(OFF), pattern(0), ptr(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_START_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_END_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    patterntrig_count++;
    create_params();
}

patterntrig::~patterntrig()
{
    if (pattern) delete [] pattern;
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
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
    if (!pattern) {
        *err_msg = "\nNo pattern - oops.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (strlen(pattern) < 2) {
        *err_msg = "\nNon pattern - oops.";
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
                *err_msg += "\ninvalid character ";
                *err_msg += *ptr;
                *err_msg += " in pattern.";
                invalidate();
        }
        if (bc > 1) {
            *err_msg += "\nError, adjacent seperators in pattern.";
            invalidate();
        }
        ptr++;
    }
    ptr = 0;
    if (bc > 0) {
        *err_msg += 
         "\nError, cannot have seperator as last symbol in pattern.";
        invalidate();
    }
    return (is_valid()) ? stockerrs::ERR_NO_ERROR : stockerrs::ERR_ERROR;
}

void const* patterntrig::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    case outputnames::OUT_START_TRIG:
        o = &out_start_trig;
        break;
    case outputnames::OUT_END_TRIG:
        o = &out_end_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* patterntrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool patterntrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_TRIG_STRING:
        set_pattern_string((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* patterntrig::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_TRIG_STRING:
        return pattern;
    default:
        return 0;
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

int patterntrig::patterntrig_count = 0;

bool patterntrig::done_params = false;

void patterntrig::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_PATTERNTRIG, paramnames::PAR_TRIG_STRING);
    done_params = true;
}

#endif
