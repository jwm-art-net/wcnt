#ifndef MODINPUT_H
#include "../include/modinput.h"

modinput::modinput(synthmod * sm, inputnames::IN_TYPE it) :
 synthmodule(sm), input_type(it)
{
}

synthmodnames::SYNTH_MOD_TYPE modinput::get_moduletype()
{
    return (this != 0)
     ? synthmodule->get_module_type()
     : synthmodnames::MOD_FIRST;
}

inputnames::IN_TYPE modinput::get_inputtype()
{
    return (this != 0)
     ? input_type
     : inputnames::IN_FIRST;
}

IOCAT modinput::get_inputcategory()
{
    return (this != 0)
     ? synthmod::get_inputnames()->get_category(input_type)
     : CAT_FIRST;
}

#endif
