#include "../include/nonezero.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"

nonezero::nonezero(char const* uname) :

 synthmod(
    synthmodnames::NONEZERO,
    uname,
    SM_EMPTY_RUN | SM_UNGROUPABLE | SM_UNDUPLICABLE),

 out_none_double(0.00), out_none_short(0), out_none_ulong(0),
 out_none_STATUS(OFF), out_none_string(0)
{
    out_none_string = new char[1];
    *out_none_string = '\0';
    register_output(outputnames::OUT_NONE_DOUBLE);
    register_output(outputnames::OUT_NONE_SHORT);
    register_output(outputnames::OUT_NONE_ULONG);
    register_output(outputnames::OUT_NONE_TRIG);
    register_output(outputnames::OUT_NONE_STATE);
    register_output(outputnames::OUT_NONE_STRING);
}

nonezero::~nonezero()
{
    if (out_none_string)
        delete [] out_none_string;
}

void const* nonezero::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_NONE_DOUBLE:  return &out_none_double;
        case outputnames::OUT_NONE_SHORT:   return &out_none_short;
        case outputnames::OUT_NONE_ULONG:   return &out_none_ulong;
        case outputnames::OUT_NONE_TRIG:    return &out_none_STATUS;
        case outputnames::OUT_NONE_STATE:   return &out_none_STATUS;
        case outputnames::OUT_NONE_STRING:  return &out_none_string;
        default: return 0;
    }
}

