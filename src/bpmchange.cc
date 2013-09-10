#ifndef BPMCHANGE_H
#include "../include/bpmchange.h"

bpmchange::bpmchange() :
	dobj(dobjnames::SIN_BPM),
	atbar(0), tobpm(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

bpmchange::bpmchange(short bar, double bpm) :
	dobj(dobjnames::SIN_BPM),
	atbar(bar), tobpm(bpm)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

#ifndef BARE_MODULES

bool bpmchange::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_BPM:
			set_bpm(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_BAR:
			set_bar(*(short*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* bpmchange::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_BPM:
			retv = new double(get_bpm());
			break;
		case dparnames::DPAR_BAR:
			retv = new short(get_bar());
			break;
		default: 
			retv = 0;
			break;
	}
	return retv;
}

bool bpmchange::validate()
{
	if (tobpm <= MIN_BPM || tobpm > MAX_BPM) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_BPM);
		*err_msg += " out of range (20 to 1000)";
		invalidate();
	}
	if (atbar < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_BAR);
		*err_msg += " is negative, should be ( >= 0)";
		invalidate();
	}
	return is_valid();
}

bool bpmchange::done_dparams = false;

void bpmchange::create_dparams()
{
	if (done_dparams == true) return;
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_BPM, dparnames::DPAR_BPM);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_BPM, dparnames::DPAR_BAR);
	done_dparams = true;
}

#endif // BARE_MODULES
#endif
