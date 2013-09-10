#ifndef OUTPUTNAMES_H
#include "../include/outputnames.h"

outputnames::outputnames() : outname(NULL), outcat(NULL)
{
    outname = new char*[OUT_LAST + 1];
    outcat = new IOCAT[OUT_LAST + 1];
    outname[OUT_FIRST] = "BAD_OUTPUT_TYPE";
    outcat[OUT_FIRST] = CAT_FIRST;
    outname[OUT_NONE_DOUBLE] = "off";
    outcat[OUT_NONE_DOUBLE] = CAT_DOUBLE;
    outname[OUT_NONE_SHORT] = "off";
    outcat[OUT_NONE_SHORT] = CAT_SHORT;
    outname[OUT_NONE_ULONG] = "off";
    outcat[OUT_NONE_ULONG] = CAT_ULONG;
    outname[OUT_NONE_TRIG] = "off";
    outcat[OUT_NONE_TRIG] = CAT_TRIG;
    outname[OUT_NONE_STATE] = "off";
    outcat[OUT_NONE_STATE] = CAT_STATE;
    outname[OUT_OUTPUT] = "out_output";
    outcat[OUT_OUTPUT] = CAT_DOUBLE;
    outname[OUT_PLAY_STATE] = "out_play_state";
    outcat[OUT_PLAY_STATE] = CAT_STATE;
    outname[OUT_LEFT] = "out_left";
    outcat[OUT_LEFT] = CAT_SHORT;
    outname[OUT_RIGHT] = "out_right";
    outcat[OUT_RIGHT] = CAT_SHORT;
    outname[OUT_TRIG] = "out_trig";
    outcat[OUT_TRIG] = CAT_TRIG;
    outname[OUT_PHASE_TRIG] = "out_phase_trig";
    outcat[OUT_PHASE_TRIG] = CAT_TRIG;
    outname[OUT_PREMOD_DEG_SIZE] = "out_premod_degsize";
    outcat[OUT_PREMOD_DEG_SIZE] = CAT_DOUBLE;
    outname[OUT_DEG_SIZE] = "out_deg_size";
    outcat[OUT_DEG_SIZE] = CAT_DOUBLE;
    outname[OUT_FREQ] = "out_frequency";
    outcat[OUT_FREQ] = CAT_DOUBLE;
    outname[OUT_NOTE_ON_TRIG] = "out_note_on_trig";
    outcat[OUT_NOTE_ON_TRIG] = CAT_TRIG;
    outname[OUT_NOTE_SLIDE_TRIG] = "out_note_slide_trig";
    outcat[OUT_NOTE_SLIDE_TRIG] = CAT_TRIG;
    outname[OUT_NOTE_OFF_TRIG] = "out_note_off_trig";
    outcat[OUT_NOTE_OFF_TRIG] = CAT_TRIG;
    outname[OUT_RIFF_START_TRIG] = "out_riff_start_trig";
    outcat[OUT_RIFF_START_TRIG] = CAT_TRIG;
    outname[OUT_RIFF_END_TRIG] = "out_riff_end_trig";
    outcat[OUT_RIFF_END_TRIG] = CAT_TRIG;
    outname[OUT_START_TRIG] = "out_start_trig";
    outcat[OUT_START_TRIG] = CAT_TRIG;
    outname[OUT_END_TRIG] = "out_end_trig";
    outcat[OUT_END_TRIG] = CAT_TRIG;
    outname[OUT_VELOCITY] = "out_velocity";	
    outcat[OUT_VELOCITY] = CAT_DOUBLE;
    outname[OUT_TOTAL_SAMPLE_POS] = "out_total_sample_pos";
    outcat[OUT_TOTAL_SAMPLE_POS] = CAT_ULONG;
    outname[OUT_RIFF_PLAY_STATE] = "out_riff_play_state";
    outcat[OUT_RIFF_PLAY_STATE] = CAT_STATE;
    outname[OUT_NOTE_PLAY_STATE] = "out_note_play_state";
    outcat[OUT_NOTE_PLAY_STATE] = CAT_STATE;
    outname[OUT_OFF_PULSE] = "out_pulse_off_trig";
    outcat[OUT_OFF_PULSE] = CAT_TRIG;
    outname[OUT_L] = "out_l";
    outcat[OUT_L] = CAT_DOUBLE;
    outname[OUT_R] = "out_r";
    outcat[OUT_R] = CAT_DOUBLE;
    outname[OUT_WRITE_STATE] = "out_file_write_state";
    outcat[OUT_WRITE_STATE] = CAT_STATE;
    outname[OUT_WRITE_START_TRIG] = "out_write_start_trig";
    outcat[OUT_WRITE_START_TRIG] = CAT_TRIG;
    outname[OUT_WRITE_END_TRIG] = "out_write_end_trig";
    outcat[OUT_WRITE_END_TRIG] = CAT_TRIG;
    outname[OUT_PRE_AMP_MOD] = "out_pre_amp_mod";
    outcat[OUT_PRE_AMP_MOD] = CAT_DOUBLE;
    outname[OUT_NOT_TRIG] = "out_not_trig";
    outcat[OUT_NOT_TRIG] = CAT_TRIG;
    //wcnt 1.1001
    outname[OUT_BPM] = "out_bpm";
    outcat[OUT_BPM] = CAT_DOUBLE;
    outname[OUT_BAR] = "out_bar";
    outcat[OUT_BAR] = CAT_SHORT;
    outname[OUT_BAR_TRIG] = "out_bar_trig";
    outcat[OUT_BAR_TRIG] = CAT_TRIG;
    outname[OUT_POS_IN_BAR] = "out_pos_in_bar"; 
    outcat[OUT_POS_IN_BAR] = CAT_ULONG;
    outname[OUT_POS_STEP_SIZE] = "out_pos_step_size";
    outcat[OUT_POS_STEP_SIZE] = CAT_DOUBLE;
    outname[OUT_SAMPLE_TOTAL] = "out_sample_total";	
    outcat[OUT_SAMPLE_TOTAL] = CAT_ULONG;
    outname[OUT_SAMPLE_IN_BAR] = "out_sample_in_bar";
    outcat[OUT_SAMPLE_IN_BAR] = CAT_ULONG;
    outname[OUT_BEATS_PER_BAR] = "out_beats_per_bar";
    outcat[OUT_BEATS_PER_BAR] = CAT_SHORT;
    outname[OUT_BEAT_VALUE] = "out_beat_value";
    outcat[OUT_BEAT_VALUE] = CAT_SHORT;
    outname[OUT_BPM_CHANGE_TRIG] = "out_bpm_change_trig";
    outcat[OUT_BPM_CHANGE_TRIG] = CAT_TRIG;
    outname[OUT_METER_CHANGE_TRIG]="out_meter_change_trig";
    outcat[OUT_METER_CHANGE_TRIG] = CAT_TRIG;
    outname[OUT_BPM_CHANGE_STATE] = "out_bpm_change_state";
    outcat[OUT_BPM_CHANGE_STATE] = CAT_STATE;
    outname[OUT_LOOP_TRIG] = "out_loop_trig";
    outcat[OUT_LOOP_TRIG] = CAT_TRIG;
    outname[OUT_NOTENAME] = "out_notename";
    outcat[OUT_NOTENAME] = CAT_STRING;
    outname[OUT_VELOCITY_RAMP] = "out_velocity_ramp";
    outcat[OUT_VELOCITY_RAMP] = CAT_DOUBLE;
    outname[OUT_OFF_TRIG] = "out_off_trig";
    outcat[OUT_OFF_TRIG] = CAT_TRIG;
    outname[OUT_MONO] = "out_mono"; 
    outcat[OUT_MONO] = CAT_SHORT;
    outname[OUT_WET_OUTPUT] = "out_wet_output"; 
    outcat[OUT_WET_OUTPUT] = CAT_DOUBLE;
    outname[OUT_RMS] = "out_rms";
    outcat[OUT_RMS] = CAT_DOUBLE;
    outname[OUT_M] = "out_m";
    outcat[OUT_M] = CAT_DOUBLE;
    outname[OUT_RISE_TRIG] = "out_rise_trig";
    outcat[OUT_RISE_TRIG] = CAT_TRIG;
    outname[OUT_FALL_TRIG] = "out_fall_trig";
    outcat[OUT_FALL_TRIG] = CAT_TRIG;
    outname[OUT_TRANSPOSE] = "out_transpose";
    outcat[OUT_TRANSPOSE] = CAT_SHORT;
    outname[OUT_NOVALUE] = "out_no_value";
    outcat[OUT_NOVALUE] = CAT_DOUBLE;
    outname[OUT_NSVALUE] = "out_ns_value";
    outcat[OUT_NSVALUE] = CAT_DOUBLE;
    outname[OUT_NOT_NO_TRIG] = "out_not_no_trig";
    outcat[OUT_NOT_NO_TRIG] = CAT_TRIG;
    outname[OUT_NOT_NS_TRIG] = "out_not_ns_trig";
    outcat[OUT_NOT_NS_TRIG] = CAT_TRIG;
    outname[OUT_COUNT] = "out_count";
    outcat[OUT_COUNT] = CAT_SHORT;
    outname[OUT_WAIT_STATE] = "out_wait_state";
    outcat[OUT_WAIT_STATE] = CAT_STATE;
    outname[OUT_ATTACK_STATE] = "out_attack_state";
    outcat[OUT_ATTACK_STATE] = CAT_STATE;
    outname[OUT_RELEASE_STATE] = "out_release_state";
    outcat[OUT_RELEASE_STATE] = CAT_STATE;
/* These were intended for sampler2 module which uses inputs/outputs
   instead of parameters, but it has not sprouted into existence yet.
   still, look at all the new ones instead...burp.
    outname[OUT_PLAY_DIR] = "out_play_dir";
    outcat[OUT_PLAY_DIR] = CAT_PLAY_DIR;
    outname[OUT_WAVFILE] = "out_wavfilein";
    outcat[OUT_WAVFILE] = CAT_WAVFILEIN;
    outname[OUT_LOOP1_MODE] = "out_loop1_mode";
    outcat[OUT_LOOP1_MODE] = CAT_LOOP_MODE;
    outname[OUT_LOOP2_MODE] = "out_loop2_mode";
    outcat[OUT_LOOP2_MODE] = CAT_LOOP_MODE;
    outname[OUT_NOTE_OFF_MODE] = "out_note_off_mode";
    outcat[OUT_NOTE_OFF_MODE] = CAT_NOTE_OFF_MODE;
    outname[OUT_ST_SAMPLE] = "out_start_sample";
    outcat[OUT_ST_SAMPLE] = CAT_ULONG;
    outname[OUT_END_SAMPLE] = "out_end_sample";
    outcat[OUT_END_SAMPLE] = CAT_ULONG;
    outname[OUT_LOOP1_ST_SAMPLE] = "out_lp1_st_samp";
    outcat[OUT_LOOP1_ST_SAMPLE] = CAT_ULONG;
    outname[OUT_LOOP1_END_SAMPLE] = "out_lp1_end_samp";
    outcat[OUT_LOOP1_END_SAMPLE] = CAT_ULONG;
    outname[OUT_LOOP2_ST_SAMPLE] = "out_lp2_st_samp";
    outcat[OUT_LOOP2_ST_SAMPLE] = CAT_ULONG;
    outname[OUT_LOOP2_END_SAMPLE] = "out_lp2_end_samp";
    outcat[OUT_LOOP2_END_SAMPLE] = CAT_ULONG;*/
}

