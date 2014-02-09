#include "../include/ladspa_module.h"


ladspa_module::ladspa_module(const char* uname) :
 synthmod::base(synthmod::LADSPA, uname, SM_DEFAULT)
{
}

void ladspa_module::register_ui()
{
}

ui::moditem_list* ladspa_module::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

ladspa_module::~ladspa_module()
{
}

const void* ladspa_module::get_out(int ot) const
{
    return 0;
}

const void* ladspa_module::set_in(int it, const void* o)
{
    return 0;
}

const void* ladspa_module::get_in(int it) const
{
    return 0;
}


bool ladspa_module::set_param(int pt, const void* data)
{
    switch(pt) {
    default:
        return false;
    }
}

const void* ladspa_module::get_param(int pt) const
{
    return 0;
}

errors::TYPE ladspa_module::validate()
{
    return errors::NO_ERROR;
}

void ladspa_module::run()
{
}

