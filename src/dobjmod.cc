#include "../include/dobjmod.h"

dobjmod::dobjmod() :
 dobj::base(dobj::DOBJ_SYNTHMOD), synth_mod(0)
{
}

void dobjmod::register_ui()
{
    ui::dobjitem* i = register_param(param::MODNAME);
    if (i)
        i->add_comment("A module to be contained by group.");
}

ui::dobjitem_list* dobjmod::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool dobjmod::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
     case param::MODNAME:
        set_synthmod((synthmod::base*)data);
        return true;
     default:
        return false;
    }
}

const void* dobjmod::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::MODNAME: // aparently, no reason to use this.
        default: return 0;
    }
}

