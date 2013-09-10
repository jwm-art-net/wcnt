#ifndef WCNTMODULE_H
#include "../include/wcntmodule.h"

#ifndef BARE_MODULES

wcnt_module::wcnt_module(string uname) :
	synthmod(synthmodnames::MOD_WCNT, wcnt_module_count, uname),
  	in_bar(0), exit_bar(0)
{
	get_inputlist()->add_input(this, inputnames::IN_BAR);
	wcnt_module_count++;
	validate();
	create_params();
}
 
wcnt_module::~wcnt_module()
{
}

void const* wcnt_module::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_BAR:
			i = in_bar = (short*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool wcnt_module::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_EXIT_BAR:
			set_exit_bar(*(short*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

short wcnt_module::wcnt_module_count = 0;
bool wcnt_module::done_params = false;

void wcnt_module::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_WCNT, paramnames::PAR_EXIT_BAR);
	done_params = true;
}

#endif // BARE_MODULES
#endif
