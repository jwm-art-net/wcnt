#include "../include/patterntrig.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

patterntrig::patterntrig(const char* uname) :
 synthmod(module::PATTERNTRIG, uname, SM_HAS_OUT_TRIG),
 in_trig(0), out_trig(OFF), out_start_trig(OFF),
 out_end_trig(OFF), out_index(-1), pattern(0), ptr(0)
{
    register_input(input::IN_TRIG);
    register_output(output::OUT_TRIG);
    register_output(output::OUT_START_TRIG);
    register_output(output::OUT_END_TRIG);
    register_output(output::OUT_INDEX);
    init_first();
}

patterntrig::~patterntrig()
{
    if (pattern) delete [] pattern;
}

void patterntrig::set_pattern_string(const char* pat)
{
    if (!pat) return;
    if (pattern) delete [] pattern;
    pattern = new char[strlen(pat) + 1];
    strcpy(pattern, pat);
}

errors::TYPE patterntrig::validate()
{
    if (!pattern || strlen(pattern) < 1) {
        sm_err("%s", "No pattern - oops.");
        invalidate();
        return errors::ERROR;
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
            ? errors::NO_ERROR
            : errors::ERROR;
}

const void* patterntrig::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:         return &out_trig;
        case output::OUT_START_TRIG:   return &out_start_trig;
        case output::OUT_END_TRIG:     return &out_end_trig;
        case output::OUT_INDEX:        return &out_index;
        default: return 0;
    }
}

const void* patterntrig::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* patterntrig::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool patterntrig::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::TRIG_STRING:
            set_pattern_string((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* patterntrig::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::TRIG_STRING: return pattern;
        default: return 0;
    }
}

void patterntrig::run()
{
    if (*in_trig == ON) {
        if (!ptr) {
            out_index = -1;
            ptr = pattern;
            out_start_trig = ON;
        }
        if (*ptr == '-')
            ptr++;
        if (*ptr == '1') {
            out_trig = ON;
            ++out_index;
        }
        ptr++;
        if (*ptr == '\0') {
            out_end_trig = ON;
            ptr = 0;
        }
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        if (out_start_trig == ON)
            out_start_trig = OFF;
        if (out_end_trig == ON) {
            out_end_trig = OFF;
            out_index = -1;
        }
    }
}



void patterntrig::init_first()
{
    if (done_first())
        return;
    register_param(param::TRIG_STRING);
}

