#ifndef INPUTNAMES_H
#include "../include/inputnames.h"

inputnames::inputnames()
: inname(NULL), incat(NULL)
{
	inname = new string[IN_LAST + 1];
	incat = new IOCAT[IN_LAST + 1];
	if (inname == NULL || incat == NULL)
	{
		cerr << "\nextreme lack of memory exiting....";
		cerr << "\ncould not creat list of input names and categories";
		exit(1);
	}
	inname[IN_FIRST] = "BAD_INPUT_TYPE";					incat[IN_FIRST] = CAT_FIRST;
	inname[IN_VELOCITY] = "in_velocity";					incat[IN_VELOCITY] = CAT_DOUBLE;
	inname[IN_NOTE_ON_TRIG] = "in_note_on_trig";			incat[IN_NOTE_ON_TRIG] = CAT_TRIG;
	inname[IN_NOTE_SLIDE_TRIG] = "in_note_slide_trig";	incat[IN_NOTE_SLIDE_TRIG] = CAT_TRIG;
	inname[IN_NOTE_OFF_TRIG] = "in_note_off_trig";		incat[IN_NOTE_OFF_TRIG] = CAT_TRIG;
	inname[IN_LEFT] = "in_left";							incat[IN_LEFT] = CAT_SHORT;
	inname[IN_RIGHT] = "in_right";						incat[IN_RIGHT] = CAT_SHORT;
	inname[IN_FREQ] = "in_frequency";					incat[IN_FREQ] = CAT_DOUBLE;
	inname[IN_FREQ_MOD1] = "in_freq_mod1";				incat[IN_FREQ_MOD1] = CAT_DOUBLE;
	inname[IN_FREQ_MOD2] = "in_freq_mod2";				incat[IN_FREQ_MOD2] = CAT_DOUBLE;
	inname[IN_PHASE_TRIG] = "in_phase_trig";				incat[IN_PHASE_TRIG] = CAT_TRIG;
	inname[IN_DEG_SIZE] = "in_deg_size";					incat[IN_DEG_SIZE] = CAT_DOUBLE;
	inname[IN_SIGNAL] = "in_signal";						incat[IN_SIGNAL] = CAT_DOUBLE;
	inname[IN_EG] = "in_amp_eg";								incat[IN_EG] = CAT_DOUBLE;
	inname[IN_AMP_MOD] = "in_amp_mod";					incat[IN_AMP_MOD] = CAT_DOUBLE;
	inname[IN_PAN_MOD] = "in_pan_mod";					incat[IN_PAN_MOD] = CAT_DOUBLE;
	inname[IN_TRIG] = "in_trig";							incat[IN_TRIG] = CAT_TRIG;
	inname[IN_WAVE] = "in_waveform";						incat[IN_WAVE] = CAT_DOUBLE;
	inname[IN_PWM] = "in_pwm";							incat[IN_PWM] = CAT_DOUBLE;
	inname[IN_CUTOFF_DEG_SIZE] = "in_cutoff_degsize";		incat[IN_CUTOFF_DEG_SIZE] = CAT_DOUBLE;
	inname[IN_RES_AMP_MOD] = "in_res_amp_mod";			incat[IN_RES_AMP_MOD] = CAT_DOUBLE;
	inname[IN_PLAY_TRIG] = "in_play_trig";				incat[IN_PLAY_TRIG] = CAT_TRIG;
	inname[IN_START_POS_MOD] = "in_start_pos_mod";		incat[IN_START_POS_MOD] = CAT_DOUBLE;
	inname[IN_MODIFIER] = "in_modifier";					incat[IN_MODIFIER] = CAT_DOUBLE;
	inname[IN_NORM_MOD] = "in_normal_freq_mod";			incat[IN_NORM_MOD] = CAT_DOUBLE;
	inname[IN_V_MOD] = "in_level_mod";					incat[IN_V_MOD] = CAT_DOUBLE;
	inname[IN_H_MOD] = "in_time_mod";					incat[IN_H_MOD] = CAT_DOUBLE;
	inname[IN_TRIG1] = "in_trig1";						incat[IN_TRIG1] = CAT_TRIG;
	inname[IN_TRIG2] = "in_trig1";						incat[IN_TRIG2] = CAT_TRIG;
}

inputnames::~inputnames()
{
	if (inname)
		delete[] inname;
	if (incat)
		delete[] incat;
}

string inputnames::getname(IN_TYPE id)
{
	if (id >= IN_FIRST && id < IN_LAST) 
		return inname[id];
	else 
		return inname[IN_FIRST];
}

IOCAT inputnames::getcategory(IN_TYPE id)
{
	if (id >= IN_FIRST && id < IN_LAST)
		return incat[id];
	else
		return incat[IN_FIRST];
}

inputnames::IN_TYPE inputnames::get_type(string const* iname)
{
	int i;
	for (i = IN_FIRST + 1; i < IN_LAST; i++) 
		if (inname[i] == *iname)
			return (IN_TYPE)i;
	return IN_FIRST;
}

#endif
