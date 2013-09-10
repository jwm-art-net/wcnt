#ifndef INPUTEDITOR_H
#include "../include/inputeditor.h"
#include "../include/connectorlist.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <string>

inputeditor::inputeditor() :
 dobj(dobjnames::DEF_INPUTEDITOR),
 input_edit_list(0), input_item(0), input_edit(0)
{
    input_edit_list = 
     new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
}

inputeditor::~inputeditor()
{
    goto_first_inputedit();
    while(input_item) {
        delete input_edit;
        goto_next_inputedit();
    }
    delete input_edit_list;
}

inputedit* inputeditor::add_inputedit(inputedit* ie)
{
    if (!(input_item = input_edit_list->add_at_tail(ie)))
        return input_edit = 0;
    return input_edit = (inputedit*)input_item->get_data();
}

bool inputeditor::create_connectors()
{
    goto_first_inputedit();
    while(input_item) {
        if (jwm.is_verbose()) {
            std::cout
                << "\nsetting inputs for " << input_edit->get_modname();
        }
        if (!input_edit->create_connectors()) {
            std::string errmsg = *err_msg;
            *err_msg = "\nIn ";
            *err_msg += jwm.get_dobjnames().get_name(get_object_type());
            *err_msg += " ";
            *err_msg += get_username();
            *err_msg += ", connection attempt failed, ";
            *err_msg += errmsg;
            return false;
        }
        goto_next_inputedit();
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
        if (!(retv = add_inputedit((inputedit*)dbj)))
            *err_msg = "\ncould not add parameter edit to ";
            *err_msg += *get_username();
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
    dobjdobjlist* dbjlist = jwm.get_topdobjlist().create_dobjdobjlist(
        dobjnames::DEF_INPUTEDITOR, dobjnames::LST_EDITS);
    dbjlist->add_dobjdobj(
        dobjnames::LST_EDITS, dobjnames::SIN_EDIT_INPUT);
    inputeditor::done_params = true;
}

bool inputeditor::done_params = false;

#endif
