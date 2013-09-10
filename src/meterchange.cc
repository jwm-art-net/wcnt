#ifndef METERCHANGE_H
#include "../include/meterchange.h"

meterchange::meterchange() :
 dobj(dobjnames::SIN_METER),
 bar(0)
{
    time_sig.beatsperbar = 0;
    time_sig.beatvalue = 0;
    create_params();
}

meterchange::meterchange(short br, char btpb, char btval) :
        dobj(dobjnames::SIN_METER), bar(br)
{
    time_sig.beatsperbar = btpb;
    time_sig.beatvalue = btval;
    create_params();
}

bool meterchange::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_METER:
        set_beatsperbar(((timesig*)data)->beatsperbar);
        set_beatvalue(((timesig*)data)->beatvalue);
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

void const* meterchange::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_METER:
        retv = &time_sig;
        break;
    case paramnames::PAR_BAR:
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
        *err_msg = get_paramnames()->get_name(paramnames::PAR_METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (time_sig.beatvalue <= 2 || time_sig.beatvalue > 32) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (!get_dparlist()->validate(
        this, paramnames::PAR_BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool meterchange::done_params = false;

void meterchange::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_METER, paramnames::PAR_METER);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_METER, paramnames::PAR_BAR);
    done_params = true;
}

#endif
