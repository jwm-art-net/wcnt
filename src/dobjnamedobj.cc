#include "../include/dobjnamedobj.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

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
    switch(dt)
    {
        case paramnames::NAME:
            set_dobjname((char*)data);
            return true;
        default:
            return false;
    }
}

void const* dobjnamedobj::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::NAME: return dobjname;
        default: return 0;
    }
}

void dobjnamedobj::create_params()
{
    if (done_params())
        return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_DOBJNAME, paramnames::NAME);
}



