#ifndef INPUTNAMES_H
#include "../include/inputnames.h"

inputnames::inputnames() : inname(NULL), incat(NULL)
{
    inname = new char*[IN_LAST + 1];
    incat = new IOCAT[IN_LAST + 1];
    inname[IN_FIRST] = "BAD_INPUT_TYPE";
    incat[IN_FIRST] = CAT_FIRST;
    inname[IN_VELOCITY] = "in_velocity";
    incat[IN_VELOCITY] = CAT_DOUBLE;
    inname[IN_NOTE_ON_TRIG] = "in_note_on_trig";
    incat[IN_NOTE_ON_TRIG] = CAT_TRIG;
    inname[IN_NOTE_SLIDE_TRIG] = "in_note_slide_trig";
    incat[IN_NOTE_SLIDE_TRIG] = CAT_TRIG;
    inname[IN_NOTE_OFF_TRIG] = "in_note_off_trig";
    incat[IN_NOTE_OFF_TRIG] = CAT_TRIG;
    inname[IN_LEFT] = "in_left";
    incat[IN_LEFT] = CAT_SHORT;
    inname[IN_RIGHT] = "in_right";
    incat[IN_RIGHT] = CAT_SHORT;
    inname[IN_FREQ] = "in_frequency";
    incat[IN_FREQ] = CAT_DOUBLE;
    inname[IN_FREQ_MOD1] = "in_freq_mod1";
    incat[IN_FREQ_MOD1] = CAT_DOUBLE;
    inname[IN_FREQ_MOD2] = "in_freq_mod2";
    incat[IN_FREQ_MOD2] = CAT_DOUBLE;
    inname[IN_PHASE_TRIG] = "in_phase_trig";
    incat[IN_PHASE_TRIG] = CAT_TRIG;
    inname[IN_DEG_SIZE] = "in_deg_size";
    incat[IN_DEG_SIZE] = CAT_DOUBLE;
    inname[IN_SIGNAL] = "in_signal";
    incat[IN_SIGNAL] = CAT_DOUBLE;
    inname[IN_EG] = "in_amp_eg";
    incat[IN_EG] = CAT_DOUBLE;
    inname[IN_AMP_MOD] = "in_amp_mod";
    incat[IN_AMP_MOD] = CAT_DOUBLE;
    inname[IN_PAN_MOD] = "in_pan_mod";
    incat[IN_PAN_MOD] = CAT_DOUBLE;
    inname[IN_TRIG] = "in_trig";
    incat[IN_TRIG] = CAT_TRIG;
    inname[IN_WAVE] = "in_waveform";
    incat[IN_WAVE] = CAT_DOUBLE;
    inname[IN_PWM] = "in_pwm";
    incat[IN_PWM] = CAT_DOUBLE;
    inname[IN_CUTOFF_DEG_SIZE] = "in_cutoff_degsize";
    incat[IN_CUTOFF_DEG_SIZE] = CAT_DOUBLE;
    inname[IN_PLAY_TRIG] = "in_play_trig";
    incat[IN_PLAY_TRIG] = CAT_TRIG;
    inname[IN_START_POS_MOD] = "in_start_pos_mod";
    incat[IN_START_POS_MOD] = CAT_DOUBLE;
    inname[IN_MODIFIER] = "in_modifier";
    incat[IN_MODIFIER] = CAT_DOUBLE;
    inname[IN_NORM_MOD] = "in_normal_freq_mod";
    incat[IN_NORM_MOD] = CAT_DOUBLE;
    inname[IN_V_MOD] = "in_level_mod";
    incat[IN_V_MOD] = CAT_DOUBLE;
    inname[IN_H_MOD] = "in_time_mod";
    incat[IN_H_MOD] = CAT_DOUBLE;
    inname[IN_TRIG1] = "in_trig1";
    incat[IN_TRIG1] = CAT_TRIG;
    inname[IN_TRIG2] = "in_trig2";
    incat[IN_TRIG2] = CAT_TRIG;
    // wcnt 1.1001 :-
    inname[IN_BAR] = "in_bar";
    incat[IN_BAR] = CAT_SHORT;
    inname[IN_BAR_TRIG] = "in_bar_trig";
    incat[IN_BAR_TRIG] = CAT_TRIG;
    inname[IN_POS_STEP_SIZE] = "in_pos_step_size";
    incat[IN_POS_STEP_SIZE] = CAT_DOUBLE;
    inname[IN_WRITE_TRIG] = "in_write_trig";
    incat[IN_WRITE_TRIG] = CAT_TRIG;
    inname[IN_STOP_TRIG] = "in_stop_trig";
    incat[IN_STOP_TRIG] = CAT_TRIG;
    inname[IN_BPM] = "in_bpm";
    incat[IN_BPM] = CAT_DOUBLE;
    inname[IN_POWER_MOD] = "in_power_mod";
    incat[IN_POWER_MOD] = CAT_DOUBLE;
    inname[IN_RUDE_SWITCH_TRIG] = "in_rude_switch_trig";
    incat[IN_RUDE_SWITCH_TRIG] = CAT_TRIG;
    inname[IN_NOTENAME] = "in_notename";
    incat[IN_NOTENAME] = CAT_STRING;
    inname[IN_L] = "in_l";
    incat[IN_L] = CAT_DOUBLE;
    inname[IN_R] = "in_r";
    incat[IN_R] = CAT_DOUBLE;
    inname[IN_GAIN_MOD] = "in_gain_mod";
    incat[IN_GAIN_MOD] = CAT_DOUBLE;
    inname[IN_FEEDBACK] = "in_feedback";
    incat[IN_FEEDBACK] = CAT_DOUBLE;
    inname[IN_FB_MOD] = "in_feedback_mod";
    incat[IN_FB_MOD] = CAT_DOUBLE;
    inname[IN_BEATS_PER_BAR] = "in_beats_per_bar";
    incat[IN_BEATS_PER_BAR] = CAT_SHORT;
    inname[IN_BEAT_VALUE] = "in_beat_value";
    incat[IN_BEAT_VALUE] = CAT_SHORT;
    inname[IN_MODULATION] = "in_modulation";
    incat[IN_MODULATION] = CAT_DOUBLE;
    inname[IN_DETRANSPOSE] = "in_detranspose";
    incat[IN_DETRANSPOSE] = CAT_SHORT;
    inname[IN_STATE] = "in_state";
    incat[IN_STATE] = CAT_STATE;
    inname[IN_PLAY_STATE] = "in_play_state";
    incat[IN_PLAY_STATE] = CAT_STATE;
    inname[IN_RESTART_TRIG] = "in_restart_trig";
    incat[IN_RESTART_TRIG] = CAT_TRIG;
}

inputnames::~inputnames()
{
    if (inname)
        delete[] inname;
    if (incat)
        delete[] incat;
}

const char* inputnames::get_name(IN_TYPE id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return inname[id];
    else
        return inname[IN_FIRST];
}

IOCAT inputnames::get_category(IN_TYPE id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return incat[id];
    else
        return incat[IN_FIRST];
}

inputnames::IN_TYPE inputnames::get_type(const char* iname)
{
    int i;
    for (i = IN_FIRST + 1; i < IN_LAST; i++)
        if (strcmp(inname[i], iname) == 0)
            return (IN_TYPE)i;
    return IN_FIRST;
}

#endif
