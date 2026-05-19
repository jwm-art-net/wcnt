#include "../include/inputnames.h"

namespace input
{

 iocat::TYPE names::get_off_category_for_input(TYPE it)
 {
    switch(it)
    {
       case IN_EG:
          return iocat::DOUBLE1;
       default:
          return data[chk(it)].cat;
    }
 }


 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,             "BAD_INPUT_TYPE",       iocat::ERR_TYPE,"INPUT ERROR",                                  output::ERR_TYPE   },
    { IN_VELOCITY,          "in_velocity",          iocat::DOUBLE,  "Velocity input, value range -1.0 ~ +1.0.",     output::OUT_VELOCITY },
    { IN_NOTE_ON_TRIG,      "in_note_on_trig",      iocat::TRIG,    "Trigger note-on. ",                            output::OUT_NOTE_ON_TRIG },
    { IN_NOTE_SLIDE_TRIG,   "in_note_slide_trig",   iocat::TRIG,    "Trigger note-slide (portamento).",             output::OUT_NOTE_SLIDE_TRIG },
    { IN_NOTE_OFF_TRIG,     "in_note_off_trig",     iocat::TRIG,    "Trigger note-off. ",                           output::OUT_NOTE_OFF_TRIG },
    { IN_LEFT,              "in_left",              iocat::DOUBLE,  "Left signal input.",       output::OUT_LEFT },
    { IN_RIGHT,             "in_right",             iocat::DOUBLE,  "Right signal input.",      output::OUT_RIGHT },
    { IN_FREQ,              "in_frequency",         iocat::DOUBLE,  "Frequency input.",         output::OUT_FREQ },
    { IN_FREQ_MOD1,         "in_freq_mod1",         iocat::DOUBLE,  "1st frequency modulation input. ",     output::OUT_OUTPUT },
    { IN_FREQ_MOD2,         "in_freq_mod2",         iocat::DOUBLE,  "2nd frequency modulation input.",      output::OUT_OUTPUT },
    { IN_PHASE_TRIG,        "in_phase_trig",        iocat::TRIG,    "Triggers a hard reset of phase. ",     output::OUT_PHASE_TRIG },
    { IN_PHASE_STEP,        "in_phase_step",        iocat::DOUBLE,  "Amount in radians to step through the phase per sample. ", output::OUT_PHASE_STEP },
    { IN_SIGNAL,            "in_signal",            iocat::DOUBLE,  "The input signal.",                                        output::OUT_OUTPUT },
    { IN_EG,                "in_amp_eg",            iocat::DOUBLE,  "Amplitude Envelope Generator (ie adsr) to apply to the signal, -1.0 ~ +1.0. ", output::OUT_OUTPUT },
    { IN_AMP_MOD,           "in_amp_mod",           iocat::DOUBLE,  "Amplitude modulation input.",  output::OUT_OUTPUT },
    { IN_PAN_MOD,           "in_pan_mod",           iocat::DOUBLE,  "Panning modulation input.",    output::OUT_OUTPUT },
    { IN_TRIG,              "in_trig",              iocat::TRIG,    "Trigger input.",               output::OUT_TRIG },
    { IN_WAVE,              "in_waveform",          iocat::DOUBLE,  "Waveform input.",              output::OUT_OUTPUT },
    { IN_PWM,               "in_pwm",               iocat::DOUBLE,  "Pulse Width Modulation input.",output::OUT_OUTPUT },
    { IN_CUTOFF_PHASE_STEP, "in_cutoff_phase_step", iocat::DOUBLE,  "Input providing the phase step of the desired cut-off frequency.", output::OUT_PHASE_STEP },
    { IN_PLAY_TRIG,         "in_play_trig",         iocat::TRIG,    "Triggers play.",                               output::OUT_TRIG },
    { IN_START_POS_MOD,     "in_start_pos_mod",     iocat::DOUBLE,  "Modulation input for the start position.",     output::OUT_OUTPUT },
    { IN_NORM_MOD,          "in_normal_freq_mod",   iocat::DOUBLE,  "Modulation input for the 'normal' frequency.", output::OUT_OUTPUT },
    { IN_V_MOD,             "in_level_mod",         iocat::DOUBLE,  "Output level (V) modulation.",     output::OUT_OUTPUT },
    { IN_H_MOD,             "in_time_mod",          iocat::DOUBLE,  "Output time (H) modulation.",      output::OUT_OUTPUT },
    { IN_TRIG1,             "in_trig1",             iocat::TRIG,    "1st trigger input.",               output::ERR_TYPE },
    { IN_TRIG2,             "in_trig2",             iocat::TRIG,    "2nd trigger input.",               output::ERR_TYPE },
    { IN_BAR,               "in_bar",               iocat::WCINT_T, "Input for bar number.",            output::OUT_BAR },
    { IN_BAR_TRIG,          "in_bar_trig",          iocat::TRIG,    "Input to trigger the start of a new bar.", output::OUT_BAR_TRIG },
    { IN_POS_STEP_SIZE,     "in_pos_step_size",     iocat::DOUBLE,  "Input for stepping through position in bar per sample, TPQN dependent.",   output::OUT_POS_STEP_SIZE },
    { IN_WRITE_TRIG,        "in_write_trig",        iocat::TRIG,    "Trigger to start writing.",            output::ERR_TYPE },
    { IN_STOP_TRIG,         "in_stop_trig",         iocat::TRIG,    "Trigger to stop.",                     output::ERR_TYPE },
    { IN_BPM,               "in_bpm",               iocat::DOUBLE,  "BPM input.",                           output::OUT_BPM },
    { IN_POWER_MOD,         "in_power_mod",         iocat::DOUBLE,  "Power modulation input.",              output::OUT_OUTPUT },
    { IN_RUDE_SWITCH_TRIG,  "in_rude_switch_trig",  iocat::TRIG,    "Trigger to toggle rude mode ON/OFF.",  output::OUT_TRIG },
    { IN_NOTENAME,          "in_notename",          iocat::STRING,  "Note name input.",             output::OUT_NOTENAME },
    { IN_GAIN_MOD,          "in_gain_mod",          iocat::DOUBLE,  "Gain modulation input.",       output::OUT_OUTPUT },
    { IN_FEEDBACK,          "in_feedback",          iocat::DOUBLE,  "Feedback input (hint: has to originate from same module somewhere along the line.)", output::OUT_WET_OUTPUT },
    { IN_FB_MOD,            "in_feedback_mod",      iocat::DOUBLE,  "Feedback modulation input.",               output::OUT_OUTPUT },
    { IN_BEATS_PER_BAR,     "in_beats_per_bar",     iocat::WCINT_T, "Beats per bar of current time signature.", output::OUT_BEATS_PER_BAR },
    { IN_BEAT_VALUE,        "in_beat_value",        iocat::WCINT_T, "Beat value of current time signature.",    output::OUT_BEAT_VALUE },
    { IN_MODULATION,        "in_modulation",        iocat::DOUBLE,  "Modulation source. ",          output::OUT_OUTPUT},
    { IN_DETRANSPOSE,       "in_detranspose",       iocat::WCINT_T, "Input providing value to say how much notes were transposed.", output::OUT_TRANSPOSE },
    { IN_STATE,             "in_state",             iocat::STATE,   "ON or OFF.",   output::OUT_PLAY_STATE },
    { IN_PLAY_STATE,        "in_play_state",        iocat::STATE,   "ON or OFF.",   output::OUT_PLAY_STATE },
    { IN_RESTART_TRIG,      "in_restart_trig",      iocat::TRIG,    "Hard restart trigger.",    output::OUT_TRIG },
    { IN_RESET_TRIG,        "in_reset_trig",        iocat::TRIG,    "Trigger to reset module.", output::OUT_TRIG },
    { IN_ALL_OFF_TRIG,      "in_all_off_trig",      iocat::TRIG,    "Trigger to turn all running events off, ie notes.", output::ERR_TYPE },
    { IN_INDEX,             "in_index",             iocat::WCINT_T, "Index input.", output::OUT_INDEX },
    { IN_BIAS,              "in_bias",              iocat::DOUBLE,  "Bias input.",  output::OUT_OUTPUT },
    { IN_RES_MOD,           "in_res_mod",           iocat::DOUBLE,  "Resonance modulation input.",      output::OUT_OUTPUT },
    { IN_BANDWIDTH_MOD,     "in_bandwidth_mod",     iocat::DOUBLE,  "Bandwidth modulation input.",      output::OUT_OUTPUT },
    { IN_SHIFT_MOD,         "in_shift_mod",         iocat::DOUBLE,  "Modulation of shifting amount.",   output::OUT_OUTPUT },
    { IN_SHAPE_PHASE_STEP,  "in_shape_phase_step",  iocat::DOUBLE,  "Phase step input.",    output::OUT_PHASE_STEP },
    { IN_SIGNAL1,           "in_signal1",           iocat::DOUBLE,  "1st signal.",  output::ERR_TYPE },
    { IN_SIGNAL2,           "in_signal2",           iocat::DOUBLE,  "2nd signal.",  output::ERR_TYPE },
    { IN_INSP_DF,           "_in_float_",           iocat::DOUBLE,  "Floating point input.",                    output::ERR_TYPE },
    { IN_INSP_DF_TRIG,      "_in_float_trig_",      iocat::TRIG,    "Trigger to display value of floating point input.", output::ERR_TYPE },
    { IN_INSP_SI,           "_in_int_",             iocat::WCINT_T, "Integer input.",                           output::ERR_TYPE },
    { IN_INSP_SI_TRIG,      "_in_int_trig_",        iocat::TRIG,    "Trigger to display value of integer input.", output::ERR_TYPE },
    { IN_INSP_UL,           "_in_sample_",          iocat::SAMP_T,  "Long integer input.",                      output::ERR_TYPE },
    { IN_INSP_UL_TRIG,      "_in_sample_trig_",     iocat::TRIG,    "Trigger to display value of long integer input.", output::ERR_TYPE },
    { IN_INSP_ST,           "_in_state_",           iocat::STATE,   "State input.",                             output::ERR_TYPE },
    { IN_INSP_ST_TRIG,      "_in_state_trig_",      iocat::TRIG,    "Trigger to display state input.",          output::ERR_TYPE },
    { IN_INSP_STR,          "_in_string_",          iocat::STRING,  "String input.",                            output::ERR_TYPE },
    { IN_INSP_STR_TRIG,     "_in_string_trig_",     iocat::TRIG,    "Trigger to display text of string input.", output::ERR_TYPE },
    { IN_PHASE_MOD,         "in_phase_mod",         iocat::DOUBLE,  "Input to modulate phase",                  output::OUT_OUTPUT }
 };
}; // namespace input
