#include "../include/modnamedobj.h"

modnamedobj::modnamedobj() :
 dobj::base(dobj::SIN_MODNAME),
 modname(0)
{
}

void modnamedobj::register_ui()
{
    ui::dobjitem* i = register_param(param::NAME);
    if (i)
        i->add_comment("Name of module.");
}

ui::dobjitem_list* modnamedobj::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
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

bool modnamedobj::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
        case param::NAME:
            set_modname((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* modnamedobj::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::NAME: return modname;
        default: return 0;
    }
}



