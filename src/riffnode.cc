#ifndef RIFFNODE_H
#include "../include/riffnode.h"

riff_node::riff_node() :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(0), riff_source(0),
 transpose(0), startlen(0), endlen(0)

{
    create_params();
}

riff_node::riff_node(riffdata* rd, short barpos) :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(barpos), riff_source(rd)
{
    create_params();
}

riff_node::~riff_node()
{
}

bool riff_node::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_RIFFNAME:
        set_riff_source((riffdata*)data);// pass pointer
        retv = true;
        break;
    case paramnames::PAR_BAR:
        set_start_bar(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_TRANSPOSE:
        set_transpose(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_STARTLEN:
        set_start_length(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_ENDLEN:
        set_end_length(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* riff_node::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_RIFFNAME:
        retv = riff_source;
        break;
    case paramnames::PAR_BAR:
        retv = &start_bar;
        break;
    case paramnames::PAR_TRANSPOSE:
        retv = &transpose;
        break;
    case paramnames::PAR_STARTLEN:
        retv = &startlen;
        break;
    case paramnames::PAR_ENDLEN:
        retv = &endlen;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE riff_node::validate()
{
    if (!get_dparlist()->validate(
        this, paramnames::PAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool riff_node::done_params = false;

void riff_node::create_params()
{
    if (done_params == true) return;
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_RIFFNAME);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_BAR);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_TRANSPOSE);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_STARTLEN);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_ENDLEN);
    done_params = true;
}

#endif
