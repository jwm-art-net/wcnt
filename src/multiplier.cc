#ifndef MULTIPLIER_H
#include "../include/multiplier.h"

multiplier::multiplier(string uname) : 
	synthmod(synthmodnames::MOD_MULTIPLIER, multiplier_count, uname),
	in_signal(0), in_mod(0), out_output(0.00)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	get_inputlist()->add_input(this, inputnames::IN_MODIFIER);
	#endif
	multiplier_count++;
	validate();
	// no params, no bother
}

multiplier::~multiplier() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* multiplier::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &out_output;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* multiplier::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		case inputnames::IN_MODIFIER:
			i = in_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}
#endif

int multiplier::multiplier_count = 0;
// go on, give us a snare rush rush rush rush.
#endif
