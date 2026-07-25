#include "../include/editor.h"
#include "../include/setparam.h"
#include "../include/globals.h"
#include "../include/ui_moditem.h"
#include "../include/ui_dobjitem.h"

#include <cstdio>

#include <iostream>

editor::editor() :
 dobj::base(dobj::DEF_EDITOR)
{
    set_flags(DO_ALLOW_IN_AUTOGROUP | DO_ALLOW_NAMELESS);
}

editor::~editor()
{
}

void editor::register_ui()
{
    register_dobj(dobj::LST_EDITS, dobj::SIN_EDIT);
}

ui::dobjitem_list* editor::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}

bool editor::do_actions()
{
    edit* e = goto_first();
    while(e) {
        if (wcnt::jwm.is_verbose()) {
            std::cout << "\nsetting items for ";
            std::cout << e->get_target_name();
        }
        if (!e->do_actions()) {
            dobjerr("In %s %s, item set attempt failed: %s",
                    dobj::names::get(get_object_type()), get_username(), err_msg);
            invalidate();
            return false;
        }
        e = goto_next();
    }
    return true;
}

const dobj::base* editor::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_EDIT:
        if (!(retv = add_at_tail((edit*)dbj)->get_data()))
            dobjerr("Could not add parameter edit to %s", get_username());
        break;
    default:
        dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                            get_username());
        retv = 0;
    }
    return retv;
}



