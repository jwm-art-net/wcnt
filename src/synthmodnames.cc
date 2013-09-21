#include "../include/synthmodnames.h"

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


const char* synthmodnames::get_name(SYNTH_MOD_TYPE id)
{
    if (id > FIRST && id < LAST)
        return data[id].name;
    else
        return data[FIRST].name;
}

synthmodnames::SYNTH_MOD_TYPE
synthmodnames::get_type(const char* const mname)
{
    for (int i = FIRST; i < LAST; i++)
        if (strcmp(data[i].name, mname) == 0)
            return (SYNTH_MOD_TYPE)i;
    return FIRST;
}

const synthmodnames::mod_data synthmodnames::data[LAST] =
{
    { FIRST,            "BAD_MODULE_TYPE"   },
    { NONEZERO,         "none"              },
    { ADSR,             "adsr"              },
    { STEREOAMP,        "stereo_amp"        },
    { CLOCK,            "clock"             },
    { CONSTMOD,         "constant"          },
    { FREQGEN,          "freq_generator"    },
    { LFOCLOCK,         "lfo_clock"         },
    { LFOCONTROL,       "lfo_controller"    },
    { MODIFIER,         "modifier"          },
    { NOISEGEN,         "noise_generator"   },
    { OSCCLOCK,         "osc_clock"         },
    { SAMPLEHOLD,       "sample_hold"       },
    { SEQUENCER,        "sequencer"         },
    { SQUAREWAVE,       "square_wave"       },
    { TRIGGER,          "trigger"           },
    { TRIWAVE2,         "tri_wave2"         },
    { USERWAVE,         "user_wave"         },
    { SAMPLER,          "sampler"           },
    { WAVFILEOUT,       "wavfile_out"       },
    { STEREOCHANNEL,    "mix_chan"          },
    { STEREOMIXER,      "mixer"             },
    { RANDTRIGGER,      "rnd_trigger"       },
    { LOGICTRIGGER,     "logic_trigger"     },
    { SWITCHER,         "switcher"          },
    { WCNTSIGNAL,       "wcnt_signal"       },
    { COMBINER,         "combiner"          },
    { TIMEMAP,          "time_map"          },
    { CONTRASTER,       "contraster"        },
    { SERIALWAVFILEOUT, "serial_wavout"     },
    { DELAY,            "delay"             },
    { ECHO,             "echo"              },
    { MONOAMP,          "mono_amp"          },
    { WCNTEXIT,         "wcnt_exit"         },
    { MULTIPLIER,       "multiplier"        },
    { RANGELIMIT,       "range_limit"       },
    { PAN,              "panner"            },
    { RMS,              "rms"               },
    { DCFILTER,         "dc_filter"         },
    { DYNAMIC,          "dynamic"           },
    { SPREADER,         "spreader"          },
    { NOTETRAN,         "note_tran"         },
    { WAITTRIG,         "wait_trig"         },
    { PATTERNTRIG,      "pattern_trig"      },
    { STATEGATETRIG,    "state_gate_trig"   },
    { INVERT,           "invert"            },
    { TIMER,            "timer"             },
    { SYNCCLOCK,        "sync_clock"        },
    { WCNTTRIGGER,      "wcnt_trigger"      },
    { TRIGSWITCHER,     "trig_switcher"     },
    { ONOFFTRIG,        "on_off_trig"       },
    { PEAKDETECTOR,     "peak_detector"     },
    { STEPPER,          "stepper"           },
    { ADDER,            "adder"             },
    { SUBTRACTER,       "subtracter"        },
    { TRIGDELAY,        "trig_delay"        },
    { SIMPLEDELAY,      "simple_delay"      },
    { DIFFFILTER,       "diff_filter"       },
    { IMPULSE,          "impulse"           },
    { ORBIT,            "orbit"             },
    #ifdef WITH_LADSPA
    { GLAME_BUTTERWORTH,        "glame_butterworth"         },
    { FAST_LOOKAHEAD_LIMITER,   "fast_lookahead_limiter"    },
    { DC_OFFSET_REMOVER,        "dc_offset_remover"         },
    { SC1,                      "sc1"                       },
    { SINGLE_BAND_PARA,         "single_band_parametric"    },
    { GLAME_FILTER,             "glame_filter"              },
    { BODE_FREQ_SHIFTER,        "bode_freq_shifter"         },
    { CAPS_PLATE,               "caps_plate"                },
    { CAPS_PLATE2X2,            "caps_plate2x2"             },
    #endif
    { WAVE,             "wave"              },
    { WAVE_PHASE,       "wave_phase"        },
    { CONSTANT_FREQ,    "constant_freq"     },
    { CONSTANT_NOTE,    "constant_note"     },
    { BALANCE,          "balance"           },
    { TRIGECHO,         "trig_echo"         },
    { INSPECT,          "inspector"         },
    { TRIGCOUNTER,      "trig_counter"      },
    { TRIGROUTER,       "trig_router"       },
    { GROUPCONTROL,     "group_control"     },
    { FADER,            "fader"             },
    { SAMPLECLIMB,      "sample_climb"      }
};

