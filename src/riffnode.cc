#include "../include/riffnode.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjparamlist.h"


riff_node::riff_node() :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(0), riff_source(0),
 transpose(0), repeat(0), repeat_stripe(0)
{
    init_first();
}

riff_node::riff_node(riffdata* rd, short barpos) :
 dobj(dobjnames::SIN_RIFFNODE), start_bar(barpos), riff_source(rd),
 transpose(0), repeat(0), repeat_stripe(0)
{
    init_first();
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
    switch(pt)
    {
        case paramnames::RIFFNAME:
            set_riff_source((riffdata*)data);
            return true;
        case paramnames::BAR:
            set_start_bar(*(short*)data);
            return true;
        case paramnames::TRANSPOSE:
            set_transpose(*(short*)data);
            return true;
        case paramnames::REPEAT:
            set_repeat(*(short*)data);
            return true;
        case paramnames::REPEAT_STRIPE:
            set_repeat_stripe(*(short*)data);
            return true;
        default:
            return false;
    }
}

void const* riff_node::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RIFFNAME:      return riff_source;
        case paramnames::BAR:           return &start_bar;
        case paramnames::TRANSPOSE:     return &transpose;
        case paramnames::REPEAT:        return &repeat;
        case paramnames::REPEAT_STRIPE: return &repeat_stripe;
        default: return 0;
    }
}

stockerrs::ERR_TYPE riff_node::validate()
{
    if (((dobj*)riff_source)->get_object_type() != dobjnames::DEF_RIFF) 
    {
        dobjerr("%s is not a riff and cannot be used as one.",
                                    riff_source->get_username());
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_dparlist()->validate(this,
        paramnames::BAR, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_dparlist()->validate(this,
        paramnames::REPEAT, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::REPEAT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_dparlist()->validate(this,
        paramnames::REPEAT_STRIPE, stockerrs::ERR_NEG_ZERO))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(
                                            paramnames::REPEAT_STRIPE));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void riff_node::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::RIFFNAME);
    register_param(paramnames::BAR);
    register_param(paramnames::TRANSPOSE);
    register_param(paramnames::REPEAT);
    register_param(paramnames::REPEAT_STRIPE);
}

