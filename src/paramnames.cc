#ifndef paramnames_H
#include "../include/paramnames.h"

#include "../include/dobj.h"

paramnames::paramnames() : parname(0), parcat(0)
{
    parname = new char*[(short)PAR_LAST + 1];
    parcat = new IOCAT[(short)PAR_LAST + 1];
    for (int i = PAR_FIRST + 1; i < PAR_LAST; i++) {
        parname[i] = 0;
    }
    parname[PAR_FIRST] = "BAD_PARAM_TYPE";
    parcat[PAR_FIRST] = CAT_FIRST;
    parname[PAR_SUSTAIN_STATUS] = "sustain_state"; 
    parcat[PAR_SUSTAIN_STATUS] = CAT_STATE;
    parname[PAR_ZERO_RETRIGGER] = "zero_retrigger";
    parcat[PAR_ZERO_RETRIGGER] = CAT_STATE;
    parname[PAR_FREQ] = "freq"; 
    parcat[PAR_FREQ] = CAT_DOUBLE;
    parname[PAR_FREQ_MOD1SIZE] = "freq_mod1_size";
    parcat[PAR_FREQ_MOD1SIZE] = CAT_DOUBLE;
    parname[PAR_FREQ_MOD2SIZE] = "freq_mod2_size";
    parcat[PAR_FREQ_MOD2SIZE] = CAT_DOUBLE;
    parname[PAR_VALUE] = "value";
    parcat[PAR_VALUE] = CAT_DOUBLE;
    parname[PAR_SIG_RANGE_HI] = "sig_range_hi";
    parcat[PAR_SIG_RANGE_HI] = CAT_DOUBLE;
    parname[PAR_SIG_RANGE_LO] = "sig_range_lo";
    parcat[PAR_SIG_RANGE_LO] = CAT_DOUBLE;
    parname[PAR_FREQ_RANGE_HI] = "freq_range_hi";
    parcat[PAR_FREQ_RANGE_HI] = CAT_DOUBLE;
    parname[PAR_FREQ_RANGE_LO] = "freq_range_lo";
    parcat[PAR_FREQ_RANGE_LO] = CAT_DOUBLE;
    parname[PAR_STEP_COUNT] = "step_count";
    parcat[PAR_STEP_COUNT] = CAT_SHORT;
    parname[PAR_DELAY_TIME] = "delay_time";
    parcat[PAR_DELAY_TIME] = CAT_DOUBLE;
    parname[PAR_RAMP_TIME] = "ramp_time";
    parcat[PAR_RAMP_TIME] = CAT_DOUBLE;
    parname[PAR_START_LEVEL] = "start_level";
    parcat[PAR_START_LEVEL] = CAT_DOUBLE;
    parname[PAR_END_LEVEL] = "end_level";
    parcat[PAR_END_LEVEL] = CAT_DOUBLE;
    parname[PAR_AMP_MODSIZE] = "amp_mod_size";
    parcat[PAR_AMP_MODSIZE] = CAT_DOUBLE;
    parname[PAR_MODIFIER_FUNC] = "function";
    parcat[PAR_MODIFIER_FUNC] = CAT_FIX_STR;
    parname[PAR_MODIFIER_SIZE] = "modifier_size";
    parcat[PAR_MODIFIER_SIZE] = CAT_DOUBLE;
    parname[PAR_MIN_OUT] = "min_out";
    parcat[PAR_MIN_OUT] = CAT_DOUBLE;
    parname[PAR_MAX_OUT] = "max_out";
    parcat[PAR_MAX_OUT] = CAT_DOUBLE;
    parname[PAR_POSNEG_MIRROR] = "posneg_mirror";
    parcat[PAR_POSNEG_MIRROR] = CAT_STATE;
    parname[PAR_OCTAVE] = "octave_offset";
    parcat[PAR_OCTAVE] = CAT_SHORT;
    parname[PAR_PORTAMENTO] = "portamento_time";
    parcat[PAR_PORTAMENTO] = CAT_DOUBLE;
    parname[PAR_DECAY_TIME] = "decay_time";
    parcat[PAR_DECAY_TIME] = CAT_DOUBLE;
    parname[PAR_START_POS_MIN] = "start_pos_min";
    parcat[PAR_START_POS_MIN] = CAT_ULONG;
    parname[PAR_START_POS_MAX] = "start_pos_max";
    parcat[PAR_START_POS_MAX] = CAT_ULONG;
    parname[PAR_LOOP_BEGIN] = "loop_start_pos";
    parcat[PAR_LOOP_BEGIN] = CAT_ULONG;
    parname[PAR_LOOP_END] = "loop_end_pos";
    parcat[PAR_LOOP_END] = CAT_ULONG;
    parname[PAR_LOOP_MODE] = "loop_mode";
    parcat[PAR_LOOP_MODE] = CAT_FIX_STR;
    parname[PAR_ANTI_CLIP] = "anti_clip_samples";
    parcat[PAR_ANTI_CLIP] = CAT_SHORT;
    parname[PAR_DEGSIZE_AMOUNT] = "degsize_amount";
    parcat[PAR_DEGSIZE_AMOUNT] = CAT_DOUBLE;
    parname[PAR_RECYCLE_MODE] = "recycle_mode";
    parcat[PAR_RECYCLE_MODE] = CAT_STATE;
    parname[PAR_TRIG_ZERO_DEG] = "on_trig_reset_phase";
    parcat[PAR_TRIG_ZERO_DEG] = CAT_STATE;
    parname[PAR_CYCLES] = "phase_cycles";
    parcat[PAR_CYCLES] = CAT_DOUBLE;
    parname[PAR_PULSE_WIDTH] = "pulse_width";
    parcat[PAR_PULSE_WIDTH] = CAT_DOUBLE;
    parname[PAR_PWM_SIZE] = "pwm_size";
    parcat[PAR_PWM_SIZE] = CAT_DOUBLE;
    parname[PAR_AMPLITUDE] = "amplitude";
    parcat[PAR_AMPLITUDE] = CAT_SHORT;
    parname[PAR_PAN] = "pan";
    parcat[PAR_PAN] = CAT_DOUBLE;
    parname[PAR_PAN_MODSIZE] = "pan_mod_size";
    parcat[PAR_PAN_MODSIZE] = CAT_DOUBLE;
    parname[PAR_CLIP_LEVEL] = "clip_level";
    parcat[PAR_CLIP_LEVEL] = CAT_SHORT;
    parname[PAR_MASTER_LEVEL] = "master_level";
    parcat[PAR_MASTER_LEVEL] = CAT_DOUBLE;
    parname[PAR_NORM_FREQ] = "normal_freq";
    parcat[PAR_NORM_FREQ] = CAT_DOUBLE;
    parname[PAR_NORM_MODSIZE] = "norm_modsize";
    parcat[PAR_NORM_MODSIZE] = CAT_DOUBLE;
    parname[PAR_TRIGGER_LEVEL] = "trigger_level";
    parcat[PAR_TRIGGER_LEVEL] = CAT_DOUBLE;
    parname[PAR_FILENAME] = "filename";
    parcat[PAR_FILENAME] = CAT_STRING;
    parname[PAR_WAVFILEIN] = "wavfile";
    parcat[PAR_WAVFILEIN] = CAT_DOBJ;
    parname[PAR_VELOCITY_RESPONSE] = "velocity_response_time";
    parcat[PAR_VELOCITY_RESPONSE] = CAT_DOUBLE;
    parname[PAR_PROBABILITY] = "probability";
    parcat[PAR_PROBABILITY] = CAT_DOUBLE;
    parname[PAR_NOTPROBABILITY] = "not_probability";
    parcat[PAR_NOTPROBABILITY] = CAT_DOUBLE;
    parname[PAR_LOGICFUNC] = "function";
    parcat[PAR_LOGICFUNC] = CAT_FIX_STR;
    parname[PAR_XFADE_TIME] = "xfade_time";
    parcat[PAR_XFADE_TIME] = CAT_DOUBLE;
    parname[PAR_TUNING_SEMITONES] = "tuning_semitones";
    parcat[PAR_TUNING_SEMITONES] = CAT_DOUBLE;
    parname[PAR_MEAN_TOTAL] = "mean_total";
    parcat[PAR_MEAN_TOTAL] = CAT_STATE;
    parname[PAR_WAV_BASENAME] = "basename"; 
    parcat[PAR_WAV_BASENAME] = CAT_STRING;
    parname[PAR_PLAY_DIR] = "play_dir"; 
    parcat[PAR_PLAY_DIR] = CAT_FIX_STR;
    parname[PAR_PLAY_MODE] = "play_mode";
    parcat[PAR_PLAY_MODE] = CAT_FIX_STR;
    parname[PAR_JUMP_MODE] = "jump_mode";
    parcat[PAR_JUMP_MODE] = CAT_FIX_STR;
    parname[PAR_LOOP_IS_OFFSET] = "loop_is_offset";
    parcat[PAR_LOOP_IS_OFFSET] = CAT_STATE;
    parname[PAR_LOOP_BI_OFFSET]="bi-directional_offset";
    parcat[PAR_LOOP_BI_OFFSET] = CAT_SHORT;
    parname[PAR_AC_EACH_END] = "anti_clip_each_end";
    parcat[PAR_AC_EACH_END] = CAT_STATE;
    parname[PAR_POWER_MIN] = "power_min";
    parcat[PAR_POWER_MIN] = CAT_DOUBLE;
    parname[PAR_POWER_MAX] = "power_max"; 
    parcat[PAR_POWER_MAX] = CAT_DOUBLE;
    parname[PAR_RUDE_MODE] = "rude_mode"; 
    parcat[PAR_RUDE_MODE] = CAT_STATE;
    parname[PAR_WETDRY] = "wet/dry";
    parcat[PAR_WETDRY] = CAT_DOUBLE;
    parname[PAR_START_BAR] = "start_bar";
    parcat[PAR_START_BAR] = CAT_SHORT;
    parname[PAR_END_BAR] = "end_bar";
    parcat[PAR_END_BAR] = CAT_SHORT;
    parname[PAR_ZERO_SEARCH_RANGE]="zero_search_range";
    parcat[PAR_ZERO_SEARCH_RANGE] = CAT_SHORT;
    parname[PAR_GAIN] = "gain";
    parcat[PAR_GAIN] = CAT_DOUBLE;
    parname[PAR_GAIN_MODSIZE] = "gain_modsize";
    parcat[PAR_GAIN_MODSIZE] = CAT_DOUBLE;
    parname[PAR_FEED_LEVEL] = "feedback";
    parcat[PAR_FEED_LEVEL] = CAT_DOUBLE;
    parname[PAR_FEED_MODSIZE] = "feedback_modsize";
    parcat[PAR_FEED_MODSIZE] = CAT_DOUBLE;
    parname[PAR_EXIT_BAR] = "exit_bar";
    parcat[PAR_EXIT_BAR] = CAT_SHORT;
    parname[PAR_RESPONSE_TIME] = "response_time";
    parcat[PAR_RESPONSE_TIME] = CAT_DOUBLE;
    parname[PAR_LEFT_AMPLITUDE] = "left_amplitude";
    parcat[PAR_LEFT_AMPLITUDE] = CAT_SHORT;
    parname[PAR_RIGHT_AMPLITUDE] = "right_amplitude";
    parcat[PAR_RIGHT_AMPLITUDE] = CAT_SHORT;
    parname[PAR_RMS_TIME] = "rms_time";
    parcat[PAR_RMS_TIME] = CAT_DOUBLE;
    parname[PAR_DC_TIME] = "dc_time";
    parcat[PAR_DC_TIME] = CAT_DOUBLE;
    parname[PAR_UP_THRESH] = "up_thresh";
    parcat[PAR_UP_THRESH] = CAT_DOUBLE;
    parname[PAR_LO_THRESH] = "lo_thresh";
    parcat[PAR_LO_THRESH] = CAT_DOUBLE;
    parname[PAR_LEVEL] = "level";
    parcat[PAR_LEVEL] = CAT_DOUBLE;
    parname[PAR_ATTACK_TIME] = "attack_time";
    parcat[PAR_ATTACK_TIME] = CAT_DOUBLE;
    parname[PAR_ATTACK_LEVEL] = "attack_level";
    parcat[PAR_ATTACK_LEVEL] = CAT_DOUBLE;
    parname[PAR_RELEASE_TIME] = "release_time";
    parcat[PAR_RELEASE_TIME] = CAT_DOUBLE;
    parname[PAR_RELEASE_LEVEL] = "release_level";
    parcat[PAR_RELEASE_LEVEL] = CAT_DOUBLE;
    parname[PAR_INDEPENDANT] = "independant";
    parcat[PAR_INDEPENDANT] = CAT_STATE;
    parname[PAR_USE_RATIOS] = "use_ratios";
    parcat[PAR_USE_RATIOS] = CAT_STATE;
    parname[PAR_NO_LONOTE] = "no_lo_notename";
    parcat[PAR_NO_LONOTE] = CAT_STRING;
    parname[PAR_NO_HINOTE] = "no_hi_notename";
    parcat[PAR_NO_HINOTE] = CAT_STRING;
    parname[PAR_NS_LONOTE] = "ns_lo_notename";
    parcat[PAR_NS_LONOTE] = CAT_STRING;
    parname[PAR_NS_HINOTE] = "ns_hi_notename";
    parcat[PAR_NS_HINOTE] = CAT_STRING;
    parname[PAR_MINNO_OUT] = "min_no_out";
    parcat[PAR_MINNO_OUT] = CAT_DOUBLE;
    parname[PAR_MAXNO_OUT] = "max_no_out";
    parcat[PAR_MAXNO_OUT] = CAT_DOUBLE;
    parname[PAR_MINNS_OUT] = "min_ns_out";
    parcat[PAR_MINNS_OUT] = CAT_DOUBLE;
    parname[PAR_MAXNS_OUT] = "max_ns_out";
    parcat[PAR_MAXNS_OUT] = CAT_DOUBLE;
    parname[PAR_DETRAN_NO] = "detranspose_no";
    parcat[PAR_DETRAN_NO] = CAT_STATE;
    parname[PAR_DETRAN_NS] = "detranspose_ns";
    parcat[PAR_DETRAN_NS] = CAT_STATE;
    parname[PAR_NO_RESPTIME] = "no_response_time";
    parcat[PAR_NO_RESPTIME] = CAT_DOUBLE;
    parname[PAR_NS_RESPTIME] = "ns_response_time";
    parcat[PAR_NS_RESPTIME] = CAT_DOUBLE;
    parname[PAR_MIN_WAIT] = "min_wait_time";
    parcat[PAR_MIN_WAIT] = CAT_DOUBLE;
    parname[PAR_MAX_WAIT] = "max_wait_time";
    parcat[PAR_MAX_WAIT] = CAT_DOUBLE;
    parname[PAR_TRIG_STRING] = "pattern";
    parcat[PAR_TRIG_STRING] = CAT_STRING;
    parname[PAR_QUARTER_VAL] = "quarter_value";
    parcat[PAR_QUARTER_VAL] = CAT_SHORT;
    parname[PAR_RELEASE_RATIO] = "release_is_ratio";
    parcat[PAR_RELEASE_RATIO] = CAT_STATE;
    parname[PAR_MIN_TIME] = "min_time";
    parcat[PAR_MIN_TIME] = CAT_DOUBLE;
    parname[PAR_MAX_SUSTAIN_TIME] = "max_sustain_time";
    parcat[PAR_MAX_SUSTAIN_TIME] = CAT_DOUBLE;
    parname[PAR_PRECISION] = "precision";
    parcat[PAR_PRECISION] = CAT_SHORT;
    parname[PAR_NAME] = "name";
    parcat[PAR_NAME] = CAT_STRING;
    parname[PAR_NOTE_POS] = "pos";
    parcat[PAR_NOTE_POS] = CAT_DOUBLE;
    parname[PAR_NOTE_LEN] = "len";
    parcat[PAR_NOTE_LEN] = CAT_DOUBLE;
    parname[PAR_NOTE_VEL] = "vel";
    parcat[PAR_NOTE_VEL] = CAT_DOUBLE;
    parname[PAR_ADSRSECT] = "name";
    parcat[PAR_ADSRSECT] = CAT_FIX_STR;
    parname[PAR_UPTIME] = "ut";
    parcat[PAR_UPTIME] = CAT_DOUBLE;
    parname[PAR_LOTIME] = "lt";
    parcat[PAR_LOTIME] = CAT_DOUBLE;
    parname[PAR_UPLEVEL] = "ul";
    parcat[PAR_UPLEVEL] = CAT_DOUBLE;
    parname[PAR_LOLEVEL] = "ll";
    parcat[PAR_LOLEVEL] = CAT_DOUBLE;
    parname[PAR_UPDEG] = "ud";
    parcat[PAR_UPDEG] = CAT_DOUBLE;
    parname[PAR_LODEG] = "ld";
    parcat[PAR_LODEG] = CAT_DOUBLE;
    parname[PAR_METER] = "tsig";
    parcat[PAR_METER] = CAT_METER;
    parname[PAR_BAR] = "bar";
    parcat[PAR_BAR] = CAT_SHORT;
    parname[PAR_BPM] = "bpm";
    parcat[PAR_BPM] = CAT_DOUBLE;
    parname[PAR_RIFFNAME] = "riff";
    parcat[PAR_RIFFNAME] = CAT_DOBJ;
    parname[PAR_MODNAME] = "name";
    parcat[PAR_MODNAME] = CAT_SYNTHMOD;
    parname[PAR_ROOT_NOTE] = "root_note";
    parcat[PAR_ROOT_NOTE] = CAT_STRING;
    parname[PAR_TRANSPOSE] = "transpose";
    parcat[PAR_TRANSPOSE] = CAT_SHORT;
    parname[PAR_STARTLEN] = "start_len";
    parcat[PAR_STARTLEN] = CAT_DOUBLE;
    parname[PAR_ENDLEN] = "end_len";
    parcat[PAR_ENDLEN] = CAT_DOUBLE;
    parname[PAR_VALUE] = "value";
    parcat[PAR_VALUE] = CAT_DOUBLE;
    parname[PAR_SIGIN_LEVEL] = "sigin_level";
    parcat[PAR_SIGIN_LEVEL] = CAT_DOUBLE;
    parname[PAR_SIGOUT_UPLEVEL] = "so_ul";
    parcat[PAR_SIGOUT_UPLEVEL] = CAT_DOUBLE;
    parname[PAR_SIGOUT_LOLEVEL] = "so_ll";
    parcat[PAR_SIGOUT_LOLEVEL] = CAT_DOUBLE;
    parname[PAR_SECONDS] = "seconds";
    parcat[PAR_SECONDS] = CAT_DOUBLE;
    parname[PAR_MOD_ACTION] = "mod_action";
    parcat[PAR_MOD_ACTION] = CAT_FIX_STR;
    parname[PAR_DOBJ_ACTION] = "dobj_action";
    parcat[PAR_DOBJ_ACTION] = CAT_FIX_STR;
    parname[PAR_NAME_EXT] = "name_extension";
    parcat[PAR_NAME_EXT] = CAT_STRING;
    parname[PAR_PARAMETER] = "param";
    parcat[PAR_PARAMETER] = CAT_STRING;
    parname[PAR_VALUESTR] = "value";
    parcat[PAR_VALUESTR] = CAT_STRING;
    parname[PAR_DOBJNAME] = "name";
    parcat[PAR_DOBJNAME] = CAT_DOBJ;
    parname[PAR_INMODULE] = "in_module";
    parcat[PAR_INMODULE] = CAT_STRING;
    parname[PAR_INPUT] = "input";
    parcat[PAR_INPUT] = CAT_STRING;
    parname[PAR_OUTMODULE] = "out_module";
    parcat[PAR_OUTMODULE] = CAT_STRING;
    parname[PAR_OUTPUT] = "output";
    parcat[PAR_OUTPUT] = CAT_STRING;
    parname[PAR_STR_UNNAMED] = "<unnamed>";
    parcat[PAR_STR_UNNAMED] = CAT_STRING;
    parname[PAR_STR_LIST] = "<unnamed>";
    parcat[PAR_STR_LIST] = CAT_STRING;
    parname[PAR_SNAP_TO] = "snap_to";
    parcat[PAR_SNAP_TO] = CAT_DOUBLE;
    parname[PAR_COUNT] = "count";
    parcat[PAR_COUNT] = CAT_SHORT;
    parname[PAR_CHECK_LEVELS] = "check_levels";
    parcat[PAR_CHECK_LEVELS] = CAT_STATE;
    parname[PAR_MSG] = "message";
    parcat[PAR_MSG] = CAT_STRING;
    parname[PAR_FORCE_ABORT] = "force_abort";
    parcat[PAR_FORCE_ABORT] = CAT_STATE;
    parname[PAR_MAXPEAKS] = "max_peaks";
    parcat[PAR_MAXPEAKS] = CAT_SHORT;
    parname[PAR_REPEAT] = "repeat";
    parcat[PAR_REPEAT] = CAT_SHORT;
    parname[PAR_REPEAT_STRIPE] = "repeat_stripe";
    parcat[PAR_REPEAT_STRIPE] = CAT_SHORT;
    parname[PAR_COPYFROM] = "from";
    parcat[PAR_COPYFROM] = CAT_STRING;
    parname[PAR_COPYTO] = "to";
    parcat[PAR_COPYTO] = CAT_STRING;
    parname[PAR_POS] = "pos";
    parcat[PAR_POS] = CAT_DOUBLE;
    parname[PAR_BIAS] = "bias";
    parcat[PAR_BIAS] = CAT_DOUBLE;
}

paramnames::~paramnames()
{
    if (parname)
        delete[] parname;
    if (parcat)
        delete[] parcat;
}

char const* paramnames::get_name(PAR_TYPE id)
{
    if (id >= PAR_FIRST && id < PAR_LAST)
        return parname[id];
    else
        return parname[PAR_FIRST];
}

IOCAT paramnames::get_category(PAR_TYPE id)
{
    if (id >= PAR_FIRST && id < PAR_LAST)
        return parcat[id];
    else
        return parcat[PAR_FIRST];
}

paramnames::PAR_TYPE paramnames::get_type(char const* pname)
{
    for (int i = PAR_FIRST + 1; i < PAR_LAST; i++) {
        if (strcmp(parname[i], pname) == 0)
            return (PAR_TYPE)i;
    }
    return PAR_FIRST;
}

#endif
