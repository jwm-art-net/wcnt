#include "../include/inputeditor.h"
#include "../include/connectorlist.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <string>


inputeditor::inputeditor() :
 dobj(dobjnames::DEF_INPUTEDITOR)
{
    create_params();
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
                        jwm.get_dobjnames()->get_name(get_object_type()),
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
            dobjerr("*** MAJOR ERROR *** Bad attempt made to add \
                        invalid object type to %s.", get_username());
            retv = 0;
    }
    return retv;
}

void inputeditor::create_params()
{
    if (inputeditor::done_params == true)
        return;
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_INPUTEDITOR, dobjnames::LST_EDITS)->
            add_dobjdobj(dobjnames::LST_EDITS,
                dobjnames::SIN_EDIT_INPUT);
    inputeditor::done_params = true;
}

bool inputeditor::done_params = false;

