#ifndef PARAMEDITOR_H
#include "../include/parameditor.h"
#include "../include/setparam.h"

parameditor::parameditor() :
 dobj(dobjnames::DEF_PARAMEDITOR),
 par_edit_list(0), edit_item(0), par_edit(0), verbose(false)
{
    par_edit_list = 
     new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
}

parameditor::~parameditor()
{
    goto_first_paramedit();
    while(edit_item) {
        delete par_edit;
        goto_next_paramedit();
    }
    delete par_edit_list;
}

paramedit* parameditor::add_paramedit(paramedit* pe)
{
    if (!(edit_item = par_edit_list->add_at_tail(pe)))
        return par_edit = 0;
    return par_edit = (paramedit*)edit_item->get_data();
}

bool parameditor::do_param_edits()
{
    goto_first_paramedit();
    while(edit_item) {
        if (verbose) {
            cout << "\nsetting parameters for ";
            cout << par_edit->get_name();
        }
        if (!par_edit->do_param_edits(verbose)) {
            invalidate();
            return false;
        }
        goto_next_paramedit();
    }
    return true;
}

dobj const* parameditor::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_EDIT_PARAM:
        if (!(retv = add_paramedit((paramedit*)dbj)))
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

void parameditor::create_params()
{
    if (parameditor::done_params == true)
        return;
    dobjdobjlist* dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_PARAMEDITOR, dobjnames::LST_EDITS);
    dbjlist->add_dobjdobj(
        dobjnames::LST_EDITS, dobjnames::SIN_EDIT_PARAM);
    parameditor::done_params = true;
}

bool parameditor::done_params = false;

#endif
