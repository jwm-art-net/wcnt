#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <cstdio>

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
            dobjerr("In %s %s, parameter set attempt failed %s",
                    dobjnames::get_name(get_object_type()),
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
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_EDIT_PARAM:
        if (!(retv = add_at_tail((paramedit*)dbj)->get_data()))
            dobjerr("Could not add parameter edit to %s", get_username());
        break;
    default:
        dobjerr("%s %s to %s", stockerrs::major, stockerrs::bad_add,
                                                        get_username());
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



