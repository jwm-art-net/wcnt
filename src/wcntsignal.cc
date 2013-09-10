#ifndef WCNTSIGNAL_H
#include "../include/wcntsignal.h"

wcnt_signal::wcnt_signal(string uname)
:synthmod(synthmodnames::MOD_WCNTSIGNAL, wcnt_signal_count, uname), io_signal(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	#endif
	wcnt_signal_count++;
	validate();
//	create_params(); no params to create.
}

wcnt_signal::~wcnt_signal()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* wcnt_signal::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &io_signal;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* wcnt_signal::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = io_signal = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool wcnt_signal::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	return false; // no parameters
}
#endif // BARE_MODULES

int wcnt_signal::wcnt_signal_count = 0;

#endif
