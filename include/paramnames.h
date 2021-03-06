#ifndef PARAMNAMES_H
#define PARAMNAMES_H

#include "getnames.h"
#include "iocat.h"

namespace param
{
 enum TYPE
 {
    ERR_TYPE,
    SUSTAIN_STATUS,
    ZERO_RETRIGGER,
    FREQ,
    FREQ_MOD1SIZE,
    FREQ_MOD2SIZE,
    VALUE,
    SIG_RANGE_HI,
    SIG_RANGE_LO,
    FREQ_RANGE_HI,
    FREQ_RANGE_LO,
    STEP_COUNT,
    DELAY_TIME,
    RAMP_TIME,
    START_LEVEL,
    END_LEVEL,
    AMP_MODSIZE,
    FUNC,
    POSNEG_MIRROR,
    OCTAVE,
    PORTAMENTO,
    DECAY_TIME,
    START_POS,
    START_POS_MIN,
    START_POS_MAX,
    LOOP_BEGIN,
    LOOP_END,
    LOOP_MODE,
    XFADE_SAMPLES,
    PHASE_STEP_AMOUNT,
    RECYCLE_MODE,
    TRIG_RESET_PHASE,
    CYCLES,
    PULSE_WIDTH,
    PWM_SIZE,
    PAN,
    PAN_MODSIZE,
    CLIP_LEVEL,
    MASTER_LEVEL,
    NORM_FREQ,
    NORM_MODSIZE,
    TRIGGER_LEVEL,
    FILENAME,
    WAVFILEIN,
    VELOCITY_RESPONSE,
    PROBABILITY,
    NOTPROBABILITY,
    LOGICFUNC,
    XFADE_TIME,
    TUNING_SEMITONES,
    MEAN_TOTAL,
    WAV_BASENAME,
    PLAY_DIR,
    PLAY_MODE,
    JUMP_MODE,
    LOOP_IS_OFFSET,
    LOOP_BI_OFFSET,
    XFADE_EACH_END,
    POWER,
    POWER_MIN,
    POWER_MAX,
    RUDE_MODE,
    WETDRY,
    START_BAR,
    END_BAR,
    ZERO_SEARCH_RANGE,
    GAIN,
    GAIN_MODSIZE,
    FEED_LEVEL,
    FEED_MODSIZE,
    EXIT_BAR,
    RESPONSE_TIME,
    LEFT_LEVEL,
    RIGHT_LEVEL,
    RMS_TIME,
    DC_TIME,
    UP_THRESH,
    LO_THRESH,
    LEVEL,
    ATTACK_TIME,
    ATTACK_LEVEL,
    RELEASE_TIME,
    RELEASE_LEVEL,
    USE_RATIOS,
    NO_LONOTE,
    NO_HINOTE,
    NS_LONOTE,
    NS_HINOTE,
    MINNO_OUT,
    MAXNO_OUT,
    MINNS_OUT,
    MAXNS_OUT,
    DETRAN_NO,
    DETRAN_NS,
    NO_RESPTIME,
    NS_RESPTIME,
    MIN_WAIT,
    MAX_WAIT,
    TRIG_STRING,
    QUARTER_VAL,
    RELEASE_RATIO,
    MIN_TIME,
    MAX_SUSTAIN_TIME,
    PRECISION,
    NAME,
    NOTE_POS,
    NOTE_LEN,
    NOTE_VEL,
    ADSRSECT,
    TIME,
    UPTIME,
    LOTIME,
    UPLEVEL,
    LOLEVEL,
    DEG,
    UPDEG,
    LODEG,
    METER,
    BAR,
    BPM,
    RIFFNAME,
    MODNAME,
    ROOT_NOTE,
    TRANSPOSE,
    SIGIN_LEVEL,
    SIGOUT_LEVEL,
    SIGOUT_UPLEVEL,
    SIGOUT_LOLEVEL,
    SECONDS,
    MOD_ACTION,
    DOBJ_ACTION,
    SNAP_TO,
    COUNT,
    CHECK_LEVELS,
    MSG,
    FORCE_ABORT,
    MAXPEAKS,
    REPEAT,
    REPEAT_STRIPE,
    COPYFROM,
    COPYTO,
    POS,
    BIAS,
    ORBIT_TYPE,
    A,
    B,
    C,
    TEST_ITER,
    DATA_FMT,
    GLAME_FILTER_TYPE,
    RESONANCE,
    RES_MODSIZE,
    GAIN_DB,
    LIMIT_DB,
    RELEASE_SECS,
    THRESH_DB,
    RATIO_1N,
    KNEE_DB,
    MAKEUP_DB,
    BANDWIDTH,
    BANDWIDTH_MODSIZE,
    STAGES,
    FREQ_SHIFT,
    SHIFT_MODSIZE,
    TAIL,
    DAMPING,
    ADSR_NAME,
    ATTACK_SCALE,
    DECAY_SCALE,
    RELEASE_SCALE,
    WAVE_TYPE,
    WAVE_SHAPE_TYPE,
    INVERT_ALT,
    SYNC_SHAPE,
    MODIFIER_TYPE,
    SEND_INPUT_OUT,
    INSPECT_DF_MSG,
    INSPECT_SI_MSG,
    INSPECT_UL_MSG,
    INSPECT_ST_MSG,
    INSPECT_STR_MSG,
    PRE_COUNT,
    WRAP,
    GROUP_NAME,
    FADE_IN_TIME,
    FADE_OUT_TIME,
    RATE,
    SNAPSHOT_MODE,
    DROP_CHECK_RANGE,
    STR_UNNAMED,
    STR_LIST,
    TIME_IS_RELATIVE,
    LAST_TYPE
 };

 class names : public getnames<TYPE, iocat::TYPE>
 {
  public:
    static void instantiate() { static names paramnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace param

#endif
