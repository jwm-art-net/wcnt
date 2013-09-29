#include "../include/adsrcoord.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"
#include "../include/fxsparamlist.h"


adsr_coord::adsr_coord() :
 dobj(dobjnames::SIN_COORD),
 output_time(0), output_level(0), sect(ADSR_ATTACK),
 upper_time(0), upper_level(0), lower_time(0), lower_level(0)
{
    init_first();
}

adsr_coord::adsr_coord
 (adsr_coord::SECT s, double ut, double ul, double lt, double ll) :
 dobj(dobjnames::SIN_COORD),
 output_time(0), output_level(0), sect(s),
 upper_time(ut), upper_level(ul),
 lower_time(lt), lower_level(ll)
{
    init_first();
}

adsr_coord::~adsr_coord()
{
}

void adsr_coord::run(double velocity)
{
    if (velocity < 0)
        velocity = -velocity;
    if (velocity > 1.0)
        velocity = 1.0;
    output_time = lower_time + (upper_time - lower_time) * velocity;
    output_level = lower_level + (upper_level - lower_level) * velocity;
}

bool adsr_coord::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
        case param::ADSRSECT:
            set_adsr_section((SECT)(*(int*)data));
            return true;
        case param::UPTIME:
            set_upper_time(*(double*)data);
            return true;
        case param::UPLEVEL:
            set_upper_level(*(double*)data);
            return true;
        case param::LOTIME:
            set_lower_time(*(double*)data);
            return true;
        case param::LOLEVEL:
            set_lower_level(*(double*)data);
            return true;
        default:
            return false;
    }
}

const void* adsr_coord::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::ADSRSECT:  return &sect;
        case param::UPTIME:    return &upper_time;
        case param::UPLEVEL:   return &upper_level;
        case param::LOTIME:    return &lower_time;
        case param::LOLEVEL:   return &lower_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE adsr_coord::validate()
{
    dobjparamlist* dpl = jwm.get_dparlist();
    if (!dpl->validate(
        this, param::UPTIME, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", param::names::get(param::UPTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, param::LOTIME, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", param::names::get(param::LOTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void adsr_coord::init_first()
{
    if (done_first())
        return;
    register_param(param::ADSRSECT, "attack/decay/sustain/release");
    register_param(param::UPTIME);
    register_param(param::UPLEVEL);
    register_param(param::LOTIME);
    register_param(param::LOLEVEL);
}

