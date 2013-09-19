#include "../include/dynvertex.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


dynvertex::dynvertex() :
 dobj(dobjnames::SIN_DVERTEX),
 si_level(0), uso_level(0), lso_level(0)
{
    init_first();
}

dynvertex::dynvertex(double sil, double usol, double lsol) :
        dobj(dobjnames::SIN_DVERTEX),
        si_level(sil), uso_level(usol), lso_level(lsol)
{
    init_first();
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

void dynvertex::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::SIGIN_LEVEL);
    register_param(paramnames::SIGOUT_UPLEVEL);
    register_param(paramnames::SIGOUT_LOLEVEL);
}


