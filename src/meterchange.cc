#include "../include/meterchange.h"
#include "../include/globals.h"


meterchange::meterchange() :
 dobj::base(dobj::SIN_METER),
 bar(0)
{
    time_sig.beatsperbar = 0;
    time_sig.beatvalue = 0;
}

meterchange::meterchange(wcint_t br, char btpb, char btval) :
        dobj::base(dobj::SIN_METER), bar(br)
{
    time_sig.beatsperbar = btpb;
    time_sig.beatvalue = btval;
}

void meterchange::register_ui()
{
    register_param(param::METER);
    register_param(param::BAR);
}

ui::dobjitem_list* meterchange::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool meterchange::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::METER:
            set_beatsperbar(((timesig*)data)->beatsperbar);
            set_beatvalue(((timesig*)data)->beatvalue);
            return true;
        case param::BAR:
            bar = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* meterchange::get_param(int pt) const
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
    if (!validate_param(param::BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

