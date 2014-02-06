#ifndef OUTPUTNAMES_H
#define OUTPUTNAMES_H

#include "getnames.h"
#include "iocat.h"

namespace output
{
 enum TYPE
 {
    ERR_TYPE,
    OUT_NONE_DOUBLE,
    OUT_NONE_WCINT_T,
    OUT_NONE_SAMP_T,
    OUT_NONE_TRIG,
    OUT_NONE_STATE,
    OUT_NONE_STRING,
    OUT_OUTPUT,
    OUT_PLAY_STATE,
    OUT_LEFT,
    OUT_RIGHT,
    OUT_TRIG,
    OUT_PHASE_TRIG,
    OUT_PREMOD_PHASE_STEP,
    OUT_PHASE_STEP,
    OUT_FREQ,
    OUT_NOTE_ON_TRIG,
    OUT_NOTE_SLIDE_TRIG,
    OUT_NOTE_OFF_TRIG,
    OUT_RIFF_START_TRIG,
    OUT_RIFF_END_TRIG,
    OUT_START_TRIG,
    OUT_END_TRIG,
    OUT_VELOCITY,
    OUT_RIFF_PLAY_STATE,
    OUT_NOTE_PLAY_STATE,
    OUT_OFF_PULSE,
    OUT_WRITE_STATE,
    OUT_WRITE_START_TRIG,
    OUT_WRITE_END_TRIG,
    OUT_PRE_AMP_MOD,
    OUT_NOT_TRIG,
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
    OUT_WET_OUTPUT,
    OUT_RMS,
    OUT_TRANSPOSE,
    OUT_NOVALUE,
    OUT_NSVALUE,
    OUT_NOT_NO_TRIG,
    OUT_NOT_NS_TRIG,
    OUT_COUNT,
    OUT_INDEX,
    OUT_WAIT_STATE,
    OUT_ATTACK_STATE,
    OUT_RELEASE_STATE,
    OUT_X,
    OUT_Y,
    OUT_UP,
    OUT_DOWN,
    OUT_PRE_COUNT,
    OUT_PRE_SHAPE_OUTPUT,
    OUT_THROUGH,
    LAST_TYPE
 };

 class names : public getnames<int, iocat::TYPE>
 {
  public:
    static void instantiate() { static names outputnames; }
    static TYPE get_off_type(iocat::TYPE);

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace output

#endif
