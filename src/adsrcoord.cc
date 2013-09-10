#ifndef ADSRCOORD_H
#include "../include/adsrcoord.h"

adsr_coord::adsr_coord() :
        dobj(dobjnames::SIN_COORD), sect(ADSR_ATTACK),
        upper_time(0), upper_level(0), lower_time(0), lower_level(0)
{
    create_params();
}

adsr_coord::adsr_coord
 (adsr_coord::SECT s, double ut, double ul, double lt, double ll) :
 dobj(dobjnames::SIN_COORD), sect(s), 
 upper_time(ut), upper_level(ul),
 lower_time(lt), lower_level(ll)
{
    create_params();
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
#include <iostream>
using namespace std;
bool adsr_coord::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
    case paramnames::PAR_ADSRSECT:
        set_adsr_section((SECT)(*(int*)data));
        retv = true;
        break;
    case paramnames::PAR_UPTIME:
        set_upper_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_UPLEVEL:
        set_upper_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_LOTIME:
        set_lower_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_LOLEVEL:
        set_lower_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* adsr_coord::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_ADSRSECT:
        return &sect;
    case paramnames::PAR_UPTIME:
        return &upper_time;
    case paramnames::PAR_UPLEVEL:
        return &upper_level;
    case paramnames::PAR_LOTIME:
        return &lower_time;
    case paramnames::PAR_LOLEVEL:
        return &lower_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE adsr_coord::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, paramnames::PAR_UPTIME, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_UPTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, paramnames::PAR_LOTIME, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_LOTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void adsr_coord::create_params()
{
    if (adsr_coord::done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, paramnames::PAR_ADSRSECT);
    get_fxsparamlist()->add_param("attack/decay/sustain/release",
     paramnames::PAR_ADSRSECT);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, paramnames::PAR_UPTIME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, paramnames::PAR_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, paramnames::PAR_LOTIME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, paramnames::PAR_LOLEVEL);
    done_params = true;
}

bool adsr_coord::done_params = false;
#endif
