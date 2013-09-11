#include "../include/modinput.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"

modinput::modinput(synthmod * sm, inputnames::IN_TYPE it) :
 synthmodule(sm), input_type(it)
{
}

synthmodnames::SYNTH_MOD_TYPE modinput::get_moduletype()
{
    return (this != 0)
     ? synthmodule->get_module_type()
     : synthmodnames::FIRST;
}

inputnames::IN_TYPE modinput::get_inputtype()
{
    return (this != 0)
     ? input_type
     : inputnames::IN_FIRST;
}

iocat::IOCAT modinput::get_inputcategory()
{
    return (this != 0)
     ? jwm.get_inputnames()->get_category(input_type)
     : iocat::FIRST;
}

