#ifndef SYNTHMODNAMES_H
#define SYNTHMODNAMES_H

#include <string.h>

class synthmodnames
{
public:
    enum SYNTH_MOD_TYPE
    {
        MOD_FIRST,
        MOD_NONEZERO,
        MOD_ADSR,
        MOD_STEREOAMP,
        MOD_CLOCK,
        MOD_CONSTMOD,
        MOD_FREQGEN,
        MOD_LFOCLOCK,
        MOD_LFOCONTROL,
        MOD_LPFILTER,
        MOD_MODIFIER,
        MOD_NOISEGEN,
        MOD_OSCCLOCK,
        MOD_SAMPLEHOLD,
        MOD_SEQUENCER,
        MOD_SINEWAVE,
        MOD_SQUAREWAVE,
        MOD_TRIGGER,
        MOD_TRIWAVE,
        MOD_TRIWAVE2,
        MOD_USERWAVE,
        MOD_SAMPLER,
        MOD_WAVFILEOUT,
        MOD_STEREOCHANNEL,
        MOD_STEREOMIXER,
        MOD_RANDTRIGGER,
        MOD_LOGICTRIGGER,
        MOD_SWITCHER,
        MOD_WCNTSIGNAL,
        MOD_HPFILTER,
        MOD_COMBINER,
        MOD_TIMEMAP,
        MOD_CONTRASTER,
        MOD_SERIALWAVFILEOUT,
        MOD_DELAY,
        MOD_ECHO,
        MOD_MONOAMP,
        MOD_WCNTEXIT,
        MOD_MULTIPLIER,
        MOD_RANGELIMIT,
        MOD_PAN,
        MOD_RMS,
        MOD_DCFILTER,
        MOD_DYNAMIC,
        MOD_SPREADER,
        MOD_NOTETRAN,
        MOD_WAITTRIG,
        MOD_PATTERNTRIG,
        MOD_STATEGATETRIG,
        MOD_INVERT,
        MOD_TIMER,
        MOD_SYNCCLOCK,
        MOD_WCNTTRIGGER,
        MOD_TRIGSWITCHER,
        MOD_ONOFFTRIG,
        MOD_PEAKDETECTOR,
        MOD_STEPPER,
        MOD_ADDER,
        MOD_SUBTRACTER,
        MOD_TRIGDELAY,
        MOD_SIMPLEDELAY,
        MOD_LAST
    };
    synthmodnames();
    ~synthmodnames();
    char const* get_name(SYNTH_MOD_TYPE id);
    SYNTH_MOD_TYPE get_type(char const*);

private:
    char** mod_name;
};

#endif
