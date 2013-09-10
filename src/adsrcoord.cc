#ifndef ADSRCOORD_H
#include "../include/adsrcoord.h"

adsr_coord::adsr_coord() :
        dobj(dobjnames::SIN_COORD), sect(ADSR_FIRST),
        upper_time(0), upper_level(0), lower_time(0), lower_level(0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

adsr_coord::adsr_coord
 (adsr_coord::SECT s, double ut, double ul, double lt, double ll) :
 dobj(dobjnames::SIN_COORD), sect(s), 
 upper_time(ut), upper_level(ul),
 lower_time(lt), lower_level(ll)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
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

#ifndef BARE_MODULES

bool adsr_coord::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_ADSRSECT:
        set_adsr_section(*(SECT*)data);
        retv = true;
        break;
    case dparamnames::DPAR_UPTIME:
        set_upper_time(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_UPLEVEL:
        set_upper_level(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_LOTIME:
        set_lower_time(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_LOLEVEL:
        set_lower_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* adsr_coord::get_dparam(dparamnames::DPAR_TYPE pt)
{
    switch(pt)
    {
    case dparamnames::DPAR_ADSRSECT:
        return &sect;
    case dparamnames::DPAR_UPTIME:
        return &upper_time;
    case dparamnames::DPAR_UPLEVEL:
        return &upper_level;
    case dparamnames::DPAR_LOTIME:
        return &lower_time;
    case dparamnames::DPAR_LOLEVEL:
        return &lower_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE adsr_coord::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, dparamnames::DPAR_UPTIME, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_UPTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, dparamnames::DPAR_LOTIME, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_LOTIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void adsr_coord::create_dparams()
{
    if (adsr_coord::done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, dparamnames::DPAR_ADSRSECT);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, dparamnames::DPAR_UPTIME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, dparamnames::DPAR_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, dparamnames::DPAR_LOTIME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_COORD, dparamnames::DPAR_LOLEVEL);
    done_dparams = true;
}

bool adsr_coord::done_dparams = false;
#endif
#endif
