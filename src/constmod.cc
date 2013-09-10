#ifndef CONSTMOD_H
#include "../include/constmod.h"

constmod::constmod(string uname) 
: synthmod(synthmodnames::MOD_CONSTMOD, constmod_count, uname),
 output(0) 
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT)){
		invalidate();
		return;
	}
	constmod_count++;
	validate();
	create_params();
}

constmod::~constmod() 
{
	get_outputlist()->delete_module_outputs(this);
}

void const* constmod::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		default:
			o = 0;
	}
	return o;
}

/*
"eat -  this -  bitch - I'm totally in control of the drum drum drum drum drahm mascheeeen."
 - aphex twin 2002
*/

bool constmod::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
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

int constmod::constmod_count = 0;
bool constmod::done_params = false;

void constmod::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_CONSTMOD, paramnames::PAR_VALUE);
	done_params = true;
}

#endif
