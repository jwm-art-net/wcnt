#include "../include/dobjparamlist.h"
#include "../include/dobj.h"

dobjparam*
dobjparamlist::add_dobjparam(
    dobjnames::DOBJ_TYPE dt, param::TYPE dp)
{
    dobjparam* dobjp = new dobjparam(dt, dp);
    dobjparam* dpar = add_at_tail(dobjp)->get_data();
    if (!dpar) {
        delete dobjp;
        return 0;
    }
    return dpar;
}

bool
dobjparamlist::validate(
 dobj* dbj, param::TYPE dpt, stockerrs::ERR_TYPE et)
{
    if (!dbj)
        return false;
    if (!stockerrs::check_type(et)
             &&
        et < stockerrs::ERR_TYPE4)
        return false;

    dobjnames::DOBJ_TYPE dt = dbj->get_object_type();
    dobjparam* dpar = goto_first();
    while (dpar) {
        if (dpar->get_dobjtype() == dt) {
            if (dpar->get_partype() == dpt)
                return dpar->validate(dbj, et);
        }
        dpar = goto_next();
    }
    return false;
}

