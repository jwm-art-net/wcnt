#include "../include/meterchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


meterchange::meterchange() :
 dobj(dobjnames::SIN_METER),
 bar(0)
{
    time_sig.beatsperbar = 0;
    time_sig.beatvalue = 0;
    init_first();
}

meterchange::meterchange(short br, char btpb, char btval) :
        dobj(dobjnames::SIN_METER), bar(br)
{
    time_sig.beatsperbar = btpb;
    time_sig.beatvalue = btval;
    init_first();
}

bool meterchange::set_param(paramnames::PAR_TYPE pt, const void* data)
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

const void* meterchange::get_param(paramnames::PAR_TYPE pt) const
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
        dobjerr("%s", paramnames::get_name(paramnames::METER));
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (time_sig.beatvalue <= 2 || time_sig.beatvalue > 32) {
        dobjerr("%s", paramnames::get_name(paramnames::METER));
        invalidate();
        return stockerrs::ERR_RANGE_BEAT;
    }
    if (!jwm.get_dparlist()->validate(
        this, paramnames::BAR, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", paramnames::get_name(paramnames::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}



void meterchange::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::METER);
    register_param(paramnames::BAR);
}

