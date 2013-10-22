#include "../include/inputeditor.h"
#include "../include/connectorlist.h"
#include "../include/globals.h"

#include <iostream>

inputeditor::inputeditor() :
 dobj::base(dobj::DEF_INPUTEDITOR)
{
}

void inputeditor::register_ui()
{
    register_dobj(dobj::LST_EDITS, dobj::SIN_EDIT_INPUT);
}

ui::dobjitem_list* inputeditor::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

inputeditor::~inputeditor()
{
}

bool inputeditor::create_connectors()
{
    inputedit* ie = goto_first();
    while(ie) {
        if (wcnt::jwm.is_verbose()) {
            std::cout
                << "\nsetting inputs for " << ie->get_modname();
        }
        if (!ie->create_connectors()) {
            dobjerr("In %s %s connection attempt failed %s.",
                        dobj::names::get(get_object_type()),
                        get_username(), err_msg);
            return false;
        }
        ie = goto_next();
    }
    return true;
}

const dobj::base* inputeditor::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
        case dobj::SIN_EDIT_INPUT:
            if (!(retv = add_at_tail((inputedit*)dbj)->get_data())) {
                dobjerr("Could not add parameter edit to %s",
                                                    get_username());
            }
            break;
        default:
        dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
            retv = 0;
    }
    return retv;
}


