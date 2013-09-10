#ifndef NONEZERO_H
#include "../include/nonezero.h"

nonezero::nonezero(char const* uname) :
 synthmod(synthmodnames::MOD_NONEZERO, 0, uname),
 out_none_double(0.00), out_none_short(0), out_none_ulong(0), 
 out_none_STATUS(OFF)
{
    get_outputlist()->add_output(this, outputnames::OUT_NONE_DOUBLE);
    get_outputlist()->add_output(this, outputnames::OUT_NONE_SHORT);
    get_outputlist()->add_output(this, outputnames::OUT_NONE_ULONG);
    get_outputlist()->add_output(this, outputnames::OUT_NONE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NONE_STATE);
}

nonezero::~nonezero()
{
    get_outputlist()->delete_module_outputs(this);
}

synthmod* nonezero::duplicate_module(const char* uname, DUP_IO dupio)
{
    *err_msg = "\nYou are trying to duplicate 'off', ";
    *err_msg += "you can put a stop to that right now.";
    return 0;
}


void const* nonezero::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_NONE_DOUBLE:
        o = &out_none_double;
        break;
    case outputnames::OUT_NONE_SHORT:
        o = &out_none_short;
        break;
    case outputnames::OUT_NONE_ULONG:
        o = &out_none_ulong;
        break;
    case outputnames::OUT_NONE_TRIG:
        o = &out_none_STATUS;
        break;
    case outputnames::OUT_NONE_STATE:
        o = &out_none_STATUS;
        break;
    default:
        o = 0;
    }
    return o;
}

#endif
