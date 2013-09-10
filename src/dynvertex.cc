#ifndef DYNVERTEX_H
#include "../include/dynvertex.h"

dynvertex::dynvertex() :
 dobj(dobjnames::SIN_DVERTEX),
 si_level(0),	uso_level(0), lso_level(0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

dynvertex::dynvertex(double sil, double usol, double lsol) :
        dobj(dobjnames::SIN_DVERTEX),
        si_level(sil), uso_level(usol), lso_level(lsol)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

stockerrs::ERR_TYPE dynvertex::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

#ifndef BARE_MODULES

bool dynvertex::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_SIGIN_LEVEL:
        set_signal_in_level(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_SIGOUT_UPLEVEL:
        set_upper_signal_out_level(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_SIGOUT_LOLEVEL:
        set_lower_signal_out_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* dynvertex::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_SIGIN_LEVEL:
        retv = new double(get_signal_in_level());
        break;
    case dparamnames::DPAR_SIGOUT_UPLEVEL:
        retv = new double(get_upper_signal_out_level());
        break;
    case dparamnames::DPAR_SIGOUT_LOLEVEL:
        retv = new double(get_lower_signal_out_level());
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

void dynvertex::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, dparamnames::DPAR_SIGIN_LEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, dparamnames::DPAR_SIGOUT_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DVERTEX, dparamnames::DPAR_SIGOUT_LOLEVEL);
    done_dparams = true;
}

bool dynvertex::done_dparams = false;
#endif
#endif
