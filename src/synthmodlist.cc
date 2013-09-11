#include "../include/synthmodlist.h"
#include "../include/modules.h"
#include "../include/jwm_globals.h"
#include "../include/dobjlist.h"
#include "../include/group.h"
#include "../include/stockerrs.h"

#include <iostream>

nonezero* synthmodlist::off = 0;

synthmodlist::synthmodlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d),
 emptyrunlist(0),
 search_type(synthmodnames::FIRST),
 search_result(0)
{
    // nonezero module is not user accessable.
    // it's outputs are used when ever a module's input is set to off.
    // only need one for the main list (ie the first synthmodlist created)
    if (!off) {
        off = new nonezero("off");
        add_at_head(off);
    }
}

synthmodlist::~synthmodlist()
{
    if (emptyrunlist)
        delete emptyrunlist;
}

synthmod* 
synthmodlist::create_module(
    synthmodnames::SYNTH_MOD_TYPE smt, char const* uname)
{
    switch (smt) {
    case synthmodnames::WCNTEXIT:       return new wcnt_exit(uname);
    case synthmodnames::ADSR:           return new adsr(uname);
    case synthmodnames::STEREOAMP:      return new stereo_amp(uname);
    case synthmodnames::CLOCK:          return new clockclock(uname);
    case synthmodnames::CONSTMOD:       return new constmod(uname);
    case synthmodnames::FREQGEN:        return new freq_generator(uname);
    case synthmodnames::LFOCLOCK:       return new lfo_clock(uname);
    case synthmodnames::LFOCONTROL:     return new lfo_controller(uname);
    case synthmodnames::MODIFIER:       return new modifier(uname);
    case synthmodnames::NOISEGEN:       return new noise_generator(uname);
    case synthmodnames::OSCCLOCK:       return new osc_clock(uname);
    case synthmodnames::SAMPLEHOLD:     return new sample_hold(uname);
    case synthmodnames::SEQUENCER:      return new sequencer(uname);
    case synthmodnames::SQUAREWAVE:     return new square_wave(uname);
    case synthmodnames::TRIGGER:        return new trigger(uname);
    case synthmodnames::TRIWAVE2:       return new triangle_wave2(uname);
    case synthmodnames::USERWAVE:       return new user_wave(uname);
    case synthmodnames::SAMPLER:        return new sampler(uname);
    case synthmodnames::WAVFILEOUT:     return new wavfileout(uname);
    case synthmodnames::STEREOCHANNEL:  return new stereo_channel(uname);
    case synthmodnames::STEREOMIXER:    return new stereomixer(uname);
    case synthmodnames::RANDTRIGGER:    return new randomtrigger(uname);
    case synthmodnames::LOGICTRIGGER:   return new logictrigger(uname);
    case synthmodnames::SWITCHER:       return new switcher(uname);
    case synthmodnames::WCNTSIGNAL:     return new wcnt_signal(uname);
    case synthmodnames::COMBINER:       return new combiner(uname);
    case synthmodnames::TIMEMAP:        return new timemap(uname);
    case synthmodnames::SERIALWAVFILEOUT:
        return new serialwavfileout(uname);
    case synthmodnames::CONTRASTER:     return new contraster(uname);
    case synthmodnames::DELAY:          return new delay(uname);
    case synthmodnames::ECHO:           return new echo(uname);
    case synthmodnames::MONOAMP:        return new mono_amp(uname);
    case synthmodnames::MULTIPLIER:     return new multiplier(uname);
    case synthmodnames::RANGELIMIT:     return new range_limit(uname);
    case synthmodnames::PAN:            return new pan(uname);
    case synthmodnames::RMS:            return new rms(uname);
    case synthmodnames::DCFILTER:       return new dc_filter(uname);
    case synthmodnames::DYNAMIC:        return new dynamic(uname);
    case synthmodnames::SPREADER:       return new spreader(uname);
    case synthmodnames::NOTETRAN:       return new notetran(uname);
    case synthmodnames::WAITTRIG:       return new waittrig(uname);
    case synthmodnames::PATTERNTRIG:    return new patterntrig(uname);
    case synthmodnames::STATEGATETRIG:  return new stategatetrig(uname);
    case synthmodnames::INVERT:         return new invert(uname);
    case synthmodnames::TIMER:          return new timer(uname);
    case synthmodnames::SYNCCLOCK:      return new sync_clock(uname);
    case synthmodnames::WCNTTRIGGER:    return new wcnt_trigger(uname);
    case synthmodnames::TRIGSWITCHER:   return new trigswitcher(uname);
    case synthmodnames::ONOFFTRIG:      return new onofftrig(uname);
    case synthmodnames::PEAKDETECTOR:   return new peak_detector(uname);
    case synthmodnames::STEPPER:        return new stepper(uname);
    case synthmodnames::ADDER:          return new adder(uname);
    case synthmodnames::SUBTRACTER:     return new subtracter(uname);
    case synthmodnames::TRIGDELAY:      return new trigdelay(uname);
    case synthmodnames::SIMPLEDELAY:    return new simple_delay(uname);
    case synthmodnames::DIFFFILTER:     return new diff_filter(uname);
    case synthmodnames::IMPULSE:        return new impulse(uname);
    case synthmodnames::ORBIT:          return new orbit(uname);
    #ifdef WITH_LADSPA
    case synthmodnames::GLAME_BUTTERWORTH:
        return new glame_butterworth(uname);
    case synthmodnames::FAST_LOOKAHEAD_LIMITER:
        return new fast_lookahead_limiter(uname);
    case synthmodnames::DC_OFFSET_REMOVER:
        return new dc_offset_remover(uname);
    case synthmodnames::SC1:
        return new sc1(uname);
    case synthmodnames::SINGLE_BAND_PARA:
        return new single_band_para(uname);
    case synthmodnames::GLAME_FILTER:
        return new glame_filter(uname);
    case synthmodnames::BODE_FREQ_SHIFTER:
        return new bode_freq_shifter(uname);
    case synthmodnames::CAPS_PLATE:
        return new caps_plate(uname);
    case synthmodnames::CAPS_PLATE2X2:
        return new caps_plate2x2(uname);
    #endif // WITH_LADSPA
    case synthmodnames::WAVE:           return new wave(uname);
    case synthmodnames::WAVE_PHASE:     return new wave_phase(uname);
    case synthmodnames::CONSTANT_FREQ:  return new constant_freq(uname);
    case synthmodnames::CONSTANT_NOTE:  return new constant_note(uname);
    case synthmodnames::BALANCE:        return new balance(uname);
    case synthmodnames::TRIGECHO:       return new trigecho(uname);
    case synthmodnames::INSPECT:        return new inspect(uname);
    case synthmodnames::TRIGCOUNTER:    return new trigcounter(uname);
    case synthmodnames::TRIGROUTER:     return new trigrouter(uname);
    case synthmodnames::GROUPCONTROL:   return new group_control(uname);
    case synthmodnames::FADER:          return new fader(uname);
    case synthmodnames::SAMPLECLIMB:    return new sample_climb(uname);
    default:
        return 0;
    }
}

bool
synthmodlist::delete_module(synthmod * const sm)
{
    llitem *tmp = find_data(sneak_first(), sm);
    if (tmp) {
        delete unlink_item(tmp);
        delete sm;
        return true;
    }
    return false;
}

synthmodlist::linkedlist*
synthmodlist::duplicate_group(char const* from, char const* to)
{
    linkedlist* grplist = new linkedlist(MULTIREF_OFF, PRESERVE_DATA);
    llitem* i = find_in_data(sneak_first(), groupname(from));
    while(i) {
        synthmod* frommod = i->get_data();
        const char* const
            togrp = set_groupname(to, frommod->get_username());
        synthmod* tomod
            = frommod->duplicate_module(togrp, synthmod::NO_CONNECT);
        delete [] togrp;
        if (!add_module(tomod)) {
            if (tomod)
                delete tomod;
            return 0;
        }
        grplist->add_at_tail(tomod);
        i = find_in_data(i->get_next(), groupname(from));
    }
    return grplist;
}

