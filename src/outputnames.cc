#include "../include/outputnames.h"

#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

outputnames::outputnames()
{
#ifdef DEBUG
    for (int i = OUT_FIRST; i < OUT_LAST; i++){
        if (data[i].type != i) {
            std::cout << "\n***** outputnames error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

#ifdef DEBUG
static outputnames onames;
#endif


const char* outputnames::get_name(int id)
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return data[id].name;
    else
        return data[OUT_FIRST].name;
}

iocat::IOCAT outputnames::get_category(int id)
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return data[id].cat;
    else
        return data[OUT_FIRST].cat;
}

outputnames::OUT_TYPE outputnames::get_type(const char* const oname)
{
    for (int i = OUT_FIRST + 1; i < OUT_LAST; i++)
        if (strcmp(data[i].name, oname) == 0)
            return (OUT_TYPE)i;
    return OUT_FIRST;
}

outputnames::OUT_TYPE
outputnames::get_nonezerotype(iocat::IOCAT iocat)
{
    OUT_TYPE ot;
    switch(iocat)
    {
    case iocat::DOUBLE:
        ot = OUT_NONE_DOUBLE;
        break;
    case iocat::SHORT:
        ot = OUT_NONE_SHORT;
        break;
    case iocat::ULONG:
        ot = OUT_NONE_ULONG;
        break;
    case iocat::TRIG:
        ot = OUT_NONE_TRIG;
        break;
    case iocat::STATE:
        ot = OUT_NONE_STATE;
        break;
    case iocat::STRING:
        ot = OUT_NONE_STRING;
        break;
    default:
        ot = OUT_FIRST;
    }
    return ot;
}

const outputnames::output_data outputnames::data[OUT_LAST] =
{
    { OUT_FIRST,            "BAD_OUTPUT_TYPE",      iocat::FIRST,   "ERROR!"   },
    { OUT_NONE_DOUBLE,      "off",                  iocat::DOUBLE,  "Input turned off"  },
    { OUT_NONE_SHORT,       "off",                  iocat::SHORT,   "Input turned off"  },
    { OUT_NONE_ULONG,       "off",                  iocat::ULONG,   "Input turned off"  },
    { OUT_NONE_TRIG,        "off",                  iocat::TRIG,    "Input turned off"  },
    { OUT_NONE_STATE,       "off",                  iocat::STATE,   "Input turned off"  },
    { OUT_NONE_STRING,      "off",                  iocat::STATE,   "Input turned off"  },
    { OUT_OUTPUT,           "out_output",           iocat::DOUBLE,  "The main output fulfilling the purpose of the module. Value range depends on context."  },
    { OUT_PLAY_STATE,       "out_play_state",       iocat::STATE,   "Outputs ON when playing, OFF otherwise."   },
    { OUT_LEFT,             "out_left",             iocat::DOUBLE,  "Pairs with out_right, expected value range of -1.0 ~ +1.0."  },
    { OUT_RIGHT,            "out_right",            iocat::DOUBLE,  "Pairs with out_left, expected value range of -1.0 ~ +1.0."  },
    { OUT_TRIG,             "out_trig",             iocat::TRIG,    "The main trigger output of the module. When triggered outputs ON for a single sample, before reverting to OFF." },
    { OUT_PHASE_TRIG,       "out_phase_trig",       iocat::TRIG,    "Outputs a trigger when a module has reached it's full phase (for example a waveform module)." },
    { OUT_PREMOD_PHASE_STEP,"out_premod_phase_step",iocat::DOUBLE,  "The phase step value before modulation has been applied to it." },
    { OUT_PHASE_STEP,       "out_phase_step",       iocat::DOUBLE,  "Phase step is the amount (in radians) a module progresses through it's phase per sample." },
    { OUT_FREQ,             "out_frequency",        iocat::DOUBLE,  "Outputs a frequency value in HRTZ." },
    { OUT_NOTE_ON_TRIG,     "out_note_on_trig",     iocat::TRIG,    "Triggers when note-on occurs." },
    { OUT_NOTE_SLIDE_TRIG,  "out_note_slide_trig",  iocat::TRIG,    "Triggers when portamento might/should be applied." },
    { OUT_NOTE_OFF_TRIG,    "out_note_off_trig",    iocat::TRIG,    "Triggers when note-off occurs."    },
    { OUT_RIFF_START_TRIG,  "out_riff_start_trig",  iocat::TRIG,    "Triggers at the beginning of a riff of notes." },
    { OUT_RIFF_END_TRIG,    "out_riff_end_trig",    iocat::TRIG,    "Triggers at the end of a riff of notes." },
    { OUT_START_TRIG,       "out_start_trig",       iocat::TRIG,    "Triggers when something begins." },
    { OUT_END_TRIG,         "out_end_trig",         iocat::TRIG,    "Triggers when something ends." },
    { OUT_VELOCITY,         "out_velocity",         iocat::DOUBLE,  "Velocity output, usual range 0.0 ~ 1.0, but dependant on sensible values specified in note_data."  },
    { OUT_TOTAL_SAMPLE_POS, "out_total_sample_pos", iocat::ULONG,   "The number of samples processed in total." },
    { OUT_RIFF_PLAY_STATE,  "out_riff_play_state",  iocat::STATE,   "ON when a riff is playing, otherwise OFF." },
    { OUT_NOTE_PLAY_STATE,  "out_note_play_state",  iocat::STATE,   "ON when a note is playing, otherwise OFF." },
    { OUT_OFF_PULSE,        "out_pulse_off_trig",   iocat::TRIG,    "Triggers when a pulse finishes (ie twice per cycle of a square wave)." },
    { OUT_WRITE_STATE,      "out_file_write_state", iocat::STATE,   "ON when a file is being written to." },
    { OUT_WRITE_START_TRIG, "out_write_start_trig", iocat::TRIG,    "Triggers when file writing begins." },
    { OUT_WRITE_END_TRIG,   "out_write_end_trig",   iocat::TRIG,    "Triggers when file writing ends." },
    { OUT_PRE_AMP_MOD,      "out_pre_amp_mod",      iocat::DOUBLE,  "Amplitude value before modulation." },
    { OUT_NOT_TRIG,         "out_not_trig",         iocat::TRIG,    "Triggers when the input trigger was ignored by an out_trig trigger." },
    //wcnt 1.1001
    { OUT_BPM,              "out_bpm",              iocat::DOUBLE,  "Outputs a BPM value." },
    { OUT_BAR,              "out_bar",              iocat::SHORT,   "Outputs the bar number, starting at zero." },
    { OUT_BAR_TRIG,         "out_bar_trig",         iocat::TRIG,    "Triggers when a new bar begins." },
    { OUT_POS_IN_BAR,       "out_pos_in_bar",       iocat::ULONG,   "Position in a bar, relative to TPQN (ticks per quarter note)." },
    { OUT_POS_STEP_SIZE,    "out_pos_step_size",    iocat::DOUBLE,  "Amount per sample to increase the bar position, relative to TPQN." },
    { OUT_SAMPLE_TOTAL,     "out_sample_total",     iocat::ULONG,   "Total number of samples processed." },
    { OUT_SAMPLE_IN_BAR,    "out_sample_in_bar",    iocat::ULONG,   "Number of samples processed in the current bar." },
    { OUT_BEATS_PER_BAR,    "out_beats_per_bar",    iocat::SHORT,   "Outputs the beats per bar of the current time signature." },
    { OUT_BEAT_VALUE,       "out_beat_value",       iocat::SHORT,   "Outputs the current beat value of time signature." },
    { OUT_BPM_CHANGE_TRIG,  "out_bpm_change_trig",  iocat::TRIG,    "Triggers whenever a BPM change is reached (not at every sample when the tempo is ramped)." },
    { OUT_METER_CHANGE_TRIG,"out_meter_change_trig",iocat::TRIG,    "Triggers when a time signature change occurs." },
    { OUT_BPM_CHANGE_STATE, "out_bpm_change_state", iocat::STATE,   "Is ON when the BPM is changing." },
    { OUT_LOOP_TRIG,        "out_loop_trig",        iocat::TRIG,    "Triggers when a loop point is reached." },
    { OUT_NOTENAME,         "out_notename",         iocat::STRING,  "The name of a note to be processed." },
    { OUT_VELOCITY_RAMP,    "out_velocity_ramp",    iocat::DOUBLE,  "Velocity output with some smoothing applied." },
    { OUT_OFF_TRIG,         "out_off_trig",         iocat::TRIG,    "Triggers when something turns off. "},
    { OUT_WET_OUTPUT,       "out_wet_output",       iocat::DOUBLE,  "The wet output of processed effects, none of the unprocessed dry signal remains." },
    { OUT_RMS,              "out_rms",              iocat::DOUBLE,  "Outputs the Root Mean Square of a signal." },
    { OUT_RISE_TRIG,        "out_rise_trig",        iocat::TRIG,    "Triggers when a signal value rises." },
    { OUT_FALL_TRIG,        "out_fall_trig",        iocat::TRIG,    "Triggers when a signal value falls." },
    { OUT_TRANSPOSE,        "out_transpose",        iocat::SHORT,   "The amount of transpose applied to a riff." },
    { OUT_NOVALUE,          "out_no_value",         iocat::DOUBLE,  "Outputs the value calculated when triggered by note-on." },
    { OUT_NSVALUE,          "out_ns_value",         iocat::DOUBLE,  "Outputs the value calculated when triggered by a note-slide (for portamento)." },
    { OUT_NOT_NO_TRIG,      "out_not_no_trig",      iocat::TRIG,    "Triggers when the note-on note was outside the translation range." },
    { OUT_NOT_NS_TRIG,      "out_not_ns_trig",      iocat::TRIG,    "Triggers when the note-slide note was outside the translation range." },
    { OUT_COUNT,            "out_count",            iocat::SHORT,   "Outputs a count of events, ie triggers." },
    { OUT_WAIT_STATE,       "out_wait_state",       iocat::STATE,   "ON when waiting, otherwise OFF." },
    { OUT_ATTACK_STATE,     "out_attack_state",     iocat::STATE,   "ON when the attack section is active." },
    { OUT_RELEASE_STATE,    "out_release_state",    iocat::STATE,   "ON when the release section is active." },
    { OUT_X,                "out_x",                iocat::DOUBLE,  "X position calculated." },
    { OUT_Y,                "out_y",                iocat::DOUBLE,  "Y position calculated." },
    #ifdef WITH_LADSPA
    { OUT_UP,               "out_up",               iocat::DOUBLE,  "Outputs the input signal frequency shifted upwards." },
    { OUT_DOWN,             "out_down",             iocat::DOUBLE,  "Outputs the input signal shifted down in frequency." },
    #endif
    { OUT_PRE_COUNT,        "out_pre_count",        iocat::SHORT,   "Where in the pre-count stage we are." },
    { OUT_PRE_SHAPE_OUTPUT, "out_pre_shape_output", iocat::DOUBLE,  "Output before shaping applied." },
    { OUT_THROUGH,          "out_through",          iocat::DOUBLE,  "Input signal passed straight through to output." }
};

