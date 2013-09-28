#include "../include/synthmodnames.h"

#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

synthmodnames::synthmodnames()
{
#ifdef DEBUG
    for (int i = FIRST; i < LAST; i++){
        if (data[i].type != i) {
            std::cout << "\n***** synthmodnames error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

#ifdef DEBUG
static synthmodnames smnames;
#endif


const char* synthmodnames::get_name(int id)
{
    if (id > FIRST && id < LAST)
        return data[id].name;
    else
        return data[FIRST].name;
}

synthmodnames::SYNTHMOD_TYPE
synthmodnames::get_type(const char* const mname)
{
    for (int i = FIRST; i < LAST; i++)
        if (strcmp(data[i].name, mname) == 0)
            return (SYNTHMOD_TYPE)i;
    return FIRST;
}

const char* synthmodnames::get_descr(int id)
{
    if (id > FIRST && id < LAST)
        return data[id].descr;
    else
        return data[FIRST].descr;
}


const synthmodnames::mod_data synthmodnames::data[LAST] =
{
    { FIRST,            "BAD_MODULE_TYPE",  "ERROR!"   },
    { NONEZERO,         "none",             "A module providing outputs which are used when an input is turned off."        },
    { ADSR,             "adsr",             "A dual shaped modulatable envelope generator."                                 },
    { STEREOAMP,        "stereo_amp",       "A module used to shape, modulate, and adjust the levels of a pair of signals."          },
    { CLOCK,            "clock",            "A basic clock source providing a phase trigger and step outputs."              },
    { CONSTMOD,         "constant",         "Provides a constant unchanging value."                                         },
    { FREQGEN,          "freq_generator",   "Translate one signal range to another."                                        },
    { LFOCLOCK,         "lfo_clock",        "A clock with phase reset and with dual modulation inputs."                     },
    { LFOCONTROL,       "lfo_controller",   "Designed to shape waveform output to provide LFO functionality."               },
    { MODIFIER,         "modifier",         "Modify the first signal by the second, with bias and a choice of operators."   },
    { NOISEGEN,         "noise_generator",  "Generates a random signal value for every sample."                             },
    { OSCCLOCK,         "osc_clock",        "A clock generally driven by the sequencer module, versatile."                  },
    { SAMPLEHOLD,       "sample_hold",      "Samples a signal and holds the value with optional decay."                                 },
    { SEQUENCER,        "sequencer",        "Sequences and translates riffs of notes along the time line into forms usable by other modules." },
    { SQUAREWAVE,       "square_wave",      "A square waveform implementation with an additional climb-rate parameter."                 },
    { TRIGGER,          "trigger",          "Outputs a trigger when the signal reaches a specific level."                               },
    { TRIWAVE2,         "tri_wave2",        "Triangle waveform with ramps set to a specific frequency."                                 },
    { USERWAVE,         "user_wave",        "User defined waveform with X/Y shape modulation, but lacks smoothing."                     },
    { SAMPLER,          "sampler",          "Reads data from an audio file. Supports granular synthesis style usage." },
    { WAVFILEOUT,       "wavfile_out",      "Writes data to an audio file. Appends a time stamp to filename when snapshot_mode is used." },
    { STEREOCHANNEL,    "mix_chan",         "A module to provide a stereo signal to the mixer module."                                      },
    { STEREOMIXER,      "mixer",            "Mixes any number of stereo signals together. The signals are specified as a list of modules." },
    { RANDTRIGGER,      "rnd_trigger",      "Depending on probability, randomly outputs a trigger when the input triggers."                 },
    { LOGICTRIGGER,     "logic_trigger",    "Uses logic to output a trigger depending on the two (near simultaneous) input triggers."       },
    { SWITCHER,         "switcher",         "When triggered, switches to the next signal. The signals are specified as a list of modules." },
    { WCNTSIGNAL,       "wcnt_signal",      "A connective module which can be added into the switcher, combiner, and spreader modules, etc."     },
    { COMBINER,         "combiner",         "Combines (or mixes) a number of signals together. The signals are specified as a list of modules." },
    { TIMEMAP,          "time_map",         "Manages the timeline, supports ramped tempo changes, and time signature changes."},
    { CONTRASTER,       "contraster",       "Adds 'contrast' to a signal. Optional 'rude' mode provides an 'alternative' implementation" },
    { SERIALWAVFILEOUT, "serial_wavout",    "Writes data to a series of audio files. The current file is closed and a new file opened upon trigger."},
    { DELAY,            "delay",            "Takes a signal and delays it."},
    { ECHO,             "echo",             "Takes a signal and delays it with feedback."},
    { MONOAMP,          "mono_amp",         "A module used to shape, modulate, and adjust the levels of a signal."          },
    { WCNTEXIT,         "wcnt_exit",        "A mandatory module used to stop processing." },
    { MULTIPLIER,       "multiplier",       "Multiplies two signals together." },
    { RANGELIMIT,       "range_limit",      "Limits the upper and lower value bounds of a signal." },
    { PAN,              "panner",           "Positions a signal between a stereo pair, allows modulation." },
    { RMS,              "rms",              "Calculate the RMS of a signal." },
    { DCFILTER,         "dc_filter",        "Filters DC from by subtracting the mean." },
    { DYNAMIC,          "dynamic",          "User defined and modulatable dynamic mapping of signal levels." },
    { SPREADER,         "spreader",         "Dependant on value of input signal, outputs one of a number of signals. The signals are specified as a list of modules." },
    { NOTETRAN,         "note_tran",        "Performs some sort of difficult to comprehend translation of note data!" },
    { WAITTRIG,         "wait_trig",        "Waits for trigger one before outputting trigger two a specified number of times." },
    { PATTERNTRIG,      "pattern_trig",     "A binary pattern is used to decide if the input trigger should be passed through to output or not." },
    { STATEGATETRIG,    "state_gate_trig",  "The input trigger is allowed through to output only if the state input is on." },
    { INVERT,           "invert",           "Flips the sign of a signal." },
    { TIMER,            "timer",            "Outputs a trigger after specified intervals." },
    { SYNCCLOCK,        "sync_clock",       "A clock that can be syncronised with the time line (or modulated around it)." },
    { WCNTTRIGGER,      "wcnt_trigger",     "A connective module which can be used, for example, by the trig_switcher." },
    { TRIGSWITCHER,     "trig_switcher",    "On input trigger switches to the next ouput trigger. The triggers are specified as a list of modules." },
    { ONOFFTRIG,        "on_off_trig",      "Outputs one of two triggers when the input signal rises above a specified level. Outputs the other trigger when the signal falls." },
    { PEAKDETECTOR,     "peak_detector",    "Can be used to detect when input signal peaks beyond the bounds of a specified range. Can be used to display a message or abort processing." },
    { STEPPER,          "stepper",          "Given a user-defined shape, divide it into step_count sections. When input triggers, step along each section outputting the value found." },
    { ADDER,            "adder",            "Add two signals together." },
    { SUBTRACTER,       "subtracter",       "Subtract the second signal from the first."},
    { TRIGDELAY,        "trig_delay",       "Delay a trigger output by a specified time." },
    { SIMPLEDELAY,      "simple_delay",     "Delay a signal by a specified time." },
    { DIFFFILTER,       "diff_filter",      "Calculates the difference between the previous and current sample processed." },
    { IMPULSE,          "impulse",          "Outputs the value of 1.0 for a single sample when input triggers." },
    { ORBIT,            "orbit",            "Implements Threeply, Quadrup, or Hopalong fractals for modulation purposes."  },
    #ifdef WITH_LADSPA
    { GLAME_BUTTERWORTH,        "glame_butterworth",        "Provides the Glame Butterworth High and Low pass filter LADSPA plugins (if available)."         },
    { FAST_LOOKAHEAD_LIMITER,   "fast_lookahead_limiter",   "Provides the Fast Lookahead Limiter LADSPA plugin (if available)."    },
    { DC_OFFSET_REMOVER,        "dc_offset_remover",        "Provides the DC Offset Remove LADSPA plugin (if available)."        },
    { SC1,                      "sc1",                      "Provides the SC1 Compressor LADSPA plugin (if available)." },
    { SINGLE_BAND_PARA,         "single_band_parametric",   "Provides the Single Band Parametric LADSPA plugin (if available)."    },
    { GLAME_FILTER,             "glame_filter",             "Provides the Glame High and Low pass filter LADSPA plugins (if available)." },
    { BODE_FREQ_SHIFTER,        "bode_freq_shifter",        "Provides the Bode Frequency Shifter LADSPA plugin (if available)." },
    { CAPS_PLATE,               "caps_plate",               "Provides the C* Plate LADSPA plugin (if available)." },
    { CAPS_PLATE2X2,            "caps_plate2x2",            "Provides the C* Plate LADSPA plugin (if available)." },
    #endif
    { WAVE,             "wave",             "Outputs one of a number of basic waveforms."             },
    { WAVE_PHASE,       "wave_phase",       "Outputs one waveform multiplied by another. The two waveforms may be synchronised by phase despite operating at different frequencies." },
    { CONSTANT_FREQ,    "constant_freq",    "Outputs an unchanging frequency value, and the phase-step equivalent." },
    { CONSTANT_NOTE,    "constant_note",    "Outputs an unchanging note name as well as the frequency and phase-step equivalents." },
    { BALANCE,          "balance",          "Combines a pair of signals with optional bias toward one or the other." },
    { TRIGECHO,         "trig_echo",        "Delays the trigger input by a specified time, with feedback." },
    { INSPECT,          "inspector",        "A module displaying values of specified signals when the corresponding input triggers." },
    { TRIGCOUNTER,      "trig_counter",     "Outputs a count of how many times the input triggers. Can be reset and/or set to ignore a number of triggers." },
    { TRIGROUTER,       "trig_router",      "Routes the trigger input amongst a group of wcnt_trigger modules which are created by the trig_router itself. The wcnt_trigger modules are added to a group of the same name." },
    { GROUPCONTROL,     "group_control",    "Used to stop or start processing for all modules within a specified group. Use in combination with the trig_router. Take care!" },
    { FADER,            "fader",            "Perform a fade in and fade out at specific bars in the time line." },
    { SAMPLECLIMB,      "sample_climb",     "Similar to sample_hold, but instead of holding the value and decaying, this module optionally climbs to the sampled value before holding it." }
};

