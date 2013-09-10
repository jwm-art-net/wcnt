#ifndef DYNVERTEX_H
#include "../include/dynvertex.h"

dynvertex::dynvertex() : 
	dobj(dobjnames::SIN_DVERTEX), 
	si_level(0),	uso_level(0), lso_level(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

dynvertex::dynvertex(double sil, double usol, double lsol) :
	dobj(dobjnames::SIN_DVERTEX), 
	si_level(sil), uso_level(usol), lso_level(lsol)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

bool dynvertex::validate()
{
	return is_valid();
}

#ifndef BARE_MODULES

bool dynvertex::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_SIGIN_LEVEL:
			set_signal_in_level(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_SIGOUT_UPLEVEL:
			set_upper_signal_out_level(*(double*)data); 
			retv = true;
			break;
		case dparnames::DPAR_SIGOUT_LOLEVEL:
			set_lower_signal_out_level(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* dynvertex::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_SIGIN_LEVEL:
			retv = new double(get_signal_in_level());
			break;
		case dparnames::DPAR_SIGOUT_UPLEVEL:
			retv = new double(get_upper_signal_out_level());
			break;
		case dparnames::DPAR_SIGOUT_LOLEVEL:
			retv = new double(get_lower_signal_out_level());
			break;
		default: 
			retv = 0;
			break;
	}
	return retv;
}

void dynvertex::create_dparams()
{
	if (done_dparams == true) return;
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_DVERTEX, dparnames::DPAR_SIGIN_LEVEL);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_DVERTEX, dparnames::DPAR_SIGOUT_UPLEVEL);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_DVERTEX, dparnames::DPAR_SIGOUT_LOLEVEL);
	done_dparams = true;
}

bool dynvertex::done_dparams = false;
#endif
#endif
