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


const char* inputnames::get_name(IN_TYPE id)
{
    if (id >= IN_FIRST && id < IN_LAST)
        return data[id].name;
    else
        return data[IN_FIRST].name;
}

iocat::IOCAT inputnames::get_category(IN_TYPE id)
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
    { IN_FIRST,             "BAD_INPUT_TYPE",       iocat::FIRST   },
    { IN_VELOCITY,          "in_velocity",          iocat::DOUBLE  },
    { IN_NOTE_ON_TRIG,      "in_note_on_trig",      iocat::TRIG    },
    { IN_NOTE_SLIDE_TRIG,   "in_note_slide_trig",   iocat::TRIG    },
    { IN_NOTE_OFF_TRIG,     "in_note_off_trig",     iocat::TRIG    },
    { IN_LEFT,              "in_left",              iocat::DOUBLE  },
    { IN_RIGHT,             "in_right",             iocat::DOUBLE  },
    { IN_FREQ,              "in_frequency",         iocat::DOUBLE  },
    { IN_FREQ_MOD1,         "in_freq_mod1",         iocat::DOUBLE  },
    { IN_FREQ_MOD2,         "in_freq_mod2",         iocat::DOUBLE  },
    { IN_PHASE_TRIG,        "in_phase_trig",        iocat::TRIG    },
    { IN_PHASE_STEP,        "in_phase_step",        iocat::DOUBLE  },
    { IN_SIGNAL,            "in_signal",            iocat::DOUBLE  },
    { IN_EG,                "in_amp_eg",            iocat::DOUBLE  },
    { IN_AMP_MOD,           "in_amp_mod",           iocat::DOUBLE  },
    { IN_PAN_MOD,           "in_pan_mod",           iocat::DOUBLE  },
    { IN_TRIG,              "in_trig",              iocat::TRIG    },
    { IN_WAVE,              "in_waveform",          iocat::DOUBLE  },
    { IN_PWM,               "in_pwm",               iocat::DOUBLE  },
    { IN_CUTOFF_PHASE_STEP, "in_cutoff_phase_step", iocat::DOUBLE  },
    { IN_PLAY_TRIG,         "in_play_trig",         iocat::TRIG    },
    { IN_START_POS_MOD,     "in_start_pos_mod",     iocat::DOUBLE  },
    { IN_NORM_MOD,          "in_normal_freq_mod",   iocat::DOUBLE  },
    { IN_V_MOD,             "in_level_mod",         iocat::DOUBLE  },
    { IN_H_MOD,             "in_time_mod",          iocat::DOUBLE  },
    { IN_TRIG1,             "in_trig1",             iocat::TRIG    },
    { IN_TRIG2,             "in_trig2",             iocat::TRIG    },
    { IN_BAR,               "in_bar",               iocat::SHORT   },
    { IN_BAR_TRIG,          "in_bar_trig",          iocat::TRIG    },
    { IN_POS_STEP_SIZE,     "in_pos_step_size",     iocat::DOUBLE  },
    { IN_WRITE_TRIG,        "in_write_trig",        iocat::TRIG    },
    { IN_STOP_TRIG,         "in_stop_trig",         iocat::TRIG    },
    { IN_BPM,               "in_bpm",               iocat::DOUBLE  },
    { IN_POWER_MOD,         "in_power_mod",         iocat::DOUBLE  },
    { IN_RUDE_SWITCH_TRIG,  "in_rude_switch_trig",  iocat::TRIG    },
    { IN_NOTENAME,          "in_notename",          iocat::STRING  },
    { IN_GAIN_MOD,          "in_gain_mod",          iocat::DOUBLE  },
    { IN_FEEDBACK,          "in_feedback",          iocat::DOUBLE  },
    { IN_FB_MOD,            "in_feedback_mod",      iocat::DOUBLE  },
    { IN_BEATS_PER_BAR,     "in_beats_per_bar",     iocat::SHORT   },
    { IN_BEAT_VALUE,        "in_beat_value",        iocat::SHORT   },
    { IN_MODULATION,        "in_modulation",        iocat::DOUBLE  },
    { IN_DETRANSPOSE,       "in_detranspose",       iocat::SHORT   },
    { IN_STATE,             "in_state",             iocat::STATE   },
    { IN_PLAY_STATE,        "in_play_state",        iocat::STATE   },
    { IN_RESTART_TRIG,      "in_restart_trig",      iocat::TRIG    },
    #ifdef WITH_LADSPA
    { IN_RES_MOD,           "in_res_mod",           iocat::DOUBLE  },
    { IN_THRESH_MOD,        "in_thresh_mod",        iocat::DOUBLE  },
    { IN_BANDWIDTH_MOD,     "in_bandwidth_mod",     iocat::DOUBLE  },
    { IN_SHIFT_MOD,         "in_shift_mod",         iocat::DOUBLE  },
    { IN_SHAPE_PHASE_STEP,  "in_shape_phase_step",  iocat::DOUBLE  },
    { IN_FREQ_MOD,          "in_freq_mod",          iocat::DOUBLE  },
    { IN_SIGNAL1,           "in_signal1",           iocat::DOUBLE  },
    { IN_SIGNAL2,           "in_signal2",           iocat::DOUBLE  },
    #endif
    /* inspect module's bs */
    { IN__DF,               "_in_double_float_",      iocat::DOUBLE  },
    { IN__DF_TRIG,          "_in_double_float_trig_", iocat::TRIG    },
    { IN__SI,               "_in_short_int_",         iocat::SHORT   },
    { IN__SI_TRIG,          "_in_short_int_trig_",    iocat::TRIG    },
    { IN__UL,               "_in_unsigned_long_",     iocat::ULONG   },
    { IN__UL_TRIG,          "_in_unsigned_long_trig_",iocat::TRIG    },
    { IN__ST,               "_in_state_",             iocat::STATE   },
    { IN__ST_TRIG,          "_in_state_trig_",        iocat::TRIG    },
    { IN__STR,              "_in_string_",            iocat::STRING  },
    { IN__STR_TRIG,         "_in_string_trig_",       iocat::TRIG    },
    { IN_RESET_TRIG,        "in_reset_trig",        iocat::TRIG    },
    { IN_COUNT,             "in_count",             iocat::SHORT   },
    { IN_BIAS,              "in_bias",              iocat::DOUBLE  }
};
