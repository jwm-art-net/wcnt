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
            std::string errmsg = *err_msg;
            *err_msg = "\nIn ";
            *err_msg += jwm.get_dobjnames()->get_name(get_object_type());
            *err_msg += " ";
            *err_msg += get_username();
            *err_msg += ", connection attempt failed, ";
            *err_msg += errmsg;
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
                *err_msg = "\ncould not add parameter edit to ";
                *err_msg += *get_username();
            }
            break;
        default:
            *err_msg = "\n***major error*** attempt made to add an ";
            *err_msg += "\ninvalid object type to ";
            *err_msg += *get_username();
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

