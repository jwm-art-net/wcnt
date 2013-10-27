#include "../include/dobjnamedobj.h"

dobjnamedobj::dobjnamedobj() :
 dobj::base(dobj::SIN_DOBJNAME),
 dobjname(0)
{
}

void dobjnamedobj::register_ui()
{
    register_param(param::NAME)->add_descr("Name of data object.");
}

ui::dobjitem_list* dobjnamedobj::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
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

bool dobjnamedobj::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
        case param::NAME:
            set_dobjname((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* dobjnamedobj::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::NAME: return dobjname;
        default: return 0;
    }
}



