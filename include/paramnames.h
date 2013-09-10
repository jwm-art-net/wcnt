#ifndef paramnames_H
#define paramnames_H

#ifndef BARE_MODULES

#include <string.h>
#include "iocat.h"

class paramnames
{
public:
    enum PAR_TYPE
    {
        PAR_FIRST = 0,
        PAR_SUSTAIN_STATUS,
        PAR_ZERO_RETRIGGER,
        PAR_NOTELEN_FREQ,
        PAR_FREQ,
        PAR_FREQ_MOD1SIZE,
        PAR_FREQ_MOD2SIZE,
        PAR_VALUE,
        PAR_SIG_RANGE_HI,
        PAR_SIG_RANGE_LO,
        PAR_FREQ_RANGE_HI,
        PAR_FREQ_RANGE_LO,
        PAR_STEP_COUNT,
        PAR_DELAY_TIME,
        PAR_RAMP_TIME,
        PAR_START_LEVEL,
        PAR_END_LEVEL,
        PAR_AMP_MODSIZE,
        PAR_MODIFIER_FUNC,
        PAR_MODIFIER_SIZE,
        PAR_MIN_OUT,
        PAR_MAX_OUT,
        PAR_POSNEG_MIRROR,
        PAR_FREQ_MOD1_LO,
        PAR_FREQ_MOD1_HI,
        PAR_FREQ_MOD2_LO,
        PAR_FREQ_MOD2_HI,
        PAR_OCTAVE,
        PAR_PORTAMENTO,
        PAR_DECAY_TIME,
        PAR_START_POS_MIN,
        PAR_START_POS_MAX,
        PAR_LOOP_BEGIN,
        PAR_LOOP_END,
        PAR_LOOP_MODE,
        PAR_ANTI_CLIP,
        PAR_DEGSIZE_AMOUNT,
        PAR_RECYCLE_MODE,
        PAR_TRIG_ZERO_DEG,
        PAR_CYCLES,
        PAR_PULSE_WIDTH,
        PAR_PWM_SIZE,
        PAR_AMPLITUDE,
        PAR_PAN,
        PAR_PAN_MODSIZE,
        PAR_CLIP_LEVEL,
        PAR_MASTER_LEVEL,
        PAR_NORM_FREQ,
        PAR_NORM_MODSIZE,
        PAR_TRIGGER_LEVEL,
        PAR_FILENAME,
        PAR_WAVFILEIN,
        PAR_VELOCITY_RESPONSE,
        PAR_PROBABILITY,
        PAR_NOTPROBABILITY,
        PAR_LOGICFUNC,
        PAR_XFADE_TIME,
        PAR_TUNING_SEMITONES,
        PAR_MEAN_TOTAL,
        // wcnt-1.1001
        PAR_WAV_BASENAME,
        PAR_PLAY_DIR,
        PAR_PLAY_MODE,
        PAR_JUMP_MODE,
        PAR_LOOP_IS_OFFSET,
        PAR_LOOP_BI_OFFSET,
        PAR_AC_EACH_END,
        PAR_HOLD_NOTENAME,
        PAR_POWER_MIN,
        PAR_POWER_MAX,
        PAR_RUDE_MODE,
        PAR_WETDRY,
        PAR_START_BAR,
        PAR_END_BAR,
        PAR_ZERO_SEARCH_RANGE,
        PAR_GAIN,
        PAR_GAIN_MODSIZE,
        PAR_FEED_LEVEL,
        PAR_FEED_MODSIZE,
        PAR_EXIT_BAR,
        PAR_RESPONSE_TIME,
        PAR_LEFT_AMPLITUDE,
        PAR_RIGHT_AMPLITUDE,
        PAR_RMS_TIME,
        PAR_DC_TIME,
        PAR_UP_THRESH,
        PAR_LO_THRESH,
        PAR_LEVEL,
        PAR_ATTACK_TIME,
        PAR_ATTACK_LEVEL,
        PAR_RELEASE_TIME,
        PAR_RELEASE_LEVEL,
        PAR_INDEPENDANT,
        PAR_USE_RATIOS,
        PAR_NO_LONOTE,
        PAR_NO_HINOTE,
        PAR_NS_LONOTE,
        PAR_NS_HINOTE,
        PAR_MINNO_OUT,
        PAR_MAXNO_OUT,
        PAR_MINNS_OUT,
        PAR_MAXNS_OUT,
        PAR_DETRAN_NO,
        PAR_DETRAN_NS,
        PAR_NO_RESPTIME,
        PAR_NS_RESPTIME,
        PAR_MIN_WAIT,
        PAR_MAX_WAIT,
        PAR_TRIG_STRING,
        PAR_LAST
    };

    paramnames();
    ~paramnames();
    char const* get_name(PAR_TYPE);
    IOCAT get_category(PAR_TYPE);
    PAR_TYPE get_type(char const*);
private:
    char** parname;
    IOCAT* parcat;

};

#endif
#endif
