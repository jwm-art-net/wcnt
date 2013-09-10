#ifndef DYNVERTEX_H
#include "../include/dynvertex.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


dynvertex::dynvertex() :
 dobj(dobjnames::SIN_DVERTEX),
 si_level(0), uso_level(0), lso_level(0)
{
    create_params();
}

dynvertex::dynvertex(double sil, double usol, double lsol) :
        dobj(dobjnames::SIN_DVERTEX),
        si_level(sil), uso_level(usol), lso_level(lsol)
{
    create_params();
}

stockerrs::ERR_TYPE dynvertex::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

bool dynvertex::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    {
        case paramnames::SIGIN_LEVEL:
            si_level = *(double*)data;
            return true;
        case paramnames::SIGOUT_UPLEVEL:
            uso_level = *(double*)data;
            return true;
        case paramnames::SIGOUT_LOLEVEL:
            lso_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* dynvertex::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::SIGIN_LEVEL:       return &si_level;
        case paramnames::SIGOUT_UPLEVEL:    return &uso_level;
        case paramnames::SIGOUT_LOLEVEL:    return &lso_level;
        default: return 0;
    }
}

void dynvertex::create_params()
{
    if (done_params == true) return;
    jwm.get_dparlist().add_dobjparam(
        dobjnames::SIN_DVERTEX, paramnames::SIGIN_LEVEL);
    jwm.get_dparlist().add_dobjparam(
        dobjnames::SIN_DVERTEX, paramnames::SIGOUT_UPLEVEL);
    jwm.get_dparlist().add_dobjparam(
        dobjnames::SIN_DVERTEX, paramnames::SIGOUT_LOLEVEL);
    done_params = true;
}

bool dynvertex::done_params = false;
#endif
