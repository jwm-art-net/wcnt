#ifndef INPUTNAMES_H
#define INPUTNAMES_H

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
        #ifdef WITH_LADSPA
        IN_RES_MOD,
        IN_THRESH_MOD,
        IN_BANDWIDTH_MOD,
        IN_SHIFT_MOD,
        #endif
        IN_SHAPE_PHASE_STEP,
        IN_FREQ_MOD,
        IN_SIGNAL1,
        IN_SIGNAL2,
        /* inspect module's iocat type inputs etc */
        IN__DF,
        IN__DF_TRIG,
        IN__SI,
        IN__SI_TRIG,
        IN__UL,
        IN__UL_TRIG,
        IN__ST,
        IN__ST_TRIG,
        IN__STR,
        IN__STR_TRIG,
        IN_RESET_TRIG,
        IN_COUNT,
        IN_BIAS,
        // used for array to know how many:
        IN_LAST
    };
    inputnames();
    ~inputnames(){};
    const char*     get_name(IN_TYPE) const;
    iocat::IOCAT    get_category(IN_TYPE) const;
    IN_TYPE         get_type(const char* const) const;

private:
    struct input_data
    {
        IN_TYPE type;
        const char* const name;
        iocat::IOCAT cat;
    };
    static const input_data data[IN_LAST];
};

#endif
