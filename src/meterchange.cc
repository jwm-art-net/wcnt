#ifndef METERCHANGE_H
#include "../include/meterchange.h"

meterchange::meterchange() :
 dobj(dobjnames::SIN_METER),
 bar(0)
{
    time_sig.beatsperbar = 0;
    time_sig.beatvalue = 0;
#ifndef BARE_MODULES
    create_dparams();
#endif
}

meterchange::meterchange(short br, char btpb, char btval) :
        dobj(dobjnames::SIN_METER), bar(br)
{
    time_sig.beatsperbar = btpb;
    time_sig.beatvalue = btval;
#ifndef BARE_MODULES
    create_dparams();
#endif
}

#ifndef BARE_MODULES

bool meterchange::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_METER:
        set_beatsperbar(((timesig*)data)->beatsperbar);
        set_beatvalue(((timesig*)data)->beatvalue);
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

void* meterchange::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_METER:
        retv = &time_sig;
        break;
    case dparamnames::DPAR_BAR:
        retv = &bar;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE meterchange::validate()
{
    if (time_sig.beatsperbar <= 2 || time_sig.beatsperbar > 32) {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (time_sig.beatvalue <= 2 || time_sig.beatvalue > 32) {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (!get_dparlist()->validate(
        this, dparamnames::DPAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool meterchange::done_dparams = false;

void meterchange::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_METER, dparamnames::DPAR_METER);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_METER, dparamnames::DPAR_BAR);
    done_dparams = true;
}

#endif // BARE_MODULES
#endif
