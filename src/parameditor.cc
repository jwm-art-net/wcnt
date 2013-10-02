#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <cstdio>

parameditor::parameditor() :
 dobj(dataobj::DEF_PARAMEDITOR)
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
            dobjerr("In %s %s, parameter set attempt failed %s",
                    dataobj::names::get(get_object_type()),
                                            get_username(), err_msg);
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
    dataobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dataobj::SIN_EDIT_PARAM:
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


void parameditor::init_first()
{
    if (done_first())
        return;
    register_dobjdobj(dataobj::LST_EDITS, dataobj::SIN_EDIT_PARAM);
}



