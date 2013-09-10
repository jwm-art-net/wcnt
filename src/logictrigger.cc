#ifndef LOGICTRIGGER_H
#include "../include/logictrigger.h"

logictrigger::logictrigger(string uname) 
: synthmod(synthmodnames::MOD_LOGICTRIGGER, logictrigger_count, uname),
  in_trig1(0), in_trig2(0), out_trig(OFF)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_TRIG1);
	get_inputlist()->add_input(this, inputnames::IN_TRIG2);
	#endif
	logictrigger_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

logictrigger::~logictrigger()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* logictrigger::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_TRIG:
			o = &out_trig;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* logictrigger::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_TRIG1:
			i = in_trig1 = (STATUS*)o;
			break;
		case inputnames::IN_TRIG2:
			i = in_trig2 = (STATUS*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool logictrigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_LOGICFUNC:
			set_logicfunc(*(LOGIC_FUNC*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif // BARE_MODULES

void logictrigger::run() 
{
	if (logicfunc == AND){
		if (*in_trig1 == ON && *in_trig2 == ON)
			out_trig = ON;
		else if (out_trig == ON)
			out_trig = OFF;
	} else if (logicfunc == OR){
		if (*in_trig1 == ON || *in_trig2 == ON)
			out_trig = ON;
		else if (out_trig == ON)
			out_trig = OFF;
	} else { // logicfunc == XOR
		if ((*in_trig1 == ON && *in_trig2 == OFF) 
				|| 
			(*in_trig1 == OFF && *in_trig2 == ON))
			out_trig = ON;
		else if (out_trig == ON)
			out_trig = OFF;
	}
}

int logictrigger::logictrigger_count = 0;

#ifndef BARE_MODULES
bool logictrigger::done_params = false;

void logictrigger::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LOGICTRIGGER, paramnames::PAR_LOGICFUNC);
	done_params = true;
}
#endif
#endif
