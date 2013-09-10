#ifndef RIFFNODE_H
#include "../include/riffnode.h"

riff_node::riff_node() :
	dobj(dobjnames::SIN_RIFFNODE),
	start_bar(0), riff_source(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

riff_node::riff_node(riffdata* rd, short barpos) :
	dobj(dobjnames::SIN_RIFFNODE),
	start_bar(barpos), riff_source(rd)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

riff_node::~riff_node() 
{
}

#ifndef BARE_MODULES

bool riff_node::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_RIFFNAME:
			set_riff_source((riffdata*)data);// pass pointer
			retv = true;
			break;
		case dparnames::DPAR_BAR:
			set_start_bar(*(short*)data);
			retv = true;
			break;
		case dparnames::DPAR_TRANSPOSE:
			set_transpose(*(short*)data);
			retv = true;
			break;
		case dparnames::DPAR_STARTLEN:
			set_start_length(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_ENDLEN:
			set_end_length(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* riff_node::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_RIFFNAME:
			retv = get_riff_source();
			break;
		case dparnames::DPAR_BAR:
			retv = new short(get_start_bar());
			break;
		case dparnames::DPAR_TRANSPOSE:
			retv = new short(get_transpose());
			break;
		case dparnames::DPAR_STARTLEN:
			retv = new double(get_start_length());
			break;
		case dparnames::DPAR_ENDLEN:
			retv = new double(get_end_length());
			break;
		default: 
			retv = 0;
			break;
	}
	return retv;
}

bool riff_node::validate()
{
	if (start_bar < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_BAR);
		*err_msg += " is negative, should be ( >= 0)";
		invalidate();
	}
	return is_valid();
}

bool riff_node::done_dparams = false;

void riff_node::create_dparams()
{
	if (done_dparams == true) return;
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_RIFFNODE, dparnames::DPAR_RIFFNAME);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_RIFFNODE, dparnames::DPAR_BAR);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_RIFFNODE, dparnames::DPAR_TRANSPOSE);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_RIFFNODE, dparnames::DPAR_STARTLEN);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_RIFFNODE, dparnames::DPAR_ENDLEN);
	done_dparams = true;
}

#endif // BARE_MODULES
#endif
