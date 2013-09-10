#ifndef OUTPUTNAMES_H
#include "../include/outputnames.h"

#ifdef NAME_CHECK
#include <iostream>
#endif

outputnames::outputnames()
{
#ifdef NAME_CHECK
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

char const* const outputnames::get_name(OUT_TYPE id) const
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return data[id].name;
    else
        return data[OUT_FIRST].name;
}

iocat::IOCAT outputnames::get_category(OUT_TYPE id) const
{
    if (id >= OUT_FIRST && id < OUT_LAST)
        return data[id].cat;
    else
        return data[OUT_FIRST].cat;
}

outputnames::OUT_TYPE outputnames::get_type(char const* const oname) const
{
    for (int i = OUT_FIRST + 1; i < OUT_LAST; i++)
        if (strcmp(data[i].name, oname) == 0)
            return (OUT_TYPE)i;
    return OUT_FIRST;
}

outputnames::OUT_TYPE
outputnames::get_nonezerotype(iocat::IOCAT iocat) const
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
    default:
        ot = OUT_FIRST;
    }
    return ot;
}

const outputnames::output_data outputnames::data[OUT_LAST] =
{
    { OUT_FIRST,            "BAD_OUTPUT_TYPE",      iocat::FIRST   },
    { OUT_NONE_DOUBLE,      "off",                  iocat::DOUBLE  },
    { OUT_NONE_SHORT,       "off",                  iocat::SHORT   },
    { OUT_NONE_ULONG,       "off",                  iocat::ULONG   },
    { OUT_NONE_TRIG,        "off",                  iocat::TRIG    },
    { OUT_NONE_STATE,       "off",                  iocat::STATE   },
    { OUT_OUTPUT,           "out_output",           iocat::DOUBLE  },
    { OUT_PLAY_STATE,       "out_play_state",       iocat::STATE   },
    { OUT_LEFT,             "out_left",             iocat::DOUBLE  },
    { OUT_RIGHT,            "out_right",            iocat::DOUBLE  },
    { OUT_TRIG,             "out_trig",             iocat::TRIG    },
    { OUT_PHASE_TRIG,       "out_phase_trig",       iocat::TRIG    },
    { OUT_PREMOD_PHASE_STEP,"out_premod_phase_step",iocat::DOUBLE  },
    { OUT_PHASE_STEP,       "out_phase_step",       iocat::DOUBLE  },
    { OUT_FREQ,             "out_frequency",        iocat::DOUBLE  },
    { OUT_NOTE_ON_TRIG,     "out_note_on_trig",     iocat::TRIG    },
    { OUT_NOTE_SLIDE_TRIG,  "out_note_slide_trig",  iocat::TRIG    },
    { OUT_NOTE_OFF_TRIG,    "out_note_off_trig",    iocat::TRIG    },
    { OUT_RIFF_START_TRIG,  "out_riff_start_trig",  iocat::TRIG    },
    { OUT_RIFF_END_TRIG,    "out_riff_end_trig",    iocat::TRIG    },
    { OUT_START_TRIG,       "out_start_trig",       iocat::TRIG    },
    { OUT_END_TRIG,         "out_end_trig",         iocat::TRIG    },
    { OUT_VELOCITY,         "out_velocity",         iocat::DOUBLE  },
    { OUT_TOTAL_SAMPLE_POS, "out_total_sample_pos", iocat::ULONG   },
    { OUT_RIFF_PLAY_STATE,  "out_riff_play_state",  iocat::STATE   },
    { OUT_NOTE_PLAY_STATE,  "out_note_play_state",  iocat::STATE   },
    { OUT_OFF_PULSE,        "out_pulse_off_trig",   iocat::TRIG    },
    { OUT_WRITE_STATE,      "out_file_write_state", iocat::STATE   },
    { OUT_WRITE_START_TRIG, "out_write_start_trig", iocat::TRIG    },
    { OUT_WRITE_END_TRIG,   "out_write_end_trig",   iocat::TRIG    },
    { OUT_PRE_AMP_MOD,      "out_pre_amp_mod",      iocat::DOUBLE  },
    { OUT_NOT_TRIG,         "out_not_trig",         iocat::TRIG    },
    //wcnt 1.1001
    { OUT_BPM,              "out_bpm",              iocat::DOUBLE  },
    { OUT_BAR,              "out_bar",              iocat::SHORT   },
    { OUT_BAR_TRIG,         "out_bar_trig",         iocat::TRIG    },
    { OUT_POS_IN_BAR,       "out_pos_in_bar",       iocat::ULONG   },
    { OUT_POS_STEP_SIZE,    "out_pos_step_size",    iocat::DOUBLE  },
    { OUT_SAMPLE_TOTAL,     "out_sample_total",     iocat::ULONG   },
    { OUT_SAMPLE_IN_BAR,    "out_sample_in_bar",    iocat::ULONG   },
    { OUT_BEATS_PER_BAR,    "out_beats_per_bar",    iocat::SHORT   },
    { OUT_BEAT_VALUE,       "out_beat_value",       iocat::SHORT   },
    { OUT_BPM_CHANGE_TRIG,  "out_bpm_change_trig",  iocat::TRIG    },
    { OUT_METER_CHANGE_TRIG,"out_meter_change_trig",iocat::TRIG    },
    { OUT_BPM_CHANGE_STATE, "out_bpm_change_state", iocat::STATE   },
    { OUT_LOOP_TRIG,        "out_loop_trig",        iocat::TRIG    },
    { OUT_NOTENAME,         "out_notename",         iocat::STRING  },
    { OUT_VELOCITY_RAMP,    "out_velocity_ramp",    iocat::DOUBLE  },
    { OUT_OFF_TRIG,         "out_off_trig",         iocat::TRIG    },
    { OUT_WET_OUTPUT,       "out_wet_output",       iocat::DOUBLE  },
    { OUT_RMS,              "out_rms",              iocat::DOUBLE  },
    { OUT_RISE_TRIG,        "out_rise_trig",        iocat::TRIG    },
    { OUT_FALL_TRIG,        "out_fall_trig",        iocat::TRIG    },
    { OUT_TRANSPOSE,        "out_transpose",        iocat::SHORT   },
    { OUT_NOVALUE,          "out_no_value",         iocat::DOUBLE  },
    { OUT_NSVALUE,          "out_ns_value",         iocat::DOUBLE  },
    { OUT_NOT_NO_TRIG,      "out_not_no_trig",      iocat::TRIG    },
    { OUT_NOT_NS_TRIG,      "out_not_ns_trig",      iocat::TRIG    },
    { OUT_COUNT,            "out_count",            iocat::SHORT   },
    { OUT_WAIT_STATE,       "out_wait_state",       iocat::STATE   },
    { OUT_ATTACK_STATE,     "out_attack_state",     iocat::STATE   },
    { OUT_RELEASE_STATE,    "out_release_state",    iocat::STATE   },
    { OUT_X,                "out_x",                iocat::DOUBLE  },
    { OUT_Y,                "out_y",                iocat::DOUBLE  },
    #ifdef WITH_LADSPA
    { OUT_UP,               "out_up",               iocat::DOUBLE  },
    { OUT_DOWN,             "out_down",             iocat::DOUBLE  }
    #endif
};

#endif
