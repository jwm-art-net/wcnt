#ifndef NOTEDATA_H
#include "../include/notedata.h"

note_data::note_data() :
	dobj(dobjnames::SIN_NOTE), length(0), position(0), velocity(0)
{
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

note_data::note_data(const char *name, double len, double pos, double vel) :
	dobj(dobjnames::SIN_NOTE), length(len), position(pos), velocity(vel)
{
    set_name(name);
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

note_data::~note_data()
{
}

void note_data::set_name(const char *name)
{
    strncpy(notename, name, NOTE_NAME_SIZE - 1);
    notename[NOTE_NAME_SIZE - 1] = '\0';        /* incase len > 4 chars */
}

#ifndef BARE_MODULES

bool note_data::set_dparam(dparnames::DPAR_TYPE dt, void* data)
{
	bool retv = false;
	switch(dt)
	{
		case dparnames::DPAR_NOTENAME:
			set_name((char*)data); // pass pointer
			retv = true;
			break;
		case dparnames::DPAR_NOTEPOS:
			set_position(*(double*)data); 
			retv = true;
			break;
		case dparnames::DPAR_NOTELEN:
			set_length(*(double*)data);
			retv = true;
			break;
		case dparnames::DPAR_NOTEVEL:
			set_velocity(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* note_data::get_dparam(dparnames::DPAR_TYPE dt)
{
	void* retv = 0;
	switch(dt)
	{
		case dparnames::DPAR_NOTENAME:
			retv = new char[NOTE_NAME_SIZE];
		    strncpy((char*)retv, notename, NOTE_NAME_SIZE - 1);
    		((char*)retv)[NOTE_NAME_SIZE - 1] = '\0';
			break;
		case dparnames::DPAR_NOTEPOS:
			retv = new double(get_position());
			break;
		case dparnames::DPAR_NOTELEN:
			retv = new double(get_length());
			break;
		case dparnames::DPAR_NOTEVEL:
			retv = new double(get_velocity());
			break;
		default: 
			retv = 0;
			break;
	}
	return retv;
}

bool note_data::validate()
{
	if (check_notename(notename)) {
		*err_msg = "\n";
		*err_msg += notename;
		*err_msg += " is an invalid note name";
		invalidate();
	}
	if (position < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_NOTEPOS);
		*err_msg += " must be positive";
		invalidate();
	}
	if (length < 0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_NOTELEN);
		*err_msg += " must be positive";
		invalidate();
	}
	if (velocity < 0 || velocity > 1.0) {
		*err_msg += "\n" + get_dparnames()->get_name(dparnames::DPAR_NOTEVEL);
		*err_msg += " must be within range 0.0 to +1.0";
		invalidate();
	}
	return is_valid();
}

void note_data::create_dparams()
{
	if (note_data::done_dparams == true) return;
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_NOTE, dparnames::DPAR_NOTENAME);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_NOTE, dparnames::DPAR_NOTEPOS);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_NOTE, dparnames::DPAR_NOTELEN);
	get_dobjparamlist()->add_dobjparam(dobjnames::SIN_NOTE, dparnames::DPAR_NOTEVEL);
	note_data::done_dparams = true;
}

bool note_data::done_dparams = false;

#endif
#endif
