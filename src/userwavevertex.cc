#ifndef USERWAVEVERTEX_H
#include "../include/userwavevertex.h"

wave_vertex::wave_vertex() : 
	dobj(dobjnames::SIN_VERTEX), output(0, 0), lower(0, 0), upper(0, 0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

wave_vertex::wave_vertex(double udp, double ul, double ldp, double ll) : 
	dobj(dobjnames::SIN_VERTEX), output(0, 0), lower(ldp, ll), upper(udp, ul)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}
	
void wave_vertex::modulate(double v_mod, double h_mod)
{
	double up_pos = upper.get_position();
	double lo_pos = lower.get_position();
	double up_lvl = upper.get_level();
	double lo_lvl = lower.get_level();
	output.set_position(lo_pos + (up_pos - lo_pos) * ((h_mod < 0) ? -h_mod : h_mod));
	output.set_level(lo_lvl + (up_lvl - lo_lvl) * ((v_mod < 0) ? -v_mod : v_mod));
}

#ifndef BARE_MODULES

bool wave_vertex::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_UPDEG:
			set_upper_position(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_UPLEVEL:
			set_upper_level(*(double*)data); 
			retv = true;
			break;
		case dparnames::DPAR_LODEG:
			set_lower_position(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_LOLEVEL:
			set_lower_level(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* wave_vertex::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_UPDEG:
			retv = new double(get_upper_position());
			break;
		case dparnames::DPAR_UPLEVEL:
			retv = new double(get_upper_level());
			break;
		case dparnames::DPAR_LODEG:
			retv = new double(get_lower_position());
			break;
		case dparnames::DPAR_LOLEVEL:
			retv = new double(get_lower_level());
			break;
		default: 
			retv = 0;
			break;
	}
	return retv;
}

bool wave_vertex::validate()
{
	if (upper.get_level() < -1.0 || upper.get_level() > 1.0) {
		*err_msg = "\n" + get_dparnames()->get_name(dparnames::DPAR_UPLEVEL);
		*err_msg += " out of range (-1.0 to +1.0)";
		invalidate();
	}
	if (lower.get_level() < -1.0 || lower.get_level() > 1.0) {
		*err_msg = "\n" + get_dparnames()->get_name(dparnames::DPAR_LOLEVEL);
		*err_msg += " out of range (-1.0 to +1.0)";
		invalidate();
	}
	if (upper.get_position() < 0 || upper.get_position() > 360) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_UPDEG);
		*err_msg += " out of range (0.0 to +360.0)";
		invalidate();
	}		
	if (lower.get_position() < 0 || lower.get_position() > 360) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_LODEG);
		*err_msg += " out of range (0.0 to +360.0)";
		invalidate();
	}		
	return is_valid();
}

void wave_vertex::create_dparams()
{
	if (done_dparams == true) return;
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_VERTEX, dparnames::DPAR_UPDEG);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_VERTEX, dparnames::DPAR_UPLEVEL);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_VERTEX, dparnames::DPAR_LODEG);
	get_dobjparamlist()->
		add_dobjparam(dobjnames::SIN_VERTEX, dparnames::DPAR_LOLEVEL);
	done_dparams = true;
}

bool wave_vertex::done_dparams = false;
#endif


#endif
