#include "../include/inputeditor.h"
#include "../include/connectorlist.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>

inputeditor::inputeditor() :
 dobj(dobjnames::DEF_INPUTEDITOR)
{
    init_first();
}

inputeditor::~inputeditor()
{
}

bool inputeditor::create_connectors()
{
    inputedit* ie = goto_first();
    while(ie) {
        if (jwm.is_verbose()) {
            std::cout
                << "\nsetting inputs for " << ie->get_modname();
        }
        if (!ie->create_connectors()) {
            dobjerr("In %s %s connection attempt failed %s.",
                        dobjnames::get_name(get_object_type()),
                        get_username(), err_msg);
            return false;
        }
        ie = goto_next();
    }
    return true;
}

dobj const* inputeditor::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
        case dobjnames::SIN_EDIT_INPUT:
            if (!(retv = add_at_tail((inputedit*)dbj)->get_data())) {
                dobjerr("Could not add parameter edit to %s",
                                                    get_username());
            }
            break;
        default:
        dobjerr("%s %s to %s", stockerrs::major, stockerrs::bad_add,
                                                    get_username());
            retv = 0;
    }
    return retv;
}

void inputeditor::init_first()
{
    if (done_first())
        return;
    register_dobjdobj(dobjnames::LST_EDITS, dobjnames::SIN_EDIT_INPUT);
}



