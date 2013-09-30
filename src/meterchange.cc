#include "../include/meterchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


meterchange::meterchange() :
 dobj(dataobj::SIN_METER),
 bar(0)
{
    time_sig.beatsperbar = 0;
    time_sig.beatvalue = 0;
    init_first();
}

meterchange::meterchange(short br, char btpb, char btval) :
        dobj(dataobj::SIN_METER), bar(br)
{
    time_sig.beatsperbar = btpb;
    time_sig.beatvalue = btval;
    init_first();
}

bool meterchange::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::METER:
            set_beatsperbar(((timesig*)data)->beatsperbar);
            set_beatvalue(((timesig*)data)->beatvalue);
            return true;
        case param::BAR:
            bar = *(short*)data;
            return true;
        default:
            return false;
    }
}

const void* meterchange::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::METER: return &time_sig;
        case param::BAR:   return &bar;
        default: return 0;
    }
}

errors::TYPE meterchange::validate()
{
    if (time_sig.beatsperbar <= 2 || time_sig.beatsperbar > 32) {
        dobjerr("%s", param::names::get(param::METER));
        invalidate();
        return errors::RANGE_BEAT;
    }
    if (time_sig.beatvalue <= 2 || time_sig.beatvalue > 32) {
        dobjerr("%s", param::names::get(param::METER));
        invalidate();
        return errors::RANGE_BEAT;
    }
    if (!validate_param(param::BAR, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}



void meterchange::init_first()
{
    if (done_first())
        return;
    register_param(param::METER);
    register_param(param::BAR);
}

