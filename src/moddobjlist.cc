#include "../include/moddobjlist.h"

moddobjlist::moddobjlist() :
 search_type(synthmodnames::FIRST), search_result(0)
{
}

moddobj* moddobjlist::register_moddobj(synthmodnames::SYNTH_MOD_TYPE smt,
                                                    dobjnames::DOBJ_TYPE fc)
{
    if (smt <= synthmodnames::FIRST || smt >= synthmodnames::LAST)
        return 0;
    moddobj* mo = new moddobj(smt, fc);
    return add_at_tail(mo)->get_data();
}

