#include "../include/moddobj.h"
#include "../include/dobjdobjlist.h"

moddobj::moddobj(module::TYPE smt, 
                 dobjnames::DOBJ_TYPE fc) :
 synthmodule_type(smt), first_child(fc), dd_list(0)
{
    dd_list = new dobjdobjlist;
}

moddobj::~moddobj()
{
    delete dd_list;
}

