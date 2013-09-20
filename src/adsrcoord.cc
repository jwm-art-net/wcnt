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

bool adsr_coord::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
        case paramnames::ADSRSECT:
            set_adsr_section((SECT)(*(int*)data));
            return true;
        case paramnames::UPTIME:
            set_upper_time(*(double*)data);
            return true;
        case paramnames::UPLEVEL:
            set_upper_level(*(double*)data);
            return true;
        case paramnames::LOTIME:
            set_lower_time(*(double*)data);
            return true;
        case paramnames::LOLEVEL:
            set_lower_level(*(double*)data);
            return true;
        default:
            return false;
    }
}

void const* adsr_coord::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::ADSRSECT:  return &sect;
        case paramnames::UPTIME:    return &upper_time;
        case paramnames::UPLEVEL:   return &upper_level;
        case paramnames::LOTIME:    return &lower_time;
        case paramnames::LOLEVEL:   return &lower_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE adsr_coord::validate()
{
    dobjparamlist* dpl = jwm.get_dparlist();
    if (!dpl->validate(
        this, paramnames::UPTIME, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::UPTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, paramnames::LOTIME, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::LOTIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void adsr_coord::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::ADSRSECT, "attack/decay/sustain/release");
    register_param(paramnames::UPTIME);
    register_param(paramnames::UPLEVEL);
    register_param(paramnames::LOTIME);
    register_param(paramnames::LOLEVEL);
}

