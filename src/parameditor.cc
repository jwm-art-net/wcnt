#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/globals.h"

#include <iostream>
#include <cstdio>

parameditor::parameditor() :
 dobj::base(dobj::DEF_PARAMEDITOR)
{
}

void parameditor::register_ui()
{
    register_dobj(dobj::LST_EDITS, dobj::SIN_EDIT_PARAM);
}

ui::dobjitem_list* parameditor::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool parameditor::do_param_edits()
{
    paramedit* pe = goto_first();
    while(pe) {
        if (wcnt::jwm.is_verbose()) {
            std::cout << "\nsetting parameters for ";
            std::cout << pe->get_name();
        }
        if (!pe->do_param_edits()) {
            dobjerr("In %s %s, parameter set attempt failed %s",
                    dobj::names::get(get_object_type()),
                                            get_username(), err_msg);
            invalidate();
            return false;
        }
        pe = goto_next();
    }
    return true;
}

const dobj::base* parameditor::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_EDIT_PARAM:
        if (!(retv = add_at_tail((paramedit*)dbj)->get_data()))
            dobjerr("Could not add parameter edit to %s", get_username());
        break;
    default:
        dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                            get_username());
        retv = 0;
    }
    return retv;
}



