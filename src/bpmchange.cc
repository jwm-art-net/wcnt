#include "../include/bpmchange.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


bpmchange::bpmchange() :
 dobj(dataobj::SIN_BPM), atbar(0), tobpm(0)
{
    init_first();
}

bpmchange::bpmchange(short bar, double bpm) :
 dobj(dataobj::SIN_BPM),
 atbar(bar), tobpm(bpm)
{
    init_first();
}

bool bpmchange::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
    case param::BPM:    set_bpm(*(double*)data);    return true;
    case param::BAR:    set_bar(*(short*)data);     return true;
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

    if (!validate_param(param::BAR, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void bpmchange::init_first()
{
    if (done_first())
        return;
    register_param(param::BPM);
    register_param(param::BAR);
}

