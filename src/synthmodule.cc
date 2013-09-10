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

synthmod::synthmod(synthmodnames::SYNTH_MOD_TYPE mt, short id, string uname)
: module_type(mt), number_id(id), valid(false)
{
	username = uname;
}

synthmodnames* synthmod::modnames = 0;
#ifndef BARE_MODULES
inputnames* synthmod::innames = 0;
outputnames* synthmod::outnames = 0;
paramnames* synthmod::parnames = 0;
synthmodlist* synthmod::modlist = 0;
modinputlist* synthmod::inputslist = 0;
modoutputlist* synthmod::outputslist = 0;
modparamlist* synthmod::paramlist = 0;
connectorlist* synthmod::connectlist = 0;
#endif

#endif
