#ifndef CONSTMOD_H
#include "../include/constmod.h"

constmod::constmod(string uname) 
: synthmod(synthmodnames::MOD_CONSTMOD, constmod_count, uname),
 output(0) 
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	#endif
	constmod_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

constmod::~constmod() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	#endif
}

#ifndef BARE_MODULES

void const* constmod::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot) {
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		default:
			o = 0;
	}
	return o;
}
/*
"I'm totally in control of the drum drum drum drum drahm mascheeeen."
 - aphex twin 2002 - drukqs
*/
bool constmod::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt) {
		case paramnames::PAR_VALUE:
			set_value(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

#endif // BARE_MODULES

int constmod::constmod_count = 0;

#ifndef BARE_MODULES

bool constmod::done_params = false;

void constmod::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_CONSTMOD, paramnames::PAR_VALUE);
	done_params = true;
}

#endif
#endif
