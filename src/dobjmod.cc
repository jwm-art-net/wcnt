#include "../include/dobjmod.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

dobjmod::dobjmod() :
 dobj(dataobj::DOBJ_SYNTHMOD), synth_mod(0)
{
    init_first();
}

bool dobjmod::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::MODNAME:
        set_synthmod((synthmod*)data);
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

void dobjmod::init_first()
{
    if (done_first())
        return;
    register_param(param::MODNAME);
}
