#include "../include/bpmchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


bpmchange::bpmchange() : 
 dobj(dobjnames::SIN_BPM), atbar(0), tobpm(0)
{
    create_params();
}

bpmchange::bpmchange(short bar, double bpm) :
 dobj(dobjnames::SIN_BPM),
 atbar(bar), tobpm(bpm)
{
    create_params();
}

bool bpmchange::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    {
    case paramnames::BPM:
        set_bpm(*(double*)data);
        return true;
    case paramnames::BAR:
        set_bar(*(short*)data);
        return true;
    default:
        return false;
    }
}

void const* bpmchange::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::BPM:   return &tobpm;
        case paramnames::BAR:   return &atbar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE bpmchange::validate()
{
    if (!jwm.get_dparlist()->validate(
        this, paramnames::BPM, stockerrs::ERR_RANGE_BPM))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::BPM));
        invalidate();
        return stockerrs::ERR_RANGE_BPM;
    }
    if (!jwm.get_dparlist()->validate(
        this, paramnames::BAR, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool bpmchange::done_params = false;

void bpmchange::create_params()
{
    if (done_params == true) return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_BPM, paramnames::BPM);
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_BPM, paramnames::BAR);
    done_params = true;
}

