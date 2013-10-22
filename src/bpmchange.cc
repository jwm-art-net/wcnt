#include "../include/bpmchange.h"


bpmchange::bpmchange() :
 dobj::base(dobj::SIN_BPM), atbar(0), tobpm(0)
{
}

bpmchange::bpmchange(wcint_t bar, double bpm) :
 dobj::base(dobj::SIN_BPM),
 atbar(bar), tobpm(bpm)
{
}


void bpmchange::register_ui()
{
    register_param(param::BPM);
    register_param(param::BAR);
}

ui::dobjitem_list* bpmchange::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool bpmchange::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
    case param::BPM:    set_bpm(*(double*)data);    return true;
    case param::BAR:    set_bar(*(wcint_t*)data);     return true;
    default:
        return false;
    }
}

const void* bpmchange::get_param(param::TYPE pt) const
{
    switch(pt) {
    case param::BPM:   return &tobpm;
    case param::BAR:   return &atbar;
    default:
        return 0;
    }
}

errors::TYPE bpmchange::validate()
{
    if (!validate_param(param::BPM, errors::RANGE_BPM))
        return errors::RANGE_BPM;

    if (!validate_param(param::BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

