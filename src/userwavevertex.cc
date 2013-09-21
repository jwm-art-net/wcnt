#include "../include/userwavevertex.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

wave_vertex::wave_vertex() :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(0), up_pos(0), lo_deg(0), lo_pos(0)
{
    init_first();
}

wave_vertex::wave_vertex(
    double udeg, double upos, double ldeg, double lpos) :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(udeg), up_pos(upos), lo_deg(ldeg), lo_pos(lpos)
{
    init_first();
}

void wave_vertex::modulate(double vmod, double hmod)
{
    out_deg = lo_deg + (up_deg - lo_deg) * (( hmod < 0) ? -hmod : hmod);
    out_pos = lo_pos + (up_pos - lo_pos) * (( vmod < 0) ? -vmod : vmod);
}

bool wave_vertex::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    {
    case paramnames::UPDEG:
        set_updeg(*(double*)data);
        return true;
    case paramnames::UPLEVEL:
        set_uppos(*(double*)data);
        return true;
    case paramnames::LODEG:
        set_lodeg(*(double*)data);
        return true;
    case paramnames::LOLEVEL:
        set_lopos(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* wave_vertex::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
    case paramnames::UPDEG:
        return &up_deg;
    case paramnames::UPLEVEL:
        return &up_pos;
    case paramnames::LODEG:
        return &lo_deg;
    case paramnames::LOLEVEL:
        return &lo_pos;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE wave_vertex::validate()
{
    if (!jwm.get_dparlist()->validate(this,
        paramnames::UPDEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = paramnames::get_name(paramnames::UPDEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    if (!jwm.get_dparlist()->validate(this,
        paramnames::LODEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = paramnames::get_name(paramnames::LODEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wave_vertex::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::UPDEG);
    register_param(paramnames::UPLEVEL);
    register_param(paramnames::LODEG);
    register_param(paramnames::LOLEVEL);
}


