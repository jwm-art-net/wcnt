#ifndef OUTPUTNAMES_H
#define OUTPUTNAMES_H

#include <string.h>

#include "iocat.h"

class outputnames {
public:
    enum OUT_TYPE
    {
        OUT_FIRST, // used in error situations also
        OUT_NONE_DOUBLE,
        OUT_NONE_SHORT,
        OUT_NONE_ULONG,
        OUT_NONE_TRIG,
        OUT_NONE_STATE,
        OUT_OUTPUT,
        OUT_PLAY_STATE,
        OUT_LEFT,
        OUT_RIGHT,
        OUT_TRIG,
        OUT_PHASE_TRIG,
        OUT_PREMOD_DEG_SIZE,
        OUT_DEG_SIZE,
        OUT_FREQ,
        OUT_NOTE_ON_TRIG,
        OUT_NOTE_SLIDE_TRIG,
        OUT_NOTE_OFF_TRIG,
        OUT_RIFF_START_TRIG,
        OUT_RIFF_END_TRIG,
        OUT_START_TRIG,
        OUT_END_TRIG,
        OUT_VELOCITY,
        OUT_TOTAL_SAMPLE_POS,
        OUT_RIFF_PLAY_STATE,
        OUT_NOTE_PLAY_STATE,
        OUT_OFF_PULSE,
        OUT_L,
        OUT_R,
        OUT_WRITE_STATE,
        OUT_WRITE_START_TRIG,
        OUT_WRITE_END_TRIG,
        OUT_PRE_AMP_MOD,
        OUT_NOT_TRIG,
        // wcnt1.1001
        OUT_BPM,
        OUT_BAR,
        OUT_BAR_TRIG,
        OUT_POS_IN_BAR,
        OUT_POS_STEP_SIZE,
        OUT_SAMPLE_TOTAL,
        OUT_SAMPLE_IN_BAR,
        OUT_BEATS_PER_BAR,
        OUT_BEAT_VALUE,
        OUT_BPM_CHANGE_TRIG,
        OUT_METER_CHANGE_TRIG,
        OUT_BPM_CHANGE_STATE,
        OUT_LOOP_TRIG,
        OUT_NOTENAME,
        OUT_VELOCITY_RAMP,
        OUT_OFF_TRIG,
        OUT_MONO,
        OUT_WET_OUTPUT,
        OUT_RMS,
        OUT_M,
        OUT_RISE_TRIG,
        OUT_FALL_TRIG,
        OUT_TRANSPOSE,
        OUT_NOVALUE,
        OUT_NSVALUE,
        OUT_NOT_NO_TRIG,
        OUT_NOT_NS_TRIG,
        OUT_COUNT,
        OUT_WAIT_STATE,
        OUT_ATTACK_STATE,
        OUT_RELEASE_STATE,
        OUT_LAST // know when to stop
    };
    outputnames();
    ~outputnames();
    char const* get_name(OUT_TYPE) const;
    IOCAT get_category(OUT_TYPE) const;
    OUT_TYPE get_type(char const*) const;
    OUT_TYPE get_nonezerotype(IOCAT);

private:
    char** outname;
    IOCAT* outcat;
};

#endif
