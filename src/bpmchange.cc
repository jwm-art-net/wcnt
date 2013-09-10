#ifndef BPMCHANGE_H
#include "../include/bpmchange.h"

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
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_BPM:
        set_bpm(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_BAR:
        set_bar(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* bpmchange::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_BPM:
        retv = &tobpm;
        break;
    case paramnames::PAR_BAR:
        retv = &atbar;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE bpmchange::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, paramnames::PAR_BPM, stockerrs::ERR_RANGE_BPM))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_BPM);
        invalidate();
        return stockerrs::ERR_RANGE_BPM;
    }
    if (!dpl->validate(
        this, paramnames::PAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool bpmchange::done_params = false;

void bpmchange::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_BPM, paramnames::PAR_BPM);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_BPM, paramnames::PAR_BAR);
    done_params = true;
}

#endif
