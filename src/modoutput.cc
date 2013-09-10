#ifndef MODOUTPUT_H
#include "../include/modoutput.h"

#ifndef BARE_MODULES

modoutput::modoutput(synthmod * sm, outputnames::OUT_TYPE ot)
: synthmodule(sm), output_type(ot)
{
}

synthmodnames::SYNTH_MOD_TYPE modoutput::get_moduletype()
{
	return (this != 0) 
		? synthmodule->get_module_type() 
		: synthmodnames::MOD_FIRST;
}

outputnames::OUT_TYPE modoutput::get_outputtype()
{
	return (this != 0) 
		? output_type 
		: outputnames::OUT_FIRST;
}

IOCAT modoutput::get_outputcategory()
{
	return (this != 0) 
		? synthmod::get_outputnames()->get_category(output_type) 
		: CAT_FIRST;
}

#endif
#endif
