#include "../include/nonezero.h"
#include "../include/globals.h"
#include "../include/modoutputlist.h"

nonezero::nonezero(const char* uname) :

 synthmod::base(synthmod::NONEZERO, uname,
    SM_EMPTY_RUN | SM_UNGROUPABLE | SM_UNDUPLICABLE),

 out_none_double(0.00), out_none_wcint_t(0), out_none_samp_t(0),
 out_none_STATUS(OFF), out_none_string(0)
{
    out_none_string = new char[1];
    *out_none_string = '\0';
    register_output(output::OUT_NONE_DOUBLE);
    register_output(output::OUT_NONE_WCINT_T);
    register_output(output::OUT_NONE_SAMP_T);
    register_output(output::OUT_NONE_TRIG);
    register_output(output::OUT_NONE_STATE);
    register_output(output::OUT_NONE_STRING);
}

nonezero::~nonezero()
{
    if (out_none_string)
        delete [] out_none_string;
}

const void* nonezero::get_out(int ot) const
{
    switch(ot)
    {
    case output::OUT_NONE_DOUBLE:  return &out_none_double;
    case output::OUT_NONE_WCINT_T: return &out_none_wcint_t;
    case output::OUT_NONE_SAMP_T:  return &out_none_samp_t;
    case output::OUT_NONE_TRIG:    return &out_none_STATUS;
    case output::OUT_NONE_STATE:   return &out_none_STATUS;
    case output::OUT_NONE_STRING:  return &out_none_string;
    default: return 0;
    }
}

