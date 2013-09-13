#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <string>
#include <cstdio>

parameditor::parameditor() :
 dobj(dobjnames::DEF_PARAMEDITOR)
{
    create_params();
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
                    jwm.get_dobjnames()->get_name(get_object_type()),
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
        dobjerr("*** MAJOR ERROR *** bad attempt to add invalid object "
                                           "type to %s", get_username());
        retv = 0;
    }
    return retv;
}

void parameditor::create_params()
{
    if (parameditor::done_params == true)
        return;
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_PARAMEDITOR, dobjnames::LST_EDITS)->
            add_dobjdobj(dobjnames::LST_EDITS,
                dobjnames::SIN_EDIT_PARAM);
    parameditor::done_params = true;
}

bool parameditor::done_params = false;

