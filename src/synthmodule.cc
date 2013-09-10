#ifndef SYNTHMODULE_H
#include "../include/synthmodule.h"

/* Default audio settings */
short audio_channels = 2;
unsigned long audio_samplerate = 44100ul;
short audio_bitrate = 16;
short audio_ysize = 32767;

/* Default settings for music tempo */
short sm_beats_per_minute = 160;
short sm_beats_per_measure = 4;
short sm_beat_value = 4;

synthmod::synthmod(synthmodnames::SYNTH_MOD_TYPE mt, short id, string uname) :
	err_msg(0), module_type(mt), number_id(id), valid(true)
{
	// may as well start off being valid, then if something goes wrong
	// it'll change to be invalid...
	username = uname;
	err_msg = new string;
}

#ifndef BARE_MODULES
void const* synthmod::set_in(inputnames::IN_TYPE, void const*)
{
	*err_msg = 
		"\n***programmer error***\nattempt made to set input when none exist";
	return 0;
}

void const* synthmod::get_out(outputnames::OUT_TYPE)
{
	*err_msg = 
		"\n***programmer error***\nattempt made to get output when none exist";
	return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, void const*)
{
	*err_msg = "\n***programmer error***";
	*err_msg += "\nattempt made to set parameter when none exist";
	return false;
}

dobj* synthmod::add_dobj(dobj*)
{
	*err_msg = "\n***programmer error***\nattempt made to add data object to";
	*err_msg +=	"\nmodule lacking ability to accept them";
	return 0;
}

#endif

#ifndef BARE_MODULES
iocat_names* synthmod::iocatnames = 0;
synthmodnames* synthmod::modnames = 0;
inputnames* synthmod::innames = 0;
outputnames* synthmod::outnames = 0;
paramnames* synthmod::parnames = 0;
synthmodlist* synthmod::modlist = 0;
modinputlist* synthmod::inputslist = 0;
modoutputlist* synthmod::outputslist = 0;
modparamlist* synthmod::paramlist = 0;
connectorlist* synthmod::connectlist = 0;
moddobjlist* synthmod::mdobjlist = 0;
#endif

#endif
