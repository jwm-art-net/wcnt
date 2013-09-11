#include "../include/dobjdobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dobjdobj.h"

dobjdobj * dobjdobjlist::add_dobjdobj(
 dobjnames::DOBJ_TYPE dt, dobjnames::DOBJ_TYPE kid)
{
    if (jwm.get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST
     || jwm.get_dobjnames()->check_type(kid) == dobjnames::DOBJ_FIRST)
        return 0;
    dobjdobj* dd = new dobjdobj(dt, kid);
    llitem* tmp = add_at_tail(dd);
    if (!tmp) {
        delete dd;
        return 0;
    }
    return tmp->get_data();
}

dobjdobjlist *
dobjdobjlist::get_dobjdobjlist_for_dobjtype(dobjnames::DOBJ_TYPE dt)
{
    if (jwm.get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
        return 0;
    dobjdobjlist* mdl = new dobjdobjlist(PRESERVE_DATA);
    dobjdobj* dd = goto_first();
    while(dd) {
        if (dd->get_dobj_type() == dt) {
            if (!mdl->add_dobjdobj(dd)) {
                delete mdl;
                return 0;
            }
        }
        dd = goto_next();
    }
    return mdl;
}

dobjdobjlist * dobjdobjlist::get_dobjdobjlist_of_sprogs(
 dobjnames::DOBJ_TYPE kid)
{
    if (jwm.get_dobjnames()->check_type(kid) == dobjnames::DOBJ_FIRST)
        return 0; // check_type returns DOBJ_FIRST if dt not user type
    dobjdobjlist* mdl = new dobjdobjlist(PRESERVE_DATA);
    dobjdobj* dd = goto_first();
    while(dd != 0) {
        if (dd->get_dobj_sprog() == kid) {
            if (!mdl->add_dobjdobj(dd)) {
                delete mdl;
                return 0;
            }
        }
        dd = goto_next();
    }
    return mdl;
}
