#ifndef SYNTHMODNAMES_H
#define SYNTHMODNAMES_H

#include "getnames.h"

namespace synthmod
{
 enum TYPE
 {
    ERR_TYPE,
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
    SIMPLEDELAY,
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
    DIFFFILTER,
    IMPULSE,
    ORBIT,
    GLAME_BUTTERWORTH,
    FAST_LOOKAHEAD_LIMITER,
    DC_OFFSET_REMOVER,
    SC1,
    SINGLE_BAND_PARA,
    GLAME_FILTER,
    BODE_FREQ_SHIFTER,
    CAPS_PLATE,
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
    WCNT_NOTE,
    SEQ_ROUTER,
    SEQ_ECHO,
    LAST_TYPE
 };

 class names : public getnames<TYPE, void*>
 {
  public:
    static void instantiate() { static names synthmodnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace synthmod

#endif
