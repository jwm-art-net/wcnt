#ifndef INPUTNAMES_H
#define INPUTNAMES_H

#ifndef BARE_MODULES

#include <string.h>
#include "iocat.h"

class inputnames
{
public:
    enum IN_TYPE
    {
        IN_FIRST, // <-- returned on error
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
        IN_DEG_SIZE,
        IN_SIGNAL,
        IN_EG,
        IN_AMP_MOD,
        IN_PAN_MOD,
        IN_TRIG,
        IN_WAVE,
        IN_PWM,
        IN_CUTOFF_DEG_SIZE,
        // IN_RES_AMP_MOD,  ***removed***
        IN_PLAY_TRIG,
        IN_START_POS_MOD,
        IN_MODIFIER,
        IN_NORM_MOD,
        IN_V_MOD,
        IN_H_MOD,
        IN_TRIG1,
        IN_TRIG2,
        // wcnt 1.1001 :-
        IN_BAR,
        IN_BAR_TRIG,
        IN_POS_STEP_SIZE,
        IN_WRITE_TRIG,
        IN_STOP_TRIG,
        IN_BPM,
        IN_POWER_MOD,
        IN_RUDE_SWITCH_TRIG,
        IN_NOTENAME,
        IN_L,
        IN_R,
        IN_GAIN_MOD,
        IN_FEEDBACK,
        IN_FB_MOD,
        IN_BEATS_PER_BAR,
        IN_BEAT_VALUE,
        IN_MODULATION,
        IN_DETRANSPOSE,
        IN_STATE,
        // used for array to know how many:
        IN_LAST
    };
    inputnames();
    ~inputnames();
    char const* get_name(IN_TYPE);
    IOCAT get_category(IN_TYPE);
    IN_TYPE get_type(char const*);

private:
    char** inname;
    IOCAT* incat;
};

#endif
#endif
