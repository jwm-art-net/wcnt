#ifndef DOBJMOD_H
#include "../include/dobjmod.h"

dobjmod::dobjmod() :
 dobj(dobjnames::DOBJ_SYNTHMOD), synth_mod(0)
{
    create_params();
}

bool dobjmod::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MODNAME:
        set_synthmod((synthmod*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* dobjmod::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_MODNAME:
        retv = 0;   // not sure how to handle this yet and besides,
        break;      // there is currently no reason to use it.
    default:
        retv = 0;
        break;
    }
    return retv;
}

bool dobjmod::done_params = false;

void dobjmod::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::DOBJ_SYNTHMOD, paramnames::PAR_MODNAME);
    done_params = true;
}

#endif
