#ifndef DOBJMOD_H
#include "../include/dobjmod.h"

#ifndef BARE_MODULES

dobjmod::dobjmod() :
 dobj(dobjnames::DOBJ_SYNTHMOD), synth_mod(0)
{
    create_dparams();
}

bool dobjmod::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_MODNAME:
        set_synthmod((synthmod*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* dobjmod::get_dparam(dparamnames::DPAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case dparamnames::DPAR_MODNAME:
        retv = 0;	// not sure how to handle this yet and besides,
        break;		// there is currently no reason to use it.
    default:
        retv = 0;
        break;
    }
    return retv;
}

bool dobjmod::done_dparams = false;

void dobjmod::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::DOBJ_SYNTHMOD, dparamnames::DPAR_MODNAME);
    done_dparams = true;
}

#endif
#endif
