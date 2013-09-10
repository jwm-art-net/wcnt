#ifndef DOBJNAMEDOBJ_H
#include "../include/dobjnamedobj.h"

dobjnamedobj::dobjnamedobj() :
 dobj(dobjnames::SIN_DOBJNAME),
 dobjname(0)
{
    create_params();
}

dobjnamedobj::~dobjnamedobj()
{
    if (dobjname)
        delete [] dobjname;
}

void dobjnamedobj::set_dobjname(const char* name)
{
    if (dobjname)
        delete [] dobjname;
    dobjname = new char[strlen(name) + 1];
    strcpy(dobjname, name);
    return;
}

bool dobjnamedobj::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        set_dobjname((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* dobjnamedobj::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        retv = dobjname;
        break;
    default:
        retv = 0;
    }
    return retv;
}

void dobjnamedobj::create_params()
{
    if (done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_DOBJNAME, paramnames::PAR_NAME);
    done_params = true;
}

bool dobjnamedobj::done_params = false;

#endif
