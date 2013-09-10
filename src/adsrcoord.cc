#ifndef ADSRCOORD_H
#include "../include/adsrcoord.h"

adsr_coord::adsr_coord() :
	dobj(dobjnames::SIN_COORD), sect(ADSR_FIRST), 
	upper_time(0), upper_level(0), lower_time(0), lower_level(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

adsr_coord::adsr_coord
	(adsr_coord::SECT s, double ut, double ul, double lt, double ll) :
	dobj(dobjnames::SIN_COORD), sect(s), upper_time(ut), upper_level(ul), 
	lower_time(lt), lower_level(ll)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

adsr_coord::~adsr_coord()
{
}

void adsr_coord::run(double velocity) 
{
	if (velocity < 0) 
		velocity = -velocity;
	if (velocity > 1.0) 
		velocity = 1.0;
	output_time = lower_time + (upper_time - lower_time) * velocity;
	output_level = lower_level + (upper_level - lower_level) * velocity;
}

#ifndef BARE_MODULES

bool adsr_coord::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_ADSRSECT:
			set_adsr_section(*(SECT*)data);
			retv = true;
			break;
		case dparnames::DPAR_UPTIME:
			set_upper_time(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_UPLEVEL:
			set_upper_level(*(double*)data); 
			retv = true;
			break;
		case dparnames::DPAR_LOTIME:
			set_lower_time(*(double*)data);
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

void* adsr_coord::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_ADSRSECT:
			retv = new adsr_coord::SECT(get_adsr_section());
			break;
		case dparnames::DPAR_UPTIME:
			retv = new double(get_upper_time());
			break;
		case dparnames::DPAR_UPLEVEL:
			retv = new double(get_upper_level());
			break;
		case dparnames::DPAR_LOTIME:
			retv = new double(get_lower_time());
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

bool adsr_coord::validate()
{
	if (sect <= ADSR_FIRST || sect >= ADSR_LAST) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_ADSRSECT);
		*err_msg += " " + get_iocatnames()->get_name(
			get_dparnames()->get_category(dparnames::DPAR_ADSRSECT));
		invalidate();
	}
	if (upper_level < -1.0 || upper_level > 1.0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_UPLEVEL);
		*err_msg += " out of range (-1.0 to +1.0)";
		invalidate();
	}
	if (lower_level < -1.0 || lower_level > 1.0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_LOLEVEL);
		*err_msg += " out of range (-1.0 to +1.0)";
		invalidate();
	}
	if (upper_time < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_UPTIME);
		*err_msg += " is negative ( >= 0)";
		invalidate();
	}		
	if (lower_time < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_LOTIME);
		*err_msg += " is negative ( >= 0)";
		invalidate();
	}
	return is_valid();
}

void adsr_coord::create_dparams()
{
	if (adsr_coord::done_dparams == true) return;
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_COORD, dparnames::DPAR_ADSRSECT);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_COORD, dparnames::DPAR_UPTIME);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_COORD, dparnames::DPAR_UPLEVEL);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_COORD, dparnames::DPAR_LOTIME);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_COORD, dparnames::DPAR_LOLEVEL);
	done_dparams = true;
}

bool adsr_coord::done_dparams = false;
#endif

#endif
