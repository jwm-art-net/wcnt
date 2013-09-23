#include "../include/dobjmod.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

dobjmod::dobjmod() :
 dobj(dobjnames::DOBJ_SYNTHMOD), synth_mod(0)
{
    init_first();
}

bool dobjmod::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt)
    {
    case paramnames::MODNAME:
        set_synthmod((synthmod*)data);
         return true;
   default:
        return false;
    }
}

const void* dobjmod::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MODNAME: // aparently, no reason to use this.
        default: return 0;
    }
}

void dobjmod::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::MODNAME);
}
