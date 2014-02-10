#ifndef MODULES_H
#define MODULES_H

// version 1.0
#include "adsr.h"
#include "clockclock.h"
#include "constmod.h"
#include "freqgenerator.h"
#include "lfoclock.h"
#include "lfocontroller.h"
#include "modifier.h"
#include "noisegenerator.h"
#include "oscclock.h"
#include "samplehold.h"
#include "sampler.h"
#include "sequencer.h"
#include "squarewave.h"
#include "stereoamp.h"
#include "stereochannel.h"
#include "stereomixer.h"
#include "trianglewave2.h"
#include "trigger.h"
#include "userwave.h"
#include "wavfileout.h"
#include "randomtrigger.h"
#include "logictrigger.h"
#include "switcher.h"
#include "combiner.h"
#include "nonezero.h"
// version 1.1000
#include "timemap.h"
#include "serialwavfileout.h"
#include "contraster.h"
#include "delay.h"
#include "echo.h"
#include "monoamp.h"
#include "wcntexit.h"
#include "multiplier.h"
#include "rangelimit.h"
#include "pan.h"
#include "rms.h"
#include "dc_filter.h"
// version 1.1z
#include "dynamic.h"
#include "spreader.h"
#include "notetran.h"
// version 1.127
#include "waittrig.h"
#include "patterntrig.h"
#include "stategatetrig.h"
#include "invert.h"
#include "timer.h"
// version 1.128
#include "syncclock.h"
// version 1.2
#include "wcnttrigger.h"
#include "trigswitcher.h"
#include "onofftrig.h"
// version 1.25
#include "peakdetector.h"
#include "stepper.h"
#include "adder.h"
#include "subtracter.h"
#include "trigdelay.h"
#include "simpledelay.h"
// wcnt-1.26
#include "difffilter.h"
#include "impulse.h"
#include "orbit.h"

#ifdef WITH_LADSPA // hardcoded ladspa plugin modules
#include "glame_butterworth.h"
#include "fast_lookahead_limiter.h"
#include "dc_offset_remover.h"
#include "sc1.h"
#include "single_band_para.h"
#include "glame_filter.h"
#include "bode_freq_shifter.h"
#include "caps_plate2x2.h"
#endif

#include "wave.h"
#include "wave_phase.h"
#include "constant_freq.h"
#include "constant_note.h"
#include "balance.h"
#include "trigecho.h"
#include "inspect.h"
#include "trigcounter.h"
#include "trigrouter.h"
#include "groupcontrol.h"
#include "fader.h"
#include "sampleclimb.h"

//--------------

#include "seqecho.h"
#include "seqrouter.h"
#include "wcnt_note.h"

#include "ladspa_module.h"

#endif
