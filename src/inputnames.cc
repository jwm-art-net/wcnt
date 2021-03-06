#include "../include/inputnames.h"

namespace input
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,             "BAD_INPUT_TYPE",       iocat::ERR_TYPE,"INPUT ERROR"   },
    { IN_VELOCITY,          "in_velocity",          iocat::DOUBLE,  "Velocity input, value range -1.0 ~ +1.0."  },
    { IN_NOTE_ON_TRIG,      "in_note_on_trig",      iocat::TRIG,    "Trigger note-on. " },
    { IN_NOTE_SLIDE_TRIG,   "in_note_slide_trig",   iocat::TRIG,    "Trigger note-slide (portamento)." },
    { IN_NOTE_OFF_TRIG,     "in_note_off_trig",     iocat::TRIG,    "Trigger note-off. " },
    { IN_LEFT,              "in_left",              iocat::DOUBLE,  "Left signal input." },
    { IN_RIGHT,             "in_right",             iocat::DOUBLE,  "Right signal input." },
    { IN_FREQ,              "in_frequency",         iocat::DOUBLE,  "Frequency input." },
    { IN_FREQ_MOD1,         "in_freq_mod1",         iocat::DOUBLE,  "1st frequency modulation input. " },
    { IN_FREQ_MOD2,         "in_freq_mod2",         iocat::DOUBLE,  "2nd frequency modulation input." },
    { IN_PHASE_TRIG,        "in_phase_trig",        iocat::TRIG,    "Triggers a hard reset of phase. " },
    { IN_PHASE_STEP,        "in_phase_step",        iocat::DOUBLE,  "Amount in radians to step through the phase per sample. "},
    { IN_SIGNAL,            "in_signal",            iocat::DOUBLE,  "The input signal." },
    { IN_EG,                "in_amp_eg",            iocat::DOUBLE,  "Amplitude Envelope Generator (ie adsr) to apply to the signal, -1.0 ~ +1.0. "},
    { IN_AMP_MOD,           "in_amp_mod",           iocat::DOUBLE,  "Amplitude modulation input." },
    { IN_PAN_MOD,           "in_pan_mod",           iocat::DOUBLE,  "Panning modulation input." },
    { IN_TRIG,              "in_trig",              iocat::TRIG,    "Trigger input." },
    { IN_WAVE,              "in_waveform",          iocat::DOUBLE,  "Waveform input." },
    { IN_PWM,               "in_pwm",               iocat::DOUBLE,  "Pulse Width Modulation input." },
    { IN_CUTOFF_PHASE_STEP, "in_cutoff_phase_step", iocat::DOUBLE,  "Input providing the phase step of the desired cut-off frequency." },
    { IN_PLAY_TRIG,         "in_play_trig",         iocat::TRIG,    "Triggers play." },
    { IN_START_POS_MOD,     "in_start_pos_mod",     iocat::DOUBLE,  "Modulation input for the start position." },
    { IN_NORM_MOD,          "in_normal_freq_mod",   iocat::DOUBLE,  "Modulation input for the 'normal' frequency." },
    { IN_V_MOD,             "in_level_mod",         iocat::DOUBLE,  "Output level (V) modulation." },
    { IN_H_MOD,             "in_time_mod",          iocat::DOUBLE,  "Output time (H) modulation." },
    { IN_TRIG1,             "in_trig1",             iocat::TRIG,    "1st trigger input." },
    { IN_TRIG2,             "in_trig2",             iocat::TRIG,    "2nd trigger input." },
    { IN_BAR,               "in_bar",               iocat::WCINT_T,   "Input for bar number." },
    { IN_BAR_TRIG,          "in_bar_trig",          iocat::TRIG,    "Input to trigger the start of a new bar." },
    { IN_POS_STEP_SIZE,     "in_pos_step_size",     iocat::DOUBLE,  "Input for stepping through position in bar per sample, TPQN dependent." },
    { IN_WRITE_TRIG,        "in_write_trig",        iocat::TRIG,    "Trigger to start writing." },
    { IN_STOP_TRIG,         "in_stop_trig",         iocat::TRIG,    "Trigger to stop." },
    { IN_BPM,               "in_bpm",               iocat::DOUBLE,  "BPM input." },
    { IN_POWER_MOD,         "in_power_mod",         iocat::DOUBLE,  "Power modulation input." },
    { IN_RUDE_SWITCH_TRIG,  "in_rude_switch_trig",  iocat::TRIG,    "Trigger to toggle rude mode ON/OFF." },
    { IN_NOTENAME,          "in_notename",          iocat::STRING,  "Note name input." },
    { IN_GAIN_MOD,          "in_gain_mod",          iocat::DOUBLE,  "Gain modulation input." },
    { IN_FEEDBACK,          "in_feedback",          iocat::DOUBLE,  "Feedback input (hint: has to originate from same module somewhere along the line.)" },
    { IN_FB_MOD,            "in_feedback_mod",      iocat::DOUBLE,  "Feedback modulation input." },
    { IN_BEATS_PER_BAR,     "in_beats_per_bar",     iocat::WCINT_T,   "Beats per bar of current time signature." },
    { IN_BEAT_VALUE,        "in_beat_value",        iocat::WCINT_T,   "Beat value of current time signature." },
    { IN_MODULATION,        "in_modulation",        iocat::DOUBLE,  "Modulation source. "},
    { IN_DETRANSPOSE,       "in_detranspose",       iocat::WCINT_T,   "Input providing value to say how much notes were transposed." },
    { IN_STATE,             "in_state",             iocat::STATE,   "ON or OFF." },
    { IN_PLAY_STATE,        "in_play_state",        iocat::STATE,   "ON or OFF." },
    { IN_RESTART_TRIG,      "in_restart_trig",      iocat::TRIG,    "Hard restart trigger." },
    { IN_RESET_TRIG,        "in_reset_trig",        iocat::TRIG,    "Trigger to reset module." },
    { IN_ALL_OFF_TRIG,      "in_all_off_trig",      iocat::TRIG,    "Trigger to turn all running events off, ie notes." },
    { IN_INDEX,             "in_index",             iocat::WCINT_T, "Index input." },
    { IN_BIAS,              "in_bias",              iocat::DOUBLE,  "Bias input." },
    { IN_RES_MOD,           "in_res_mod",           iocat::DOUBLE,  "Resonance modulation input." },
    { IN_BANDWIDTH_MOD,     "in_bandwidth_mod",     iocat::DOUBLE,  "Bandwidth modulation input." },
    { IN_SHIFT_MOD,         "in_shift_mod",         iocat::DOUBLE,  "Modulation of shifting amount. "},
    { IN_SHAPE_PHASE_STEP,  "in_shape_phase_step",  iocat::DOUBLE,  "Phase step input." },
    { IN_SIGNAL1,           "in_signal1",           iocat::DOUBLE,  "1st signal." },
    { IN_SIGNAL2,           "in_signal2",           iocat::DOUBLE,  "2nd signal." },
    { IN_INSP_DF,           "_in_float_",           iocat::DOUBLE,  "Floating point input."  },
    { IN_INSP_DF_TRIG,      "_in_float_trig_",      iocat::TRIG,    "Trigger to display value of floating point input." },
    { IN_INSP_SI,           "_in_int_",             iocat::WCINT_T,   "Integer input." },
    { IN_INSP_SI_TRIG,      "_in_int_trig_",        iocat::TRIG,    "Trigger to display value of integer input." },
    { IN_INSP_UL,           "_in_sample_",          iocat::SAMP_T,  "Long integer input." },
    { IN_INSP_UL_TRIG,      "_in_sample_trig_",     iocat::TRIG,    "Trigger to display value of long integer input." },
    { IN_INSP_ST,           "_in_state_",           iocat::STATE,   "State input." },
    { IN_INSP_ST_TRIG,      "_in_state_trig_",      iocat::TRIG,    "Trigger to display state input." },
    { IN_INSP_STR,          "_in_string_",          iocat::STRING,  "String input." },
    { IN_INSP_STR_TRIG,     "_in_string_trig_",     iocat::TRIG,    "Trigger to display text of string input." },
 };
}; // namespace input
