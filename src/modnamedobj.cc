#ifndef MODNAMEDOBJ_H
#include "../include/modnamedobj.h"

modnamedobj::modnamedobj() :
 dobj(dobjnames::SIN_MODNAME),
 modname(0)
{
    create_params();
}

modnamedobj::~modnamedobj()
{
    if (modname)
        delete [] modname;
}

void modnamedobj::set_modname(const char* name)
{
    if (modname)
        delete [] modname;
    modname = new char[strlen(name) + 1];
    strcpy(modname, name);
    return;
}

bool modnamedobj::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        set_modname((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* modnamedobj::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        retv = modname;
        break;
    default:
        retv = 0;
    }
    return retv;
}

void modnamedobj::create_params()
{
    if (done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_MODNAME, paramnames::PAR_NAME);
    done_params = true;
}

bool modnamedobj::done_params = false;

#endif
