#ifndef IOCAT_H
#include "../include/iocat.h"

iocat_names::iocat_names() :
 ioname(0)
{
    ioname = new char*[CAT_LAST + 1];
    ioname[CAT_FIRST] = "Error Cat";
    ioname[CAT_DOUBLE] = "float_value";
    ioname[CAT_SHORT] = "integer_value";
    ioname[CAT_ULONG] = "integer_value";
    ioname[CAT_TRIG] = "on/off";
    ioname[CAT_STATE] = "on/off";
    ioname[CAT_STRING] = "text_string";
    ioname[CAT_FIX_STR] = "fixed_string";
    ioname[CAT_METER] = "integer_value/integer_value";
    ioname[CAT_DOBJ] = "data_object_name";
    ioname[CAT_SYNTHMOD] = "module_name";
}

iocat_names::~iocat_names()
{
    delete [] ioname;
}

char* iocat_names::get_name(IOCAT iocat)
{
    if (iocat <= CAT_FIRST || iocat >= CAT_LAST)
        return ioname[CAT_FIRST];
    return ioname[iocat];
}

#endif
