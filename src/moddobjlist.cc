#include "../include/moddobjlist.h"

moddobjlist::moddobjlist() :
 search_type(module::ERR_TYPE), search_result(0)
{
}

moddobj* moddobjlist::register_moddobj(module::TYPE smt,
                                                    dobjnames::DOBJ_TYPE fc)
{
    if (smt <= module::ERR_TYPE || smt >= module::LAST_TYPE)
        return 0;
    moddobj* mo = new moddobj(smt, fc);
    return add_at_tail(mo)->get_data();
}

