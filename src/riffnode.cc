#ifndef RIFFNODE_H
#include "../include/riffnode.h"

riff_node::riff_node() :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(0), riff_source(0),
 transpose(0), repeat(0), repeat_stripe(0)
{
    create_params();
}

riff_node::riff_node(riffdata* rd, short barpos) :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(barpos), riff_source(rd),
 transpose(0), repeat(0), repeat_stripe(0)
{
    create_params();
}

riff_node::~riff_node()
{
}

riff_node* riff_node::duplicate_for_bar(short barpos)
{
    // don't duplicate repeat settings...
    riff_node* newrn = new riff_node(riff_source, barpos);
    newrn->set_transpose(transpose);
    return newrn;
}

bool riff_node::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_RIFFNAME:
        set_riff_source((riffdata*)data);
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
    case paramnames::PAR_REPEAT:
        set_repeat(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_REPEAT_STRIPE:
        set_repeat_stripe(*(short*)data);
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
    case paramnames::PAR_REPEAT:
        retv = &repeat;
        break;
    case paramnames::PAR_REPEAT_STRIPE:
        retv = &repeat_stripe;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE riff_node::validate()
{
    if (((dobj*)riff_source)->get_object_type() != dobjnames::DEF_RIFF) 
    {
        *err_msg = "\n";
        *err_msg += riff_source->get_username();
        *err_msg += " is not a riff and cannot be used as one.";
        return stockerrs::ERR_ERROR;
    }
    if (!get_dparlist()->validate(
        this, paramnames::PAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!get_dparlist()->validate(
        this, paramnames::PAR_REPEAT, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_REPEAT);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!get_dparlist()->validate(
        this, paramnames::PAR_REPEAT_STRIPE, stockerrs::ERR_NEG_ZERO))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_REPEAT_STRIPE);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
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
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_REPEAT);
    get_dparlist()->
    add_dobjparam(dobjnames::SIN_RIFFNODE, paramnames::PAR_REPEAT_STRIPE);
    done_params = true;
}

#endif
