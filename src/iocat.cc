#ifndef IOCAT_H
#include "../include/iocat.h"
#ifndef BARE_MODULES

iocat_names::iocat_names() :
 ioname(0)
{
    ioname = new char*[CAT_LAST];
    ioname[CAT_FIRST] = "Error Cat";
    ioname[CAT_DOUBLE] = "float_value";
    ioname[CAT_SHORT] = "integer_value";
    ioname[CAT_ULONG] = "integer_value";
    ioname[CAT_TRIG] = "on/off";
    ioname[CAT_STATE] = "on/off";
    ioname[CAT_STRING] = "text_string";
    ioname[CAT_MOD_FUNC]="add/sub/mul/div/mod/sin/cos/tan/and/or/xor";
    ioname[CAT_LOGIC] = "and/or/xor";
    ioname[CAT_LOOP_MODE] = "off/fwd/rev/bi";
    ioname[CAT_PLAY_DIR] = "fwd/rev";
    ioname[CAT_PLAY_MODE] = "stop/wrap/bounce/jump";
    ioname[CAT_JUMP_DIR] = "play/loop";
    ioname[CAT_METER] = "integer_value/integer_value";
    ioname[CAT_ADSRSECT] = "attack/decay/sustain/release";
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
#endif
