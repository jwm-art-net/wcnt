#ifndef NONEZERO_H
#include "../include/nonezero.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"

nonezero::nonezero(char const* uname) :
 synthmod(synthmodnames::NONEZERO, uname),
 out_none_double(0.00), out_none_short(0), out_none_ulong(0), 
 out_none_STATUS(OFF)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_NONE_DOUBLE);
    jwm.get_outputlist().add_output(this, outputnames::OUT_NONE_SHORT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_NONE_ULONG);
    jwm.get_outputlist().add_output(this, outputnames::OUT_NONE_TRIG);
    jwm.get_outputlist().add_output(this, outputnames::OUT_NONE_STATE);
}

nonezero::~nonezero()
{
    jwm.get_outputlist().delete_module_outputs(this);
}

synthmod* nonezero::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg = "\nYou are trying to duplicate 'off', ";
    *err_msg += "you can put a stop to that right now.";
    return 0;
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
        default: return 0;
    }
}

#endif
