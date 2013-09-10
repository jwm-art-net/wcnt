#ifndef DYNVERTEX_H
#include "../include/dynvertex.h"

dynvertex::dynvertex() :
 dobj(dobjnames::SIN_DVERTEX),
 si_level(0),	uso_level(0), lso_level(0)
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
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_SIGIN_LEVEL:
        set_signal_in_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_SIGOUT_UPLEVEL:
        set_upper_signal_out_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_SIGOUT_LOLEVEL:
        set_lower_signal_out_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* dynvertex::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_SIGIN_LEVEL:
        retv = new double(get_signal_in_level());
        break;
    case paramnames::PAR_SIGOUT_UPLEVEL:
        retv = new double(get_upper_signal_out_level());
        break;
    case paramnames::PAR_SIGOUT_LOLEVEL:
        retv = new double(get_lower_signal_out_level());
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

void dynvertex::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, paramnames::PAR_SIGIN_LEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, paramnames::PAR_SIGOUT_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, paramnames::PAR_SIGOUT_LOLEVEL);
    done_params = true;
}

bool dynvertex::done_params = false;
#endif
