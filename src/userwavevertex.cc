#include "../include/userwavevertex.h"


wave_vertex::wave_vertex() :
 dobj::base(dobj::SIN_VERTEX), out_deg(0), out_lvl(0),
 up_deg(0), up_lvl(0), lo_deg(0), lo_lvl(0)
{
}

wave_vertex::wave_vertex(const wave_vertex* wv) :
 dobj::base(dobj::SIN_VERTEX), out_deg(0), out_lvl(0),
 up_deg(wv->up_deg), up_lvl(wv->up_lvl), lo_deg(wv->lo_deg), lo_lvl(wv->lo_lvl)
{
}


wave_vertex::wave_vertex(double ud, double ul, double ld, double ll) :
 dobj::base(dobj::SIN_VERTEX), out_deg(0), out_lvl(0),
 up_deg(ud), up_lvl(ul), lo_deg(ld), lo_lvl(ll)
{
}

void wave_vertex::register_ui()
{
    register_param(param::DEG)->set_flags(ui::UI_OPTION1);
    register_param(param::UPDEG)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::LODEG)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::LEVEL)->set_flags(ui::UI_OPTION1);
    register_param(param::UPLEVEL)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::LOLEVEL)->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
}

ui::dobjitem_list* wave_vertex::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

void wave_vertex::modulate(double vmod, double hmod)
{
    out_deg = lo_deg + (up_deg - lo_deg) * (( hmod < 0) ? -hmod : hmod);
    out_lvl = lo_lvl + (up_lvl - lo_lvl) * (( vmod < 0) ? -vmod : vmod);
}

bool wave_vertex::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::DEG:
        up_deg = lo_deg = *(double*)data;
        return true;
    case param::UPDEG:
        up_deg = *(double*)data;
        return true;
    case param::LODEG:
        lo_deg = *(double*)data;
        return true;
    case param::LEVEL:
        lo_lvl = up_lvl = *(double*)data;
        return true;
    case param::UPLEVEL:
        up_lvl = *(double*)data;
        return true;
    case param::LOLEVEL:
        lo_lvl = *(double*)data;
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
        return &up_lvl;
    case param::LODEG:
        return &lo_deg;
    case param::LOLEVEL:
        return &lo_lvl;
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

