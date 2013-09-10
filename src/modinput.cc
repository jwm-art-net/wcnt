#ifndef MODINPUT_H
#include "../include/modinput.h"

#ifndef BARE_MODULES

modinput::modinput(synthmod * sm, inputnames::IN_TYPE it)
: synthmodule(sm), input_type(it)
{
}

synthmodnames::SYNTH_MOD_TYPE modinput::getmoduletype()
{
	return (this != 0) ? synthmodule->get_module_type() : synthmodnames::MOD_FIRST;
}

inputnames::IN_TYPE modinput::getinputtype()
{
	return (this != 0) ? input_type : inputnames::IN_FIRST;
}

IOCAT modinput::getinputcategory()
{
	return (this != 0) ? synthmod::get_inputnames()->getcategory(input_type) : CAT_FIRST;
}

#endif
#endif
