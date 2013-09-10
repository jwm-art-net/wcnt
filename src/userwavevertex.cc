#ifndef USERWAVEVERTEX_H
#include "../include/userwavevertex.h"

wave_vertex::wave_vertex() :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(0), up_pos(0), lo_deg(0), lo_pos(0)
{
    create_params();
}

wave_vertex::wave_vertex(
    double udeg, double upos, double ldeg, double lpos) :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(udeg), up_pos(upos), lo_deg(ldeg), lo_pos(lpos)
{
    create_params();
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
    case paramnames::PAR_UPDEG:
        set_updeg(*(double*)data);
        return true;
    case paramnames::PAR_UPLEVEL:
        set_uppos(*(double*)data);
        return true;
    case paramnames::PAR_LODEG:
        set_lodeg(*(double*)data);
        return true;
    case paramnames::PAR_LOLEVEL:
        set_lopos(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* wave_vertex::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_UPDEG:
        return &up_deg;
    case paramnames::PAR_UPLEVEL:
        return &up_pos;
    case paramnames::PAR_LODEG:
        return &lo_deg;
    case paramnames::PAR_LOLEVEL:
        return &lo_pos;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE wave_vertex::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, paramnames::PAR_UPDEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_UPDEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    if (!dpl->validate(
        this, paramnames::PAR_LODEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_LODEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wave_vertex::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, paramnames::PAR_UPDEG);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, paramnames::PAR_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, paramnames::PAR_LODEG);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, paramnames::PAR_LOLEVEL);
    done_params = true;
}

bool wave_vertex::done_params = false;

#endif
