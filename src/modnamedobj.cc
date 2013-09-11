#include "../include/modnamedobj.h"
#include "../include/dobjparamlist.h"

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
    switch(dt)
    {
        case paramnames::NAME:
            set_modname((char*)data);
            return true;
        default:
            return false;
    }
}

void const* modnamedobj::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::NAME: return modname;
        default: return 0;
    }
}

void modnamedobj::create_params()
{
    if (done_params == true)
        return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_MODNAME, paramnames::NAME);
    done_params = true;
}

bool modnamedobj::done_params = false;

