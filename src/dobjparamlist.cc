#include "../include/dobjparamlist.h"
#include "../include/dobj.h"

#include <iostream>


dobjparam*
dobjparamlist::add_dobjparam(
    dataobj::TYPE dt, param::TYPE dp)
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
dobjparamlist::validate(dobj* dbj, param::TYPE dpt, errors::TYPE et)
{
    if (!dbj)
        return false;

    if (!errors::stock::chk(et))
        return false;

    #ifdef DEBUG
    switch(errors::stock::category(et)) {
    case errors::CAT_COULD_NOT:
    case errors::CAT_INVALID:
    case errors::CAT_OTHER:
        std::cout << "Is this an error here???" << std::endl;
    default:
        ;
    }
    #endif

    dataobj::TYPE dt = dbj->get_object_type();
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

