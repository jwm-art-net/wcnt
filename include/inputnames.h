#ifndef INPUTNAMES_H
#define INPUTNAMES_H

#include "getnames.h"
#include "iocat.h"

namespace input
{
 enum TYPE
 {
    ERR_TYPE,
    IN_VELOCITY,
    IN_NOTE_ON_TRIG,
    IN_NOTE_SLIDE_TRIG,
    IN_NOTE_OFF_TRIG,
    IN_LEFT,
    IN_RIGHT,
    IN_FREQ,
    IN_FREQ_MOD1,
    IN_FREQ_MOD2,
    IN_PHASE_TRIG,
    IN_PHASE_STEP,
    IN_SIGNAL,
    IN_EG,
    IN_AMP_MOD,
    IN_PAN_MOD,
    IN_TRIG,
    IN_WAVE,
    IN_PWM,
    IN_CUTOFF_PHASE_STEP,
    IN_PLAY_TRIG,
    IN_START_POS_MOD,
    IN_NORM_MOD,
    IN_V_MOD,
    IN_H_MOD,
    IN_TRIG1,
    IN_TRIG2,
    IN_BAR,
    IN_BAR_TRIG,
    IN_POS_STEP_SIZE,
    IN_WRITE_TRIG,
    IN_STOP_TRIG,
    IN_BPM,
    IN_POWER_MOD,
    IN_RUDE_SWITCH_TRIG,
    IN_NOTENAME,
    IN_GAIN_MOD,
    IN_FEEDBACK,
    IN_FB_MOD,
    IN_BEATS_PER_BAR,
    IN_BEAT_VALUE,
    IN_MODULATION,
    IN_DETRANSPOSE,
    IN_STATE,
    IN_PLAY_STATE,
    IN_RESTART_TRIG,
    IN_RESET_TRIG,
    IN_ALL_OFF_TRIG,
    IN_INDEX,
    IN_BIAS,
    IN_RES_MOD,
    IN_BANDWIDTH_MOD,
    IN_SHIFT_MOD,
    IN_SHAPE_PHASE_STEP,
    IN_SIGNAL1,
    IN_SIGNAL2,
    IN_INSP_DF,
    IN_INSP_DF_TRIG,
    IN_INSP_SI,
    IN_INSP_SI_TRIG,
    IN_INSP_UL,
    IN_INSP_UL_TRIG,
    IN_INSP_ST,
    IN_INSP_ST_TRIG,
    IN_INSP_STR,
    IN_INSP_STR_TRIG,
    LAST_TYPE
 };

 class names : public getnames<TYPE, iocat::TYPE>
 {
  public:
    static void instantiate() { static names inputnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace input

#endif
