#ifndef PARAMNAMES_H
#include "../include/paramnames.h"


paramnames::paramnames()
: parname(0), parcat(0)
{
	parname = new string[PAR_LAST + 1];
	parcat = new IOCAT[PAR_LAST + 1];
	if (!parname || !parcat)
	{
		cerr << "\nextreme lack of memory exiting....";
		cerr << "\ncould not creat list of parameter names and categories";
		exit(1);
	}
	parname[PAR_FIRST] = "BAD_PARAM_TYPE";				parcat[PAR_FIRST] = CAT_FIRST;
	parname[PAR_SUSTAIN_STATUS] = "sustain_state"; 		parcat[PAR_SUSTAIN_STATUS] = CAT_STATE;
	parname[PAR_ZERO_RETRIGGER] = "zero_retrigger";		parcat[PAR_ZERO_RETRIGGER] = CAT_STATE;
	parname[PAR_NOTELEN_FREQ] = "notelen_freq";			parcat[PAR_NOTELEN_FREQ] = CAT_SHORT;
	parname[PAR_FREQ] = "freq"; 							parcat[PAR_FREQ] = CAT_DOUBLE;
	parname[PAR_FREQ_MOD1SIZE] = "freq_mod1_size";		parcat[PAR_FREQ_MOD1SIZE] = CAT_DOUBLE;
	parname[PAR_FREQ_MOD2SIZE] = "freq_mod2_size";		parcat[PAR_FREQ_MOD2SIZE] = CAT_DOUBLE;
	parname[PAR_VALUE] = "value";						parcat[PAR_VALUE] = CAT_DOUBLE;
	parname[PAR_SIG_RANGE_HI] = "sig_range_hi";			parcat[PAR_SIG_RANGE_HI] = CAT_DOUBLE;
	parname[PAR_SIG_RANGE_LO] = "sig_range_lo";			parcat[PAR_SIG_RANGE_LO] = CAT_DOUBLE;
	parname[PAR_FREQ_RANGE_HI] = "freq_range_hi";		parcat[PAR_FREQ_RANGE_HI] = CAT_DOUBLE;
	parname[PAR_FREQ_RANGE_LO] = "freq_range_lo";		parcat[PAR_FREQ_RANGE_LO] = CAT_DOUBLE;
	parname[PAR_STEP_COUNT] = "step_count";				parcat[PAR_STEP_COUNT] = CAT_SHORT;
	parname[PAR_DELAY_TIME] = "delay_time";				parcat[PAR_DELAY_TIME] = CAT_DOUBLE;
	parname[PAR_RAMP_TIME] = "ramp_time";				parcat[PAR_RAMP_TIME] = CAT_DOUBLE;
	parname[PAR_START_LEVEL] = "start_level";			parcat[PAR_START_LEVEL] = CAT_DOUBLE;
	parname[PAR_END_LEVEL] = "end_level";				parcat[PAR_END_LEVEL] = CAT_DOUBLE;
	parname[PAR_AMP_MODSIZE] = "amp_mod_size";			parcat[PAR_AMP_MODSIZE] = CAT_DOUBLE;
	parname[PAR_RES_LEVEL] = "res_level";				parcat[PAR_RES_LEVEL] = CAT_DOUBLE;
	parname[PAR_RES_AMP_MODSIZE] = "res_amp_modsize";	parcat[PAR_RES_AMP_MODSIZE] = CAT_DOUBLE;
	parname[PAR_MODIFIER_FUNC] = "modifier_func";		parcat[PAR_MODIFIER_FUNC] = CAT_MOD_FUNC;// modifier::MOD_FUNC
	parname[PAR_MODIFIER_SIZE] = "modifier_size";			parcat[PAR_MODIFIER_SIZE] = CAT_DOUBLE;
	parname[PAR_MIN_OUT] = "min_out";					parcat[PAR_MIN_OUT] = CAT_DOUBLE;
	parname[PAR_MAX_OUT] = "max_out";					parcat[PAR_MAX_OUT] = CAT_DOUBLE;
	parname[PAR_POSNEG_MIRROR] = "posneg_mirror";		parcat[PAR_POSNEG_MIRROR] = CAT_STATE;
	parname[PAR_FREQ_MOD1_LO] = "freq_mod1_lo";			parcat[PAR_FREQ_MOD1_LO] = CAT_DOUBLE;
	parname[PAR_FREQ_MOD1_HI] = "freq_mod1_hi";			parcat[PAR_FREQ_MOD1_HI] = CAT_DOUBLE;
	parname[PAR_FREQ_MOD2_LO] = "freq_mod2_lo";			parcat[PAR_FREQ_MOD2_LO] = CAT_DOUBLE;
	parname[PAR_FREQ_MOD2_HI] = "freq_mod2_hi";			parcat[PAR_FREQ_MOD2_HI] = CAT_DOUBLE;
	parname[PAR_OCTAVE] = "octave_offset";				parcat[PAR_OCTAVE] = CAT_SHORT;
	parname[PAR_PORTAMENTO] = "portamento_time";		parcat[PAR_PORTAMENTO] = CAT_DOUBLE;
	parname[PAR_DECAY_TIME] = "decay_time";				parcat[PAR_DECAY_TIME] = CAT_DOUBLE;
	parname[PAR_START_POS_MIN] = "start_pos_min";		parcat[PAR_START_POS_MIN] = CAT_ULONG;
	parname[PAR_START_POS_MAX] = "start_pos_max";		parcat[PAR_START_POS_MAX] = CAT_ULONG;
	parname[PAR_LOOP_BEGIN] = "loop_start_pos";			parcat[PAR_LOOP_BEGIN] = CAT_ULONG;
	parname[PAR_LOOP_END] = "loop_end_pos";				parcat[PAR_LOOP_END] = CAT_ULONG;
	parname[PAR_LOOP_MODE] = "loop_mode";				parcat[PAR_LOOP_MODE] = CAT_LOOP_MODE;
	parname[PAR_ANTI_CLIP] = "anti_clip_samples";			parcat[PAR_ANTI_CLIP] = CAT_SHORT;
	parname[PAR_ROOT_NOTE] = "root_note";				parcat[PAR_ROOT_NOTE] = CAT_NOTENAME;// ie C#0
	parname[PAR_ROOT_FREQ] = "root_freq";				parcat[PAR_ROOT_FREQ] = CAT_DOUBLE;
	parname[PAR_ROOT_DEGSIZE] = "root_degsize"; 			parcat[PAR_ROOT_DEGSIZE] = CAT_DOUBLE;
	parname[PAR_DEGSIZE_AMOUNT] = "degsize_amount";		parcat[PAR_DEGSIZE_AMOUNT] = CAT_DOUBLE;
	parname[PAR_RECYCLE_MODE] = "recycle_mode";			parcat[PAR_RECYCLE_MODE] = CAT_STATE;
	parname[PAR_TRIG_ZERO_DEG] = "on_trig_reset_phase";	parcat[PAR_TRIG_ZERO_DEG] = CAT_STATE;
	parname[PAR_CYCLES] = "phase_cycles";				parcat[PAR_CYCLES] = CAT_DOUBLE;
	parname[PAR_PULSE_WIDTH] = "pulse_width";			parcat[PAR_PULSE_WIDTH] = CAT_DOUBLE;
	parname[PAR_PWM_SIZE] = "pwm_size";					parcat[PAR_PWM_SIZE] = CAT_DOUBLE;
	parname[PAR_AMPLITUDE] = "amplitude";				parcat[PAR_AMPLITUDE] = CAT_SHORT;
	parname[PAR_PAN] = "pan";							parcat[PAR_PAN] = CAT_DOUBLE;
	parname[PAR_PAN_MODSIZE] = "pan_mod_size";			parcat[PAR_PAN_MODSIZE] = CAT_DOUBLE;
	parname[PAR_CLIP_MODE] = "clip_mode";				parcat[PAR_CLIP_MODE] = CAT_CLIP_MODE; //stereoamp::CLIP_MODE
	parname[PAR_CLIP_LEVEL] = "clip_level";				parcat[PAR_CLIP_LEVEL] = CAT_SHORT;
	parname[PAR_DC_LEVEL] = "dc_level";					parcat[PAR_DC_LEVEL] = CAT_SHORT;
	parname[PAR_MASTER_LEVEL] = "master_level";			parcat[PAR_MASTER_LEVEL] = CAT_DOUBLE;
	parname[PAR_NORM_FREQ] = "normal_freq";				parcat[PAR_NORM_FREQ] = CAT_DOUBLE;
	parname[PAR_NORM_MOD_HI] = "norm_mod_hi";			parcat[PAR_NORM_MOD_HI] = CAT_DOUBLE;
	parname[PAR_NORM_MOD_LO] = "norm_mod_lo";			parcat[PAR_NORM_MOD_LO] = CAT_DOUBLE;
	parname[PAR_TRIGGER_LEVEL] = "trigger_level";		parcat[PAR_TRIGGER_LEVEL] = CAT_DOUBLE;
	parname[PAR_FILENAME] = "filename";					parcat[PAR_FILENAME] = CAT_FILENAME;
	parname[PAR_WAVFILEIN] = "wavfile";					parcat[PAR_WAVFILEIN] = CAT_WAVFILEIN;
	parname[PAR_WRITE_FROM_BAR] = "write_from_bar";		parcat[PAR_WRITE_FROM_BAR] = CAT_SHORT;
	parname[PAR_WRITE_TO_BAR] = "write_to_bar";			parcat[PAR_WRITE_TO_BAR] = CAT_SHORT;
	parname[PAR_WRITE_FROM_SAMPLE] = "write_from_sample";	parcat[PAR_WRITE_FROM_SAMPLE] = CAT_ULONG;
	parname[PAR_WRITE_TO_SAMPLE] = "write_to_sample";	parcat[PAR_WRITE_TO_SAMPLE] = CAT_ULONG;
	parname[PAR_VELOCITY_RESPONSE] = "velocity_response_time"; parcat[PAR_VELOCITY_RESPONSE] = CAT_DOUBLE;
	parname[PAR_PROBABILITY] = "probability";				parcat[PAR_PROBABILITY] = CAT_DOUBLE;
	parname[PAR_NOTPROBABILITY] = "not_probability";				parcat[PAR_NOTPROBABILITY] = CAT_DOUBLE;
	parname[PAR_LOGICFUNC] = "function";					parcat[PAR_LOGICFUNC] = CAT_LOGIC;
	parname[PAR_XFADE_TIME] = "xfade_time";				parcat[PAR_XFADE_TIME] = CAT_DOUBLE;
	parname[PAR_TUNING_SEMITONES] = "tuning_semitones";	parcat[PAR_TUNING_SEMITONES] = CAT_DOUBLE;
	parname[PAR_MEAN_TOTAL] = "mean_total";				parcat[PAR_MEAN_TOTAL] = CAT_STATE;
}

paramnames::~paramnames()
{
	if (parname)
		delete[] parname;
	if (parcat)
		delete[] parcat;
}

string paramnames::getname(PAR_TYPE id)
{
	if (id >= PAR_FIRST && id < PAR_LAST) 
		return parname[id];
	else 
		return parname[PAR_FIRST];
}

IOCAT paramnames::getcategory(PAR_TYPE id)
{
	if (id >= PAR_FIRST && id < PAR_LAST)
		return parcat[id];
	else
		return parcat[PAR_FIRST];
}

paramnames::PAR_TYPE paramnames::get_type(string const* pname)
{
	int i;
	for (i = PAR_FIRST + 1; i < PAR_LAST; i++) 
		if (parname[i] == *pname)
			return (PAR_TYPE)i;
	return PAR_FIRST;
}

#endif
