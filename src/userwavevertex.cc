#ifndef USERWAVEVERTEX_H
#include "../include/userwavevertex.h"

wave_vertex::wave_vertex() :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(0), up_pos(0), lo_deg(0), lo_pos(0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

wave_vertex::wave_vertex(
    double udeg, double upos, double ldeg, double lpos) :
 dobj(dobjnames::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(udeg), up_pos(upos), lo_deg(ldeg), lo_pos(lpos)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

void wave_vertex::modulate(double vmod, double hmod)
{
    out_deg = lo_deg + (up_deg - lo_deg) * (( hmod < 0) ? -hmod : hmod);
    out_pos = lo_pos + (up_pos - lo_pos) * (( vmod < 0) ? -vmod : vmod);
}

#ifndef BARE_MODULES

bool wave_vertex::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_UPDEG:
        set_updeg(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_UPLEVEL:
        set_uppos(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_LODEG:
        set_lodeg(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_LOLEVEL:
        set_lopos(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* wave_vertex::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_UPDEG:
        retv = &up_deg;
        break;
    case dparamnames::DPAR_UPLEVEL:
        retv = &up_pos;
        break;
    case dparamnames::DPAR_LODEG:
        retv = &lo_deg;
        break;
    case dparamnames::DPAR_LOLEVEL:
        retv = &lo_pos;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE wave_vertex::validate()
{
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, dparamnames::DPAR_UPDEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_UPDEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    if (!dpl->validate(
        this, dparamnames::DPAR_LODEG, stockerrs::ERR_RANGE_DEGS))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_LODEG);
        invalidate();
        return stockerrs::ERR_RANGE_DEGS;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wave_vertex::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, dparamnames::DPAR_UPDEG);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, dparamnames::DPAR_UPLEVEL);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, dparamnames::DPAR_LODEG);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_VERTEX, dparamnames::DPAR_LOLEVEL);
    done_dparams = true;
}

bool wave_vertex::done_dparams = false;
#endif


#endif
