#include "../include/bpmchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


bpmchange::bpmchange() : 
 dobj(dobjnames::SIN_BPM), atbar(0), tobpm(0)
{
    init_first();
}

bpmchange::bpmchange(short bar, double bpm) :
 dobj(dobjnames::SIN_BPM),
 atbar(bar), tobpm(bpm)
{
    init_first();
}

bool bpmchange::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::BPM:
        set_bpm(*(double*)data);
        return true;
    case param::BAR:
        set_bar(*(short*)data);
        return true;
    default:
        return false;
    }
}

const void* bpmchange::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::BPM:   return &tobpm;
        case param::BAR:   return &atbar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE bpmchange::validate()
{
    if (!jwm.get_dparlist()->validate(
        this, param::BPM, stockerrs::ERR_RANGE_BPM))
    {
        dobjerr("%s", param::names::get(param::BPM));
        invalidate();
        return stockerrs::ERR_RANGE_BPM;
    }
    if (!jwm.get_dparlist()->validate(
        this, param::BAR, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", param::names::get(param::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void bpmchange::init_first()
{
    if (done_first())
        return;
    register_param(param::BPM);
    register_param(param::BAR);
}

