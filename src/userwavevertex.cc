#include "../include/userwavevertex.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


wave_vertex::wave_vertex() :
 dobj(dataobj::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(0), up_pos(0), lo_deg(0), lo_pos(0)
{
    init_first();
}

wave_vertex::wave_vertex(
    double udeg, double upos, double ldeg, double lpos) :
 dobj(dataobj::SIN_VERTEX), out_deg(0), out_pos(0),
 up_deg(udeg), up_pos(upos), lo_deg(ldeg), lo_pos(lpos)
{
    init_first();
}

void wave_vertex::modulate(double vmod, double hmod)
{
    out_deg = lo_deg + (up_deg - lo_deg) * (( hmod < 0) ? -hmod : hmod);
    out_pos = lo_pos + (up_pos - lo_pos) * (( vmod < 0) ? -vmod : vmod);
}

bool wave_vertex::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::UPDEG:
        set_updeg(*(double*)data);
        return true;
    case param::UPLEVEL:
        set_uppos(*(double*)data);
        return true;
    case param::LODEG:
        set_lodeg(*(double*)data);
        return true;
    case param::LOLEVEL:
        set_lopos(*(double*)data);
        return true;
    default:
        return false;
    }
}

const void* wave_vertex::get_param(param::TYPE pt) const
{
    switch(pt)
    {
    case param::UPDEG:
        return &up_deg;
    case param::UPLEVEL:
        return &up_pos;
    case param::LODEG:
        return &lo_deg;
    case param::LOLEVEL:
        return &lo_pos;
    default:
        return 0;
    }
}

errors::TYPE wave_vertex::validate()
{
    if (!validate_param(param::UPDEG, errors::RANGE_DEGS))
        return errors::RANGE_DEGS;

    if (!validate_param(param::LODEG, errors::RANGE_DEGS))
        return errors::RANGE_DEGS;

    return errors::NO_ERROR;
}

void wave_vertex::init_first()
{
    if (done_first())
        return;
    register_param(param::UPDEG);
    register_param(param::UPLEVEL);
    register_param(param::LODEG);
    register_param(param::LOLEVEL);
}


