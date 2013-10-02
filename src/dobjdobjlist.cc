#include "../include/dobjdobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dobjdobj.h"

dobjdobj * dobjdobjlist::register_dobjdobj(
 dataobj::TYPE parent, dataobj::TYPE sprog)
{
    if (!dataobj::names::chk(parent) || !dataobj::names::chk(sprog))
        return 0;

    dobjdobj* dd = new dobjdobj(parent, sprog);
    llitem* tmp = add_at_tail(dd);
    if (!tmp) {
        delete dd;
        return 0;
    }
    return tmp->get_data();
}

dobjdobjlist *
dobjdobjlist::get_dobjdobjlist_for_dobjtype(dataobj::TYPE dt)
{
    if (!dataobj::names::chk(dt))
        return 0;
    dobjdobjlist* mdl = new dobjdobjlist(PRESERVE_DATA);
    dobjdobj* dd = goto_first();
    while(dd) {
        if (dd->get_dobj_type() == dt) {
            if (!mdl->register_dobjdobj(dd)) {
                delete mdl;
                return 0;
            }
        }
        dd = goto_next();
    }
    return mdl;
}

#ifdef UNUSED
dobjdobjlist * dobjdobjlist::get_dobjdobjlist_of_sprogs(dataobj::TYPE sprog)
{
    if (!dataobj::names::chk(sprog))
        return 0;
    dobjdobjlist* mdl = new dobjdobjlist(PRESERVE_DATA);
    dobjdobj* dd = goto_first();
    while(dd != 0) {
        if (dd->get_dobj_sprog() == sprog) {
            if (!mdl->register_dobjdobj(dd)) {
                delete mdl;
                return 0;
            }
        }
        dd = goto_next();
    }
    return mdl;
}
#endif
