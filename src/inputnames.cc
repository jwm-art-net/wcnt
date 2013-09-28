#include "../include/inputnames.h"

#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

inputnames::inputnames()
{
#ifdef DEBUG
    for (int i = IN_FIRST; i < IN_LAST; i++){
        if (data[i].type != i) {
            std::cout << "\n***** inputnames error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

#ifdef DEBUG
static inputnames inames;
#endif


const char* inputnames::get_name(int id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return data[id].name;
    else
        return data[IN_FIRST].name;
}

const char* inputnames::get_descr(int id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return data[id].descr;
    else
        return data[IN_FIRST].descr;
}

iocat::IOCAT inputnames::get_category(int id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return data[id].cat;
    else
        return data[IN_FIRST].cat;
}

inputnames::IN_TYPE inputnames::get_type(const char* const iname)
{
    for (int i = IN_FIRST + 1; i < IN_LAST; i++)
        if (strcmp(data[i].name, iname) == 0)
            return (IN_TYPE)i;
    return IN_FIRST;
}

const inputnames::input_data inputnames::data[IN_LAST] =
{
    { IN_FIRST,             "BAD_INPUT_TYPE",       iocat::FIRST,   "INPUT ERROR"   },
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
    { IN_EG,                "in_amp_eg",            iocat::DOUBLE,  "Amplitude Envelope Generator to apply to the signal, -1.0 ~ +1.0. "},
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
    { IN_BAR,               "in_bar",               iocat::SHORT,   "Input for bar number." },
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
    { IN_BEATS_PER_BAR,     "in_beats_per_bar",     iocat::SHORT,   "Beats per bar of current time signature." },
    { IN_BEAT_VALUE,        "in_beat_value",        iocat::SHORT,   "Beat value of current time signature." },
    { IN_MODULATION,        "in_modulation",        iocat::DOUBLE,  "Modulation source. "},
    { IN_DETRANSPOSE,       "in_detranspose",       iocat::SHORT,   "Input providing value to say how much notes were transposed." },
    { IN_STATE,             "in_state",             iocat::STATE,   "ON or OFF." },
    { IN_PLAY_STATE,        "in_play_state",        iocat::STATE,   "ON or OFF." },
    { IN_RESTART_TRIG,      "in_restart_trig",      iocat::TRIG,    "Hard restart trigger." },
    #ifdef WITH_LADSPA
    { IN_RES_MOD,           "in_res_mod",           iocat::DOUBLE,  "Resonance modulation input." },
    { IN_THRESH_MOD,        "in_thresh_mod",        iocat::DOUBLE,  "Threshold modulation input." },
    { IN_BANDWIDTH_MOD,     "in_bandwidth_mod",     iocat::DOUBLE,  "Bandwidth modulation input." },
    { IN_SHIFT_MOD,         "in_shift_mod",         iocat::DOUBLE,  "Modulation of shifting amount. "},
    { IN_SHAPE_PHASE_STEP,  "in_shape_phase_step",  iocat::DOUBLE,  "Phase step input." },
    { IN_FREQ_MOD,          "in_freq_mod",          iocat::DOUBLE,  "Frequency modulation input." },
    { IN_SIGNAL1,           "in_signal1",           iocat::DOUBLE,  "1st signal." },
    { IN_SIGNAL2,           "in_signal2",           iocat::DOUBLE,  "2nd signal." },
    #endif
    /* inspect module's bs */
    { IN__DF,               "_in_double_float_",      iocat::DOUBLE,    "Floating point input."  },
    { IN__DF_TRIG,          "_in_double_float_trig_", iocat::TRIG,      "Trigger to display value of floating point input." },
    { IN__SI,               "_in_short_int_",         iocat::SHORT,     "Integer input." },
    { IN__SI_TRIG,          "_in_short_int_trig_",    iocat::TRIG,      "Trigger to display value of integer input." },
    { IN__UL,               "_in_unsigned_long_",     iocat::ULONG,     "Long integer input." },
    { IN__UL_TRIG,          "_in_unsigned_long_trig_",iocat::TRIG,      "Trigger to display value of long integer input." },
    { IN__ST,               "_in_state_",             iocat::STATE,     "State input." },
    { IN__ST_TRIG,          "_in_state_trig_",        iocat::TRIG,      "Trigger to display state input." },
    { IN__STR,              "_in_string_",            iocat::STRING,    "String input." },
    { IN__STR_TRIG,         "_in_string_trig_",       iocat::TRIG,      "Trigger to display text of string input." },
    { IN_RESET_TRIG,        "in_reset_trig",        iocat::TRIG,        "Trigger to reset module." },
    { IN_COUNT,             "in_count",             iocat::SHORT,       "Counter input." },
    { IN_BIAS,              "in_bias",              iocat::DOUBLE,      "Bias input." }
};
