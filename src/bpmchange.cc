#ifndef BPMCHANGE_H
#include "../include/bpmchange.h"

bpmchange::bpmchange() : 
 dobj(dobjnames::SIN_BPM), atbar(0), tobpm(0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

bpmchange::bpmchange(short bar, double bpm) :
 dobj(dobjnames::SIN_BPM),
 atbar(bar), tobpm(bpm)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

#ifndef BARE_MODULES

bool bpmchange::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_BPM:
        set_bpm(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_BAR:
        set_bar(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* bpmchange::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_BPM:
        retv = &tobpm;
        break;
    case dparamnames::DPAR_BAR:
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
        this, dparamnames::DPAR_BPM, stockerrs::ERR_RANGE_BPM))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_BPM);
        invalidate();
        return stockerrs::ERR_RANGE_BPM;
    }
    if (!dpl->validate(
        this, dparamnames::DPAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool bpmchange::done_dparams = false;

void bpmchange::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_BPM, dparamnames::DPAR_BPM);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_BPM, dparamnames::DPAR_BAR);
    done_dparams = true;
}

#endif // BARE_MODULES
#endif
