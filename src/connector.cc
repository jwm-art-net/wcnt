#ifndef CONNECTOR_H
#include "../include/connector.h"

#ifndef BARE_MODULES

connector::connector(
	synthmod* input_module, 
	inputnames::IN_TYPE input_type, 
	string output_module_name, 
	outputnames::OUT_TYPE output_type
	) : in_mod(input_module), in_type(input_type), out_type(output_type)
{
	out_mod_uname = output_module_name;
}

connector::~connector()
{
}

bool connector::connect()
{
	// shay kit abahht  inna fine grained sieve....  ie do a handfull of checks.
	if (!this) {
		connect_err_msg = 
			"\nCannot make connection, connection does not exist!";
		return false;
	}
	synthmod* outmod = 
		synthmod::get_modlist()->get_synthmod_by_name(&out_mod_uname);
	if (!outmod) {
		connect_err_msg = 
			"\nCannot make connection, output module not found " + 
			out_mod_uname + "\ninput module " + *in_mod->get_username();
		return false;
	}
	const void* out_data = outmod->get_out(out_type);
	if (!out_data) {
		connect_err_msg = 
			"\nCannot make connection for module " + *in_mod->get_username() 
			+ ", input " + synthmod::get_inputnames()->get_name(in_type)
			+ ", module " + out_mod_uname 
			+ " does not have any output of type " 
			+ synthmod::get_outputnames()->get_name(out_type);
		return false;
	}
	if (synthmod::get_inputnames()->get_category(in_type) !=
		synthmod::get_outputnames()->get_category(out_type)) 
	{
		connect_err_msg = 
			"\nCannot make connection, input and output categories do not match:\ninput:  "
			+ *in_mod->get_username() + 
			synthmod::get_inputnames()->get_name(in_type)
			+ "\noutput: " + out_mod_uname + 
			synthmod::get_outputnames()->get_name(out_type);
		return false;
	}
	if (!in_mod->set_in(in_type, out_data)) {
		connect_err_msg = 
			"\nCannot make connection, input type " + 
			synthmod::get_inputnames()->get_name(in_type) + 
			"\nnot available in module " + *in_mod->get_username();
		return false;
	}
	return true;
}

string connector::connect_err_msg;

#endif
#endif
