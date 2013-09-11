#include "../include/paramnames.h"

#ifdef NAME_CHECK
#include <iostream>
#endif

paramnames::paramnames()
{
#ifdef NAME_CHECK
    for (int i = FIRST; i < LAST; i++){
        if (data[i].type != i) {
            std::cout << "\n***** paramnames error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

char const* const paramnames::get_name(PAR_TYPE id) const
{
    if (id >= FIRST && id < LAST)
        return data[id].name;
    else
        return data[FIRST].name;
}

iocat::IOCAT paramnames::get_category(PAR_TYPE id) const
{
    if (id >= FIRST && id < LAST)
        return data[id].cat;
    else
        return data[FIRST].cat;
}

paramnames::PAR_TYPE paramnames::get_type(char const* const pname) const
{
    for (int i = FIRST + 1; i < LAST; i++) {
        if (strcmp(data[i].name, pname) == 0)
            return (PAR_TYPE)i;
    }
    return FIRST;
}

const paramnames::param_data paramnames::data[LAST] =
{
    { FIRST,            "BAD_PARAM_TYPE",       iocat::FIRST   },
    { SUSTAIN_STATUS,   "sustain_state",        iocat::STATE   },
    { ZERO_RETRIGGER,   "zero_retrigger",       iocat::STATE   },
    { FREQ,             "freq",                 iocat::DOUBLE  },
    { FREQ_MOD1SIZE,    "freq_mod1_size",       iocat::DOUBLE  },
    { FREQ_MOD2SIZE,    "freq_mod2_size",       iocat::DOUBLE  },
    { VALUE,            "value",                iocat::DOUBLE  },
    { SIG_RANGE_HI,     "sig_range_hi",         iocat::DOUBLE  },
    { SIG_RANGE_LO,     "sig_range_lo",         iocat::DOUBLE  },
    { FREQ_RANGE_HI,    "freq_range_hi",        iocat::DOUBLE  },
    { FREQ_RANGE_LO,    "freq_range_lo",        iocat::DOUBLE  },
    { STEP_COUNT,       "step_count",           iocat::SHORT   },
    { DELAY_TIME,       "delay_time",           iocat::DOUBLE  },
    { RAMP_TIME,        "ramp_time",            iocat::DOUBLE  },
    { START_LEVEL,      "start_level",          iocat::DOUBLE  },
    { END_LEVEL,        "end_level",            iocat::DOUBLE  },
    { AMP_MODSIZE,      "amp_mod_size",         iocat::DOUBLE  },
    { FUNC,             "function",             iocat::FIX_STR },
    { MIN_OUT,          "min_out",              iocat::DOUBLE  },
    { MAX_OUT,          "max_out",              iocat::DOUBLE  },
    { POSNEG_MIRROR,    "posneg_mirror",        iocat::STATE   },
    { OCTAVE,           "octave_offset",        iocat::SHORT   },
    { PORTAMENTO,       "portamento_time",      iocat::DOUBLE  },
    { DECAY_TIME,       "decay_time",           iocat::DOUBLE  },
    { START_POS_MIN,    "start_pos_min",        iocat::ULONG   },
    { START_POS_MAX,    "start_pos_max",        iocat::ULONG   },
    { LOOP_BEGIN,       "loop_start_pos",       iocat::ULONG   },
    { LOOP_END,         "loop_end_pos",         iocat::ULONG   },
    { LOOP_MODE,        "loop_mode",            iocat::FIX_STR },
    { ANTI_CLIP,        "anti_clip_samples",    iocat::SHORT   },
    { PHASE_STEP_AMOUNT,"phase_step_amount",    iocat::DOUBLE  },
    { RECYCLE_MODE,     "recycle_mode",         iocat::STATE   },
    { TRIG_RESET_PHASE, "on_trig_reset_phase",  iocat::STATE   },
    { CYCLES,           "phase_cycles",         iocat::DOUBLE  },
    { PULSE_WIDTH,      "pulse_width",          iocat::DOUBLE  },
    { PWM_SIZE,         "pwm_size",             iocat::DOUBLE  },
    { PAN,              "pan",                  iocat::DOUBLE  },
    { PAN_MODSIZE,      "pan_mod_size",         iocat::DOUBLE  },
    { CLIP_LEVEL,       "clip_level",           iocat::DOUBLE  },
    { MASTER_LEVEL,     "master_level",         iocat::DOUBLE  },
    { NORM_FREQ,        "normal_freq",          iocat::DOUBLE  },
    { NORM_MODSIZE,     "norm_mod_size",        iocat::DOUBLE  },
    { TRIGGER_LEVEL,    "trigger_level",        iocat::DOUBLE  },
    { FILENAME,         "filename",             iocat::STRING  },
    { WAVFILEIN,        "wavfile",              iocat::DOBJ    },
    { VELOCITY_RESPONSE,"velocity_response_time",iocat::DOUBLE },
    { PROBABILITY,      "probability",          iocat::DOUBLE  },
    { NOTPROBABILITY,   "not_probability",      iocat::DOUBLE  },
    { LOGICFUNC,        "function",             iocat::FIX_STR },
    { XFADE_TIME,       "xfade_time",           iocat::DOUBLE  },
    { TUNING_SEMITONES, "tuning_semitones",     iocat::DOUBLE  },
    { MEAN_TOTAL,       "mean_total",           iocat::STATE   },
    { WAV_BASENAME,     "basename",             iocat::STRING  },
    { PLAY_DIR,         "play_dir",             iocat::FIX_STR },
    { PLAY_MODE,        "play_mode",            iocat::FIX_STR },
    { JUMP_MODE,        "jump_mode",            iocat::FIX_STR },
    { LOOP_IS_OFFSET,   "loop_is_offset",       iocat::STATE   },
    { LOOP_BI_OFFSET,   "bi-directional_offset",iocat::SHORT   },
    { AC_EACH_END,      "anti_clip_each_end",   iocat::STATE   },
    { POWER_MIN,        "power_min",            iocat::DOUBLE  },
    { POWER_MAX,        "power_max",            iocat::DOUBLE  },
    { RUDE_MODE,        "rude_mode",            iocat::STATE   },
    { WETDRY,           "wet/dry",              iocat::DOUBLE  },
    { START_BAR,        "start_bar",            iocat::SHORT   },
    { END_BAR,          "end_bar",              iocat::SHORT   },
    { ZERO_SEARCH_RANGE,"zero_search_range",    iocat::SHORT   },
    { GAIN,             "gain",                 iocat::DOUBLE  },
    { GAIN_MODSIZE,     "gain_mod_size",        iocat::DOUBLE  },
    { FEED_LEVEL,       "feedback",             iocat::DOUBLE  },
    { FEED_MODSIZE,     "feedback_mod_size",    iocat::DOUBLE  },
    { EXIT_BAR,         "exit_bar",             iocat::SHORT   },
    { RESPONSE_TIME,    "response_time",        iocat::DOUBLE  },
    { LEFT_LEVEL,       "left_level",           iocat::DOUBLE  },
    { RIGHT_LEVEL,      "right_level",          iocat::DOUBLE  },
    { RMS_TIME,         "rms_time",             iocat::DOUBLE  },
    { DC_TIME,          "dc_time",              iocat::DOUBLE  },
    { UP_THRESH,        "up_thresh",            iocat::DOUBLE  },
    { LO_THRESH,        "lo_thresh",            iocat::DOUBLE  },
    { LEVEL,            "level",                iocat::DOUBLE  },
    { ATTACK_TIME,      "attack_time",          iocat::DOUBLE  },
    { ATTACK_LEVEL,     "attack_level",         iocat::DOUBLE  },
    { RELEASE_TIME,     "release_time",         iocat::DOUBLE  },
    { RELEASE_LEVEL,    "release_level",        iocat::DOUBLE  },
    { INDEPENDANT,      "independant",          iocat::STATE   },
    { USE_RATIOS,       "use_ratios",           iocat::STATE   },
    { NO_LONOTE,        "no_lo_notename",       iocat::STRING  },
    { NO_HINOTE,        "no_hi_notename",       iocat::STRING  },
    { NS_LONOTE,        "ns_lo_notename",       iocat::STRING  },
    { NS_HINOTE,        "ns_hi_notename",       iocat::STRING  },
    { MINNO_OUT,        "min_no_out",           iocat::DOUBLE  },
    { MAXNO_OUT,        "max_no_out",           iocat::DOUBLE  },
    { MINNS_OUT,        "min_ns_out",           iocat::DOUBLE  },
    { MAXNS_OUT,        "max_ns_out",           iocat::DOUBLE  },
    { DETRAN_NO,        "detranspose_no",       iocat::STATE   },
    { DETRAN_NS,        "detranspose_ns",       iocat::STATE   },
    { NO_RESPTIME,      "no_response_time",     iocat::DOUBLE  },
    { NS_RESPTIME,      "ns_response_time",     iocat::DOUBLE  },
    { MIN_WAIT,         "min_wait_time",        iocat::DOUBLE  },
    { MAX_WAIT,         "max_wait_time",        iocat::DOUBLE  },
    { TRIG_STRING,      "pattern",              iocat::STRING  },
    { QUARTER_VAL,      "quarter_value",        iocat::SHORT   },
    { RELEASE_RATIO,    "release_is_ratio",     iocat::STATE   },
    { MIN_TIME,         "min_time",             iocat::DOUBLE  },
    { MAX_SUSTAIN_TIME, "max_sustain_time",     iocat::DOUBLE  },
    { PRECISION,        "precision",            iocat::SHORT   },
    { NAME,             "name",                 iocat::STRING  },
    { NOTE_POS,         "pos",                  iocat::DOUBLE  },
    { NOTE_LEN,         "len",                  iocat::DOUBLE  },
    { NOTE_VEL,         "vel",                  iocat::DOUBLE  },
    { ADSRSECT,         "name",                 iocat::FIX_STR },
    { UPTIME,           "ut",                   iocat::DOUBLE  },
    { LOTIME,           "lt",                   iocat::DOUBLE  },
    { UPLEVEL,          "ul",                   iocat::DOUBLE  },
    { LOLEVEL,          "ll",                   iocat::DOUBLE  },
    { UPDEG,            "ud",                   iocat::DOUBLE  },
    { LODEG,            "ld",                   iocat::DOUBLE  },
    { METER,            "tsig",                 iocat::METER   },
    { BAR,              "bar",                  iocat::SHORT   },
    { BPM,              "bpm",                  iocat::DOUBLE  },
    { RIFFNAME,         "riff",                 iocat::DOBJ    },
    { MODNAME,          "name",                 iocat::SYNTHMOD},
    { ROOT_NOTE,        "root_note",            iocat::STRING  },
    { TRANSPOSE,        "transpose",            iocat::SHORT   },
    { STARTLEN,         "start_len",            iocat::DOUBLE  },
    { ENDLEN,           "end_len",              iocat::DOUBLE  },
    { SIGIN_LEVEL,      "sigin_level",          iocat::DOUBLE  },
    { SIGOUT_UPLEVEL,   "so_ul",                iocat::DOUBLE  },
    { SIGOUT_LOLEVEL,   "so_ll",                iocat::DOUBLE  },
    { SECONDS,          "seconds",              iocat::DOUBLE  },
    { MOD_ACTION,       "mod_action",           iocat::FIX_STR },
    { DOBJ_ACTION,      "dobj_action",          iocat::FIX_STR },
    { NAME_EXT,         "name_extension",       iocat::STRING  },
    { PARAMETER,        "param",                iocat::STRING  },
    { VALUESTR,         "value",                iocat::STRING  },
    { DOBJNAME,         "name",                 iocat::DOBJ    },
    { INMODULE,         "in_module",            iocat::STRING  },
    { INPUT,            "input",                iocat::STRING  },
    { OUTMODULE,        "out_module",           iocat::STRING  },
    { OUTPUT,           "output",               iocat::STRING  },
    { STR_UNNAMED,      "<unnamed>",            iocat::STRING  },
    { STR_LIST,         "<unnamed>",            iocat::STRING  },
    { SNAP_TO,          "snap_to",              iocat::DOUBLE  },
    { COUNT,            "count",                iocat::SHORT   },
    { CHECK_LEVELS,     "check_levels",         iocat::STATE   },
    { MSG,              "message",              iocat::STRING  },
    { FORCE_ABORT,      "force_abort",          iocat::STATE   },
    { MAXPEAKS,         "max_peaks",            iocat::SHORT   },
    { REPEAT,           "repeat",               iocat::SHORT   },
    { REPEAT_STRIPE,    "repeat_stripe",        iocat::SHORT   },
    { COPYFROM,         "from",                 iocat::STRING  },
    { COPYTO,           "to",                   iocat::STRING  },
    { POS,              "pos",                  iocat::DOUBLE  },
    { BIAS,             "bias",                 iocat::DOUBLE  },
    { SAMPLECOUNT,      "sample_count",         iocat::SHORT   },
    { ORBIT_TYPE,       "orbit_type",           iocat::FIX_STR },
    { A,                "a",                    iocat::DOUBLE  },
    { B,                "b",                    iocat::DOUBLE  },
    { C,                "c",                    iocat::DOUBLE  },
    { TEST_ITER,        "test_iterations",      iocat::ULONG   },
    { DATA_FMT,         "data_format",          iocat::FIX_STR },
    #ifdef WITH_LADSPA
    { GLAME_FILTER_TYPE,"filter_type",          iocat::FIX_STR },
    { RESONANCE,        "resonance",            iocat::DOUBLE  },
    { RES_MODSIZE,      "res_mod_size",         iocat::DOUBLE  },
    { GAIN_DB,          "gain_db",              iocat::DOUBLE  },
    { LIMIT_DB,         "limit_db",             iocat::DOUBLE  },
    { RELEASE_SECS,     "release_secs",         iocat::DOUBLE  },
    { ATTEN_DB,         "attenuation_db",       iocat::DOUBLE  },
    { THRESH_DB,        "threshold_db",         iocat::DOUBLE  },
    { RATIO_1N,         "ratio_1:n",            iocat::DOUBLE  },
    { KNEE_DB,          "knee_db",              iocat::DOUBLE  },
    { MAKEUP_DB,        "makeup_db",            iocat::DOUBLE  },
    { BANDWIDTH,        "bandwidth",            iocat::DOUBLE  },
    { BANDWIDTH_MODSIZE,"bandwidth_mod_size",   iocat::DOUBLE  },
    { STAGES,           "stages",               iocat::SHORT   },
    { FREQ_SHIFT,       "freq_shift",           iocat::DOUBLE  },
    { SHIFT_MODSIZE,    "shift_mod_size",       iocat::DOUBLE  },
    { TAIL,             "tail",                 iocat::DOUBLE  },
    { DAMPING,          "damping",              iocat::DOUBLE  },
    #endif
    { ADSR_NAME,        "adsr_name",            iocat::SYNTHMOD},
    { ATTACK_SCALE,     "attack_scale",         iocat::DOUBLE  },
    { DECAY_SCALE,      "decay_scale",          iocat::DOUBLE  },
    { RELEASE_SCALE,    "release_scale",        iocat::DOUBLE  },
    { WAVE_TYPE,        "type",                 iocat::FIX_STR },
    { WAVE_SHAPE_TYPE,  "shape_type",           iocat::FIX_STR },
    { INVERT_ALT,       "invert_alt",           iocat::STATE   },
    { SYNC_SHAPE,       "sync_shape",           iocat::STATE   },
    { MODIFIER_TYPE,    "type",                 iocat::FIX_STR },
    { SEND_INPUT_OUT,   "send_input_out",       iocat::STATE   },
    /* inspect module's bs */
    { INSPECT_DF_MSG,   "_double_float_msg_",   iocat::STRING  },
    { INSPECT_SI_MSG,   "_short_int_msg_",      iocat::STRING  },
    { INSPECT_UL_MSG,   "_long_int_msg_",       iocat::STRING  },
    { INSPECT_ST_MSG,   "_state_msg_",          iocat::STRING  },
    { INSPECT_STR_MSG,  "_string_msg_",         iocat::STRING  },
    { PRE_COUNT,        "pre_count",            iocat::SHORT   },
    { WRAP,             "wrap",                 iocat::STATE   },
    { GROUP_NAME,       "group_name",           iocat::STRING  },
    { FADE_IN_TIME,     "fade_in_time",         iocat::DOUBLE  },
    { FADE_OUT_TIME,    "fade_out_time",        iocat::DOUBLE  },
    { RATE,             "rate",                 iocat::DOUBLE  },
    { SNAPSHOT_MODE,    "snapshot_mode",        iocat::STATE   },
    { DROP_CHECK_RANGE, "drop_check_range",     iocat::SHORT   }
};

