#ifndef SYNTHMODULE_H
#include "../include/synthmodule.h"

/* Default audio settings */
short audio_channels = 2;
unsigned long audio_samplerate = 44100ul;
short audio_bitrate = 16;
short audio_ysize = 32767;

/* Default settings for music tempo */
double sm_beats_per_minute = 160;
short sm_beats_per_measure = 4;
short sm_beat_value = 4;

synthmod::synthmod(
 synthmodnames::SYNTH_MOD_TYPE mt, short id, char const* uname) :
 module_type(mt), number_id(id), username(0), valid(true)
{
    username = new char[strlen(uname) + 1];
    strcpy(username, uname);
}

synthmod::~synthmod()
{
    delete [] username;
}

void const* synthmod::set_in(inputnames::IN_TYPE, void const*)
{
    *err_msg = 
  "\n**programmer error**\nattempt made to set input when none exist\n";
    return 0;
}

void const* synthmod::get_out(outputnames::OUT_TYPE)
{
    *err_msg =
  "\n**programmer error**\nattempt made to get output when none exist\n";
    return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, void const*)
{
    *err_msg = "\n***programmer error***";
    *err_msg += "\nattempt made to set parameter when none exist\n";
    return false;
}

void const* synthmod::get_param(paramnames::PAR_TYPE)
{
    *err_msg = "\n***programmer error***";
    *err_msg += "\nattempt made to get parameter when none exist\n";
    return 0;
}

dobj* synthmod::add_dobj(dobj*)
{
    *err_msg = 
     "\n**programmer error**\nattempt made to add data object to";
    *err_msg += "\nmodule lacking ability to accept them\n";
    return 0;
}

string* synthmod::err_msg = 0;
char* synthmod::path = 0;
synthmodnames* synthmod::modnames = 0;
iocat_names* synthmod::iocatnames = 0;
inputnames* synthmod::innames = 0;
outputnames* synthmod::outnames = 0;
paramnames* synthmod::parnames = 0;
synthmodlist* synthmod::modlist = 0;
modinputlist* synthmod::inputslist = 0;
modoutputlist* synthmod::outputslist = 0;
modparamlist* synthmod::paramlist = 0;
connectorlist* synthmod::connectlist = 0;
moddobjlist* synthmod::mdobjlist = 0;
fxsparamlist* synthmod::fxsparlist = 0;

#endif
