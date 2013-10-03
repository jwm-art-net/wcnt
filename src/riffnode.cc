#include "../include/riffnode.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjparamlist.h"


riff_node::riff_node() :
 dobj(dataobj::SIN_RIFFNODE), start_bar(0), riff_source(0),
 transpose(0), repeat(0), repeat_stripe(0)
{
    init_first();
}

riff_node::riff_node(riffdata* rd, wcint_t barpos) :
 dobj(dataobj::SIN_RIFFNODE), start_bar(barpos), riff_source(rd),
 transpose(0), repeat(0), repeat_stripe(0)
{
    init_first();
}

riff_node::~riff_node()
{
}

riff_node* riff_node::duplicate_for_bar(wcint_t barpos)
{
    // don't duplicate repeat settings...
    riff_node* newrn = new riff_node(riff_source, barpos);
    newrn->set_transpose(transpose);
    return newrn;
}

bool riff_node::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::RIFFNAME:
            set_riff_source((riffdata*)data);
            return true;
        case param::BAR:
            set_start_bar(*(wcint_t*)data);
            return true;
        case param::TRANSPOSE:
            set_transpose(*(wcint_t*)data);
            return true;
        case param::REPEAT:
            set_repeat(*(wcint_t*)data);
            return true;
        case param::REPEAT_STRIPE:
            set_repeat_stripe(*(wcint_t*)data);
            return true;
        default:
            return false;
    }
}

const void* riff_node::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::RIFFNAME:      return riff_source;
        case param::BAR:           return &start_bar;
        case param::TRANSPOSE:     return &transpose;
        case param::REPEAT:        return &repeat;
        case param::REPEAT_STRIPE: return &repeat_stripe;
        default: return 0;
    }
}

errors::TYPE riff_node::validate()
{
    if (((dobj*)riff_source)->get_object_type() != dataobj::DEF_RIFF) 
    {
        dobjerr("%s is not a riff and cannot be used as one.",
                                    riff_source->get_username());
        return errors::ERROR;
    }

    if (!validate_param(param::BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::REPEAT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::REPEAT_STRIPE, errors::RANGE_COUNT1))
        return errors::RANGE_COUNT1;

    if (!validate_param(param::TRANSPOSE, errors::RANGE_SEMI2))
        return errors::RANGE_SEMI2;

    return errors::NO_ERROR;
}

void riff_node::init_first()
{
    if (done_first())
        return;
    register_param(param::RIFFNAME);
    register_param(param::BAR);
    register_param(param::TRANSPOSE);
    register_param(param::REPEAT);
    register_param(param::REPEAT_STRIPE);
}

