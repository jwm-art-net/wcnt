#include "../include/dobjmod.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

dobjmod::dobjmod() :
 dobj(dobjnames::DOBJ_SYNTHMOD), synth_mod(0)
{
    create_params();
}

bool dobjmod::set_param(paramnames::PAR_TYPE pt, void* data)
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

void const* dobjmod::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MODNAME: // aparently, no reason to use this.
        default: return 0;
    }
}

void dobjmod::create_params()
{
    if (done_params())
        return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::DOBJ_SYNTHMOD, paramnames::MODNAME);
}
