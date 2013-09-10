#ifndef SYNTHMODNAMES_H
#define SYNTHMODNAMES_H

#include <string.h>

class synthmodnames
{
public:
    enum SYNTH_MOD_TYPE
    {
        FIRST,
        NONEZERO,
        ADSR,
        STEREOAMP,
        CLOCK,
        CONSTMOD,
        FREQGEN,
        LFOCLOCK,
        LFOCONTROL,
        MODIFIER,
        NOISEGEN,
        OSCCLOCK,
        SAMPLEHOLD,
        SEQUENCER,
        SQUAREWAVE,
        TRIGGER,
        TRIWAVE2,
        USERWAVE,
        SAMPLER,
        WAVFILEOUT,
        STEREOCHANNEL,
        STEREOMIXER,
        RANDTRIGGER,
        LOGICTRIGGER,
        SWITCHER,
        WCNTSIGNAL,
        COMBINER,
        TIMEMAP,
        CONTRASTER,
        SERIALWAVFILEOUT,
        DELAY,
        ECHO,
        MONOAMP,
        WCNTEXIT,
        MULTIPLIER,
        RANGELIMIT,
        PAN,
        RMS,
        DCFILTER,
        DYNAMIC,
        SPREADER,
        NOTETRAN,
        WAITTRIG,
        PATTERNTRIG,
        STATEGATETRIG,
        INVERT,
        TIMER,
        SYNCCLOCK,
        WCNTTRIGGER,
        TRIGSWITCHER,
        ONOFFTRIG,
        PEAKDETECTOR,
        STEPPER,
        ADDER,
        SUBTRACTER,
        TRIGDELAY,
        SIMPLEDELAY,
        DIFFFILTER,
        IMPULSE,
        ORBIT,
        #ifdef WITH_LADSPA
        GLAME_BUTTERWORTH,
        FAST_LOOKAHEAD_LIMITER,
        DC_OFFSET_REMOVER,
        SC1,
        SINGLE_BAND_PARA,
        GLAME_FILTER,
        BODE_FREQ_SHIFTER,
        CAPS_PLATE,
        CAPS_PLATE2X2,
        #endif
        WAVE,
        WAVE_PHASE,
        CONSTANT_FREQ,
        CONSTANT_NOTE,
        BALANCE,
        TRIGECHO,
        INSPECT,
        TRIGCOUNTER,
        TRIGROUTER,
        GROUPCONTROL,
        FADER,
        SAMPLECLIMB,
        LAST
    };
    synthmodnames();
    ~synthmodnames(){};
    const char* const get_name(SYNTH_MOD_TYPE id) const;
    SYNTH_MOD_TYPE get_type(const char* const name) const;

private:
    struct mod_data
    {
        SYNTH_MOD_TYPE type;
        const char* const name;
    };
    static const mod_data data[LAST];
};

#endif
