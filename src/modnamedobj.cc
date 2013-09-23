#include "../include/modnamedobj.h"
#include "../include/dobjparamlist.h"

modnamedobj::modnamedobj() :
 dobj(dobjnames::SIN_MODNAME),
 modname(0)
{
    init_first();
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

bool modnamedobj::set_param(paramnames::PAR_TYPE dt, const void* data)
{
    switch(dt)
    {
        case paramnames::NAME:
            set_modname((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* modnamedobj::get_param(paramnames::PAR_TYPE dt) const
{
    switch(dt)
    {
        case paramnames::NAME: return modname;
        default: return 0;
    }
}

void modnamedobj::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::NAME);
}



