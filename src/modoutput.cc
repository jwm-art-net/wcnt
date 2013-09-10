#ifndef MODOUTPUT_H
#include "../include/modoutput.h"

#ifndef BARE_MODULES

modoutput::modoutput(synthmod * sm, outputnames::OUT_TYPE ot)
: synthmodule(sm), output_type(ot)
{
}

synthmodnames::SYNTH_MOD_TYPE modoutput::getmoduletype()
{
	return (this != 0) ? synthmodule->get_module_type() : synthmodnames::MOD_FIRST;
}

outputnames::OUT_TYPE modoutput::getoutputtype()
{
	return (this != 0) ? output_type : outputnames::OUT_FIRST;
}

IOCAT modoutput::getoutputcategory()
{
	return (this != 0) ? synthmod::get_outputnames()->getcategory(output_type) : CAT_FIRST;
}

#endif
#endif
