#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <string>

parameditor::parameditor() :
 dobj(dobjnames::DEF_PARAMEDITOR)
{
    init_first();
}

bool parameditor::do_param_edits()
{
    paramedit* pe = goto_first();
    while(pe) {
        if (jwm.is_verbose()) {
            std::cout << "\nsetting parameters for ";
            std::cout << pe->get_name();
        }
        if (!pe->do_param_edits()) {
            std::string errmsg = *err_msg;
            *err_msg = "\nIn ";
            *err_msg += jwm.get_dobjnames()->get_name(get_object_type());
            *err_msg += " ";
            *err_msg += get_username();
            *err_msg += ", set parameter attempt failed, ";
            *err_msg += errmsg;
            invalidate();
            return false;
        }
        pe = goto_next();
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
        if (!(retv = add_at_tail((paramedit*)dbj)->get_data()))
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


void parameditor::init_first()
{
    if (done_first())
        return;
    register_dobjdobj(dobjnames::LST_EDITS, dobjnames::SIN_EDIT_PARAM);
}



