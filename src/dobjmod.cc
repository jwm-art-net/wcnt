#include "../include/dobjmod.h"

dobjmod::dobjmod() :
 dobj::base(dobj::DOBJ_SYNTHMOD), synth_mod(0)
{
}

void dobjmod::register_ui()
{
    register_param(param::MODNAME)
            ->add_descr("Name of module.");
}

ui::dobjitem_list* dobjmod::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool dobjmod::set_param(int pt, const void* data)
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

const void* dobjmod::get_param(int pt) const
{
    switch(pt)
    {
        case param::MODNAME:
            return synth_mod; // aparently, there _was_ no reason to use this??
        default: return 0;
    }
}