outputnames::~outputnames()
{
    if (outname)
        delete[] outname;
    if (outcat)
        delete[] outcat;
}

char const* outputnames::get_name(OUT_TYPE id) const
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return outname[id];
    else
        return outname[OUT_FIRST];
}

IOCAT outputnames::get_category(OUT_TYPE id) const
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return outcat[id];
    else
        return outcat[OUT_FIRST];
}

outputnames::OUT_TYPE outputnames::get_type(char const* oname) const
{
    int i;
    for (i = OUT_FIRST + 1; i < OUT_LAST; i++)
        if (strcmp(outname[i], oname) == 0)
            return (OUT_TYPE)i;
    return OUT_FIRST;
}

outputnames::OUT_TYPE outputnames::get_nonezerotype(IOCAT iocat){
    OUT_TYPE ot;
    switch(iocat)
    {
    case CAT_DOUBLE:
        ot = OUT_NONE_DOUBLE;
        break;
    case CAT_SHORT:
        ot = OUT_NONE_SHORT;
        break;
    case CAT_ULONG:
        ot = OUT_NONE_ULONG;
        break;
    case CAT_TRIG:
        ot = OUT_NONE_TRIG;
        break;
    case CAT_STATE:
        ot = OUT_NONE_STATE;
        break;
    default:
        ot = OUT_FIRST;
    }
    return ot;
}

#endif
