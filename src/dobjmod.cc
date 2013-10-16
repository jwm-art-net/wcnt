#include "../include/dobjmod.h"

dobjmod::dobjmod() :
 dobj::base(dobj::DOBJ_SYNTHMOD), synth_mod(0)
{
}

void dobjmod::register_ui()
{
    register_param(param::MODNAME);
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

