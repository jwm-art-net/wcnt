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

bool dynvertex::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::SIGIN_LEVEL:
            si_level = *(double*)data;
            return true;
        case param::SIGOUT_UPLEVEL:
            uso_level = *(double*)data;
            return true;
        case param::SIGOUT_LOLEVEL:
            lso_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* dynvertex::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::SIGIN_LEVEL:       return &si_level;
        case param::SIGOUT_UPLEVEL:    return &uso_level;
        case param::SIGOUT_LOLEVEL:    return &lso_level;
        default: return 0;
    }
}

void dynvertex::init_first()
{
    if (done_first())
        return;
    register_param(param::SIGIN_LEVEL);
    register_param(param::SIGOUT_UPLEVEL);
    register_param(param::SIGOUT_LOLEVEL);
}


