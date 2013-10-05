#include "../include/paramnames.h"

namespace param
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,         "BAD_PARAM_TYPE",       iocat::ERR_TYPE,"ERROR!"   },
    { SUSTAIN_STATUS,   "sustain_state",        iocat::STATE,   "Set sustain section to be active or ignored." },
    { ZERO_RETRIGGER,   "zero_retrigger",       iocat::STATE,   "Whether to hard reset to zero on retrigger." },
    { FREQ,             "freq",                 iocat::DOUBLE,  "Operating frequency." },
    { FREQ_MOD1SIZE,    "freq_mod1_size",       iocat::DOUBLE,  "Modulation amount using 1st modulation input." },
    { FREQ_MOD2SIZE,    "freq_mod2_size",       iocat::DOUBLE,  "Modulation amount using 2nd modulation input." },
    { VALUE,            "value",                iocat::DOUBLE,  "Output value." },
    { SIG_RANGE_HI,     "sig_range_hi",         iocat::DOUBLE,  "Maximum allowed input level." },
    { SIG_RANGE_LO,     "sig_range_lo",         iocat::DOUBLE,  "Minimum allowed input level." },
    { FREQ_RANGE_HI,    "freq_range_hi",        iocat::DOUBLE,  "Maximum frequency to output." },
    { FREQ_RANGE_LO,    "freq_range_lo",        iocat::DOUBLE,  "Minimum frequency to output." },
    { STEP_COUNT,       "step_count",           iocat::WCINT_T, "Number of steps to split allowed output values into." },
    { DELAY_TIME,       "delay_time",           iocat::DOUBLE,  "Length of delay time (ms)." },
    { RAMP_TIME,        "ramp_time",            iocat::DOUBLE,  "Length of time to ramp up to full output levels (ms)." },
    { START_LEVEL,      "start_level",          iocat::DOUBLE,  "Start output level." },
    { END_LEVEL,        "end_level",            iocat::DOUBLE,  "End output level." },
    { AMP_MODSIZE,      "amp_mod_size",         iocat::DOUBLE,  "Amplitude modulation amount." },
    { FUNC,             "function",             iocat::FIX_STR, "Math function to apply." },
    { POSNEG_MIRROR,    "posneg_mirror",        iocat::STATE,   "Whether to mirror positive values to negative and vice-versa." },
    { OCTAVE,           "octave_offset",        iocat::WCINT_T, "Number of octaves to adjust frequency." },
    { PORTAMENTO,       "portamento_time",      iocat::DOUBLE,  "Length of time (ms) to slide/portamento effect should last." },
    { DECAY_TIME,       "decay_time",           iocat::DOUBLE,  "Length of time (ms) decay should last." },
    { START_POS_MIN,    "start_pos_min",        iocat::SAMP_T,  "Minimum start position (samples)." },
    { START_POS_MAX,    "start_pos_max",        iocat::SAMP_T,  "Maximum start position (samples)." },
    { LOOP_BEGIN,       "loop_start_pos",       iocat::SAMP_T,  "Start position of loop (samples)." },
    { LOOP_END,         "loop_end_pos",         iocat::SAMP_T,  "End of loop position (samples)." },
    { LOOP_MODE,        "loop_mode",            iocat::FIX_STR, "Loop mode." },
    { ANTI_CLIP,        "anti_clip_samples",    iocat::WCINT_T, "Number of samples to x-fade when playback starts or looping occurs." },
    { PHASE_STEP_AMOUNT,"phase_step_amount",    iocat::DOUBLE,  "Amount to adjust phase step. Should be set to 1.0 for normal operation." },
    { RECYCLE_MODE,     "recycle_mode",         iocat::STATE,   "Whether to loop continuously or not." },
    { TRIG_RESET_PHASE, "on_trig_reset_phase",  iocat::STATE,   "Whether to hard-reset phase on re-trigger (may cause clicks, depends on usage)." },
    { CYCLES,           "phase_cycles",         iocat::DOUBLE,  "Amount of phase cycle(s) to run through, 1.0 for normal operation." },
    { PULSE_WIDTH,      "pulse_width",          iocat::DOUBLE,  "Pulse width, 0.5 for equal width positive and negative phase pulses."},
    { PWM_SIZE,         "pwm_size",             iocat::DOUBLE,  "Pulse Width Modulation amount." },
    { PAN,              "pan",                  iocat::DOUBLE,  "Pan position, -1.0 = left, 0.0 = center, +1.0 = right." },
    { PAN_MODSIZE,      "pan_mod_size",         iocat::DOUBLE,  "Pan modulation amount. "},
    { CLIP_LEVEL,       "clip_level",           iocat::DOUBLE,  "Level to clip amplitude at, 1.0 is a fairly sane value - perhaps." },
    { MASTER_LEVEL,     "master_level",         iocat::DOUBLE,  "Master level, somewhere around just below 1.0 usually." },
    { NORM_FREQ,        "normal_freq",          iocat::DOUBLE,  "Frequency the waveform considers it normal to operate at." },
    { NORM_MODSIZE,     "norm_mod_size",        iocat::DOUBLE,  "Modulation amount of the normal operating frequency." },
    { TRIGGER_LEVEL,    "trigger_level",        iocat::DOUBLE,  "Signal level which will cause trigger output." },
    { FILENAME,         "filename",             iocat::STRING,  "Path of file. Can NOT specify a path containing spaces characters." },
    { WAVFILEIN,        "wavfile",              iocat::DOBJ,    "Name of the wavfilein data object to obtain audio from." },
    { VELOCITY_RESPONSE,"velocity_response_time",iocat::DOUBLE, "How fast (ms) to respond to velocity changes." },
    { PROBABILITY,      "probability",          iocat::DOUBLE,  "Probability of something occurring: 0.0 = never, 1.0 = always." },
    { NOTPROBABILITY,   "not_probability",      iocat::DOUBLE,  "Probability of something having not occurred, to occur as something else: 0.0 = never, 1.0 = always." },
    { LOGICFUNC,        "function",             iocat::FIX_STR, "Logic function to use." },
    { XFADE_TIME,       "xfade_time",           iocat::DOUBLE,  "Cross fade time (ms)." },
    { TUNING_SEMITONES, "tuning_semitones",     iocat::DOUBLE,  "Semitone amount to tune frequency, can be fractional." },
    { MEAN_TOTAL,       "mean_total",           iocat::STATE,   "Whether to apply mean average to signals." },
    { WAV_BASENAME,     "basename",             iocat::STRING,  "Basename of WAV filename, a four digit number will be appended to this." },
    { PLAY_DIR,         "play_dir",             iocat::FIX_STR, "Play direction to start with." },
    { PLAY_MODE,        "play_mode",            iocat::FIX_STR, "TODO: figure out how this works..." },
    { JUMP_MODE,        "jump_mode",            iocat::FIX_STR, "TODO: figure out how this works..." },
    { LOOP_IS_OFFSET,   "loop_is_offset",       iocat::STATE,   "Whether to treat loop_start and loop_end as offsets to actual start position when triggered." },
    { LOOP_BI_OFFSET,   "bi-directional_offset",iocat::WCINT_T, "Number of samples to offset when reaching loop points of bi-directional loop." },
    { AC_EACH_END,      "anti_clip_each_end",   iocat::STATE,   "Whether to fade-in and fade-out at start and end of audio file." },
    { POWER_MIN,        "power_min",            iocat::DOUBLE,  "Minimum amount of contrast to apply, 0.0 is none." },
    { POWER_MAX,        "power_max",            iocat::DOUBLE,  "Maximum amount of power to apply, more is more. " },
    { RUDE_MODE,        "rude_mode",            iocat::STATE,   "Whether to use the rude mode or not, the improper implementation." },
    { WETDRY,           "wet/dry",              iocat::DOUBLE,  "How much of the wet signal to apply, 0.0 ~ 1.0." },
    { START_BAR,        "start_bar",            iocat::WCINT_T, "Bar number (0 = first bar) to start processing on." },
    { END_BAR,          "end_bar",              iocat::WCINT_T, "Bar number (0 = first bar) to end processing on." },
    { ZERO_SEARCH_RANGE,"zero_search_range",    iocat::WCINT_T, "Number of samples to search around play/loop points for zero-crossing. " },
    { GAIN,             "gain",                 iocat::DOUBLE,  "Gain value, 0.0 = silence, 1.0 = no gain, > 1.0 = gain. "},
    { GAIN_MODSIZE,     "gain_mod_size",        iocat::DOUBLE,  "Gain modulation amount." },
    { FEED_LEVEL,       "feedback",             iocat::DOUBLE,  "Amount of feedback. 0.0 = none, 1.0 = 100%." },
    { FEED_MODSIZE,     "feedback_mod_size",    iocat::DOUBLE,  "Feedback modulation amount." },
    { EXIT_BAR,         "exit_bar",             iocat::WCINT_T, "Bar to stop all processing and exit the program at." },
    { RESPONSE_TIME,    "response_time",        iocat::DOUBLE,  "Length of time (ms) taken to respond to input." },
    { LEFT_LEVEL,       "left_level",           iocat::DOUBLE,  "Level of left output signal, 0.95 is a fair value perhaps." },
    { RIGHT_LEVEL,      "right_level",          iocat::DOUBLE,  "Level of right output signal, 0.95 is a fair value perhaps." },
    { RMS_TIME,         "rms_time",             iocat::DOUBLE,  "Length of time (ms) to calculate the Root Mean Square over." },
    { DC_TIME,          "dc_time",              iocat::DOUBLE,  "Length of time (ms) to calculate the mean over." },
    { UP_THRESH,        "up_thresh",            iocat::DOUBLE,  "Threshold level of modulation input causing usage of upper shape." },
    { LO_THRESH,        "lo_thresh",            iocat::DOUBLE,  "Threshold level of modulation input causing usage of lower shape." },
    { LEVEL,            "level",                iocat::DOUBLE,  "Signal level to let through, 0.0 = none, 1.0 = 100%." },
    { ATTACK_TIME,      "attack_time",          iocat::DOUBLE,  "Length of time (ms) to take to reach attack level." },
    { ATTACK_LEVEL,     "attack_level",         iocat::DOUBLE,  "Amplitude level to reach by end of attack stage." },
    { RELEASE_TIME,     "release_time",         iocat::DOUBLE,  "Length of time (ms) to reach release level." },
    { RELEASE_LEVEL,    "release_level",        iocat::DOUBLE,  "Amplitude level to reach at end of release stage." },
    { USE_RATIOS,       "use_ratios",           iocat::STATE,   "Whether the shape levels should be applied as ratios or not." },
    { NO_LONOTE,        "no_lo_notename",       iocat::STRING,  "Lowest note-name to accept from note-on." },
    { NO_HINOTE,        "no_hi_notename",       iocat::STRING,  "Highest note-name to accept from note-on." },
    { NS_LONOTE,        "ns_lo_notename",       iocat::STRING,  "Lowest note-name to accept from note-slide." },
    { NS_HINOTE,        "ns_hi_notename",       iocat::STRING,  "Highest note-name to accept from note-slide." },
    { MINNO_OUT,        "min_no_out",           iocat::DOUBLE,  "Minimum value to output from minimum note-on event." },
    { MAXNO_OUT,        "max_no_out",           iocat::DOUBLE,  "Maximum value to output from minimum note-on event." },
    { MINNS_OUT,        "min_ns_out",           iocat::DOUBLE,  "Minimum value to output from minimum note-slide event." },
    { MAXNS_OUT,        "max_ns_out",           iocat::DOUBLE,  "Maximum value to output from minimum note-slide event." },
    { DETRAN_NO,        "detranspose_no",       iocat::STATE,   "Whether to detranspose note-on events before processing them." },
    { DETRAN_NS,        "detranspose_ns",       iocat::STATE,   "Whether to detranspose note-slide events before processing them." },
    { NO_RESPTIME,      "no_response_time",     iocat::DOUBLE,  "Length of time (ms) to respond to note-on." },
    { NS_RESPTIME,      "ns_response_time",     iocat::DOUBLE,  "Length of time (ms) to respond to note-slide." },
    { MIN_WAIT,         "min_wait_time",        iocat::DOUBLE,  "Minimum amount of time (ms) to wait before processing." },
    { MAX_WAIT,         "max_wait_time",        iocat::DOUBLE,  "Maximum amount of time (ms) to wait before processing." },
    { TRIG_STRING,      "pattern",              iocat::STRING,  "Trigger pattern to use, 0 = do not trigger, 1 = trigger." },
    { QUARTER_VAL,      "quarter_value",        iocat::WCINT_T, "Ticks Per Quarter Note (often a value divisible by both 3 and 4)." },
    { RELEASE_RATIO,    "release_is_ratio",     iocat::STATE,   "Generally should mostly be set to on." },
    { MIN_TIME,         "min_time",             iocat::DOUBLE,  "Minium time (ms) before allowing release stage to activate." },
    { MAX_SUSTAIN_TIME, "max_sustain_time",     iocat::DOUBLE,  "Maximum time (ms) sustain section should last. 0.0 for no restriction."},
    { PRECISION,        "precision",            iocat::WCINT_T, "Number of samples apart the two inputs may trigger to still be considered simultaneous." },
    { NAME,             "name",                 iocat::STRING,  "Name."},
    { NOTE_POS,         "pos",                  iocat::DOUBLE,  "Position of note in bar, relative to TPQN of riff." },
    { NOTE_LEN,         "len",                  iocat::DOUBLE,  "Length of note, relative to TPQN of riff." },
    { NOTE_VEL,         "vel",                  iocat::DOUBLE,  "Velocity of note, consider 1.0 as full velocity." },
    { ADSRSECT,         "name",                 iocat::FIX_STR, "ADSR section to define." },
    { UPTIME,           "ut",                   iocat::DOUBLE,  "Upper shape section time (ms)." },
    { LOTIME,           "lt",                   iocat::DOUBLE,  "Lower shape section time (ms)." },
    { UPLEVEL,          "ul",                   iocat::DOUBLE,  "Upper shape section level." },
    { LOLEVEL,          "ll",                   iocat::DOUBLE,  "Lower shape section level."},
    { UPDEG,            "ud",                   iocat::DOUBLE,  "Upper shape section degree (0 ~ 360)." },
    { LODEG,            "ld",                   iocat::DOUBLE,  "Lower shape section degree (0 ~ 360)." },
    { METER,            "tsig",                 iocat::METER,   "Time signature/Meter, ie 4/4 or 3/8 etc." },
    { BAR,              "bar",                  iocat::WCINT_T, "Bar number (starting at 0)." },
    { BPM,              "bpm",                  iocat::DOUBLE,  "Tempo as Beats Per Minute." },
    { RIFFNAME,         "riff",                 iocat::DOBJ,    "Name of riff to use." },
    { MODNAME,          "name",                 iocat::SYNTHMOD,"Name of module to use." },
    { ROOT_NOTE,        "root_note",            iocat::STRING,  "Root note of sample when playing at its natural rate." },
    { TRANSPOSE,        "transpose",            iocat::WCINT_T, "Number of semitones to transpose the notes (none-fractional)." },
    { SIGIN_LEVEL,      "sigin_level",          iocat::DOUBLE,  "Adjust input signal level, 1.0 = no adjustment." },
    { SIGOUT_UPLEVEL,   "so_ul",                iocat::DOUBLE,  "Upper shape output level." },
    { SIGOUT_LOLEVEL,   "so_ll",                iocat::DOUBLE,  "Lower shape output level." },
    { SECONDS,          "seconds",              iocat::DOUBLE,  "Number of seconds to wait." },
    { MOD_ACTION,       "mod_action",           iocat::FIX_STR, "What to do with modules named in the list." },
    { DOBJ_ACTION,      "dobj_action",          iocat::FIX_STR, "What to do with data objects named in the list."},
    { SNAP_TO,          "snap_to",              iocat::DOUBLE,  "Multiple, relative to TPQN (quarter_value), to snap positions to." },
    { COUNT,            "count",                iocat::WCINT_T, "The number of what-have-yous to do and see." },
    { CHECK_LEVELS,     "check_levels",         iocat::STATE,   "Whether to perform extra checks on the levels or not." },
    { MSG,              "message",              iocat::STRING,  "A message to display during processing." },
    { FORCE_ABORT,      "force_abort",          iocat::STATE,   "Whether to force wcnt to abort after so many detected peaks." },
    { MAXPEAKS,         "max_peaks",            iocat::WCINT_T, "Maximum number of peaks that may be detected before message display stopped or forced abort." },
    { REPEAT,           "repeat",               iocat::WCINT_T, "How many times to repeat the riff along bars in the timeline." },
    { REPEAT_STRIPE,    "repeat_stripe",        iocat::WCINT_T, "Repeat every n bars." },
    { COPYFROM,         "from",                 iocat::STRING,  "Name of module or data object to copy." },
    { COPYTO,           "to",                   iocat::STRING,  "Name of new module or data object created by copy." },
    { POS,              "pos",                  iocat::DOUBLE,  "Position 0.0 ~ 1.0." },
    { BIAS,             "bias",                 iocat::DOUBLE,  "Bias between two signals, 0.0 ~ 1.0." },
    { ORBIT_TYPE,       "orbit_type",           iocat::FIX_STR, "Type of orbit fractal to calculate." },
    { A,                "a",                    iocat::DOUBLE,  "Parameter A of orbit fractal." },
    { B,                "b",                    iocat::DOUBLE,  "Parameter B of orbit fractal." },
    { C,                "c",                    iocat::DOUBLE,  "Parameter C of orbit fractal." },
    { TEST_ITER,        "test_iterations",      iocat::SAMP_T,  "Number of fractal iterations to perform to obtain an output scaling ratio in attempt to limit output within -1.0 ~ +1.0 range." },
    { DATA_FMT,         "data_format",          iocat::FIX_STR, "Data format of audio file to be written." },
    #ifdef WITH_LADSPA
    { GLAME_FILTER_TYPE,"filter_type",          iocat::FIX_STR, "The type of filter to use." },
    { RESONANCE,        "resonance",            iocat::DOUBLE,  "The resonance amount." },
    { RES_MODSIZE,      "res_mod_size",         iocat::DOUBLE,  "Resonance modulation amount." },
    { GAIN_DB,          "gain_db",              iocat::DOUBLE,  "Gain amount, in decibels." },
    { LIMIT_DB,         "limit_db",             iocat::DOUBLE,  "dB Limit." },
    { RELEASE_SECS,     "release_secs",         iocat::DOUBLE,  "Release time (seconds)." },
    { ATTEN_DB,         "attenuation_db",       iocat::DOUBLE,  "Attenuation dB." },
    { THRESH_DB,        "threshold_db",         iocat::DOUBLE,  "Threshold dB." },
    { RATIO_1N,         "ratio_1:n",            iocat::DOUBLE,  "Ratio, 1 to n, where n is..." },
    { KNEE_DB,          "knee_db",              iocat::DOUBLE,  "Knee dB." },
    { MAKEUP_DB,        "makeup_db",            iocat::DOUBLE,  "Make-up dB," },
    { BANDWIDTH,        "bandwidth",            iocat::DOUBLE,  "Bandwidth of input sent through reverb." },
    { BANDWIDTH_MODSIZE,"bandwidth_mod_size",   iocat::DOUBLE,  "Bandwidth modulation amount." },
    { STAGES,           "stages",               iocat::WCINT_T, "Number of filter stages." },
    { FREQ_SHIFT,       "freq_shift",           iocat::DOUBLE,  "Amount to frequency shift." },
    { SHIFT_MODSIZE,    "shift_mod_size",       iocat::DOUBLE,  "Frequency shift modulation amount." },
    { TAIL,             "tail",                 iocat::DOUBLE,  "Length of reverb tail.." },
    { DAMPING,          "damping",              iocat::DOUBLE,  "Amount of reverb damping." },
    #endif
    { ADSR_NAME,        "adsr_name",            iocat::SYNTHMOD,"Name of the ADSR to scale." },
    { ATTACK_SCALE,     "attack_scale",         iocat::DOUBLE,  "Amount to scale the ADSR attack section by."   },
    { DECAY_SCALE,      "decay_scale",          iocat::DOUBLE,  "Amount to scale the ADSR decay section by." },
    { RELEASE_SCALE,    "release_scale",        iocat::DOUBLE,  "Amount to scale the ADSR release section by." },
    { WAVE_TYPE,        "type",                 iocat::FIX_STR, "Waveform type."  },
    { WAVE_SHAPE_TYPE,  "shape_type",           iocat::FIX_STR, "Waveform type of shaping function." },
    { INVERT_ALT,       "invert_alt",           iocat::STATE,   "Whether to invert sign of alternate phases (note: only useful with abnormal-phase-length waveforms)." },
    { SYNC_SHAPE,       "sync_shape",           iocat::STATE,   "Whether to sync shaping waveform."    },
    { MODIFIER_TYPE,    "type",                 iocat::FIX_STR, "Math operator type for applying the two signals together." },
    { SEND_INPUT_OUT,   "send_input_out",       iocat::STATE,   "Whether to directly output the input trigger also." },
    { INSPECT_DF_MSG,   "_float_msg_",          iocat::STRING,  "Message to display when a floating point value is triggered."  },
    { INSPECT_SI_MSG,   "_int_msg_",            iocat::STRING,  "Message to display when a integer value triggered."  },
    { INSPECT_UL_MSG,   "_sample_msg_",         iocat::STRING,  "Message to display when a sample position value triggered."  },
    { INSPECT_ST_MSG,   "_state_msg_",          iocat::STRING,  "Message to display when a state value triggered."  },
    { INSPECT_STR_MSG,  "_string_msg_",         iocat::STRING,  "Message to display when a string value triggered."  },
    { PRE_COUNT,        "pre_count",            iocat::WCINT_T,   "How many triggers to ignore before counting." },
    { WRAP,             "wrap",                 iocat::STATE,   "Whether to start counting over again after reaching desired count of triggers, even if not reset by reset trigger."   },
    { GROUP_NAME,       "group_name",           iocat::STRING,  "Name of group to control."  },
    { FADE_IN_TIME,     "fade_in_time",         iocat::DOUBLE,  "Length of time (ms) that fade in should take."  },
    { FADE_OUT_TIME,    "fade_out_time",        iocat::DOUBLE,  "Length of time (ms) that fade out should take."  },
    { RATE,             "rate",                 iocat::DOUBLE,  "Rate that target level should be reached, ie output = output + (target - output) * rate." },
    { SNAPSHOT_MODE,    "snapshot_mode",        iocat::STATE,   "Whether to append a timestamp to filenames of written audio files." },
    { DROP_CHECK_RANGE, "drop_check_range",     iocat::WCINT_T, "How may degrees apart a pair of vertices may be to considered a collision and for one to be dropped from waveform. " },
    { STR_UNNAMED,      "<unnamed_item>",       iocat::STRING,  "A name of something." },
    { STR_LIST,         "<unnamed_list>",       iocat::STRING,  "A number of names of things." },
    { TIME_IS_RELATIVE, "time_is_relative",     iocat::STATE,   "Whether the specified times should be treated as relative to each other, or as absolute times." }
 };
}; // namespace param
