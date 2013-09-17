#include "../include/meterchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

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
    switch(pt)
    {
        case paramnames::METER:
            set_beatsperbar(((timesig*)data)->beatsperbar);
            set_beatvalue(((timesig*)data)->beatvalue);
            return true;
        case paramnames::BAR:
            bar = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* meterchange::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::METER: return &time_sig;
        case paramnames::BAR:   return &bar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE meterchange::validate()
{
    if (time_sig.beatsperbar <= 2 || time_sig.beatsperbar > 32) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (time_sig.beatvalue <= 2 || time_sig.beatvalue > 32) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::METER);
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (!jwm.get_dparlist()->validate(
        this, paramnames::BAR, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}



void meterchange::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::METER);
    relate_param(paramnames::BAR);
}

