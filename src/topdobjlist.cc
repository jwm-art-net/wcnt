#include "../include/topdobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjdobj.h"

topdobjlist::topdobjlist() :
 search_result(0), search_type(dataobj::ERR_TYPE)
{
}

topdobjlist::~topdobjlist()
{
}

dobjdobjlist* topdobjlist::create_dobjdobjlist(
    dataobj::TYPE parent, dataobj::TYPE child)
{
    if (dataobj::names::category(parent) == dataobj::CAT_ERR_TYPE
     || dataobj::names::category(child) == dataobj::CAT_ERR_TYPE)
        return 0;

    dobjdobjlist* newddlist = new dobjdobjlist;
    newddlist->register_dobjdobj(parent, child);
    if (!add_at_tail(newddlist)) {
        delete newddlist;
        return 0;
    }
    return newddlist;
}
