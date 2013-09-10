#ifndef METERCHANGE_H
#include "../include/meterchange.h"

meterchange::meterchange() :
	dobj(dobjnames::SIN_METER),
	bar(0), beatsperbar(0), beatvalue(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

meterchange::meterchange(short br, char btpb, char btval) :
	dobj(dobjnames::SIN_METER),
	bar(br), beatsperbar(btpb), beatvalue(btval)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

#ifndef BARE_MODULES

bool meterchange::set_dparam(dparnames::DPAR_TYPE pt, void* data)
{
	bool retv = false;
	switch(pt)
	{
		case dparnames::DPAR_METER:
			set_beatsperbar(((timesig*)data)->beatsperbar);
			set_beatvalue(((timesig*)data)->beatvalue);
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

void* meterchange::get_dparam(dparnames::DPAR_TYPE pt)
{
	void* retv = 0;
	switch(pt)
	{
		case dparnames::DPAR_METER:
			retv = new timesig;
			((timesig*)retv)->beatsperbar = get_beatsperbar();
			((timesig*)retv)->beatvalue = get_beatvalue();
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

bool meterchange::validate()
{
	if (beatsperbar <= 2 || beatsperbar > 32) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_METER);
		*err_msg += " beatsperbar is out of range (2 to 32)";
		invalidate();
	}
	if (beatvalue <= 2 || beatvalue > 32) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_METER);
		*err_msg += " beatvalue is out of range (2 to 32)";
		invalidate();
	}
	if (bar < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_BAR);
		*err_msg += " is negative, should be ( >= 0)";
		invalidate();
	}
	return is_valid();
}

bool meterchange::done_dparams = false;

void meterchange::create_dparams()
{
	if (done_dparams == true) return;
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_METER, dparnames::DPAR_METER);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_METER, dparnames::DPAR_BAR);
	done_dparams = true;
}

#endif // BARE_MODULES
#endif
