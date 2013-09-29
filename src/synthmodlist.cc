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
 search_type(module::ERR_TYPE),
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
    module::TYPE smt, const char* uname)
{
    switch (smt) {
    case module::WCNTEXIT:       return new wcnt_exit(uname);
    case module::ADSR:           return new adsr(uname);
    case module::STEREOAMP:      return new stereo_amp(uname);
    case module::CLOCK:          return new clockclock(uname);
    case module::CONSTMOD:       return new constmod(uname);
    case module::FREQGEN:        return new freq_generator(uname);
    case module::LFOCLOCK:       return new lfo_clock(uname);
    case module::LFOCONTROL:     return new lfo_controller(uname);
    case module::MODIFIER:       return new modifier(uname);
    case module::NOISEGEN:       return new noise_generator(uname);
    case module::OSCCLOCK:       return new osc_clock(uname);
    case module::SAMPLEHOLD:     return new sample_hold(uname);
    case module::SEQUENCER:      return new sequencer(uname);
    case module::SQUAREWAVE:     return new square_wave(uname);
    case module::TRIGGER:        return new trigger(uname);
    case module::TRIWAVE2:       return new triangle_wave2(uname);
    case module::USERWAVE:       return new user_wave(uname);
    case module::SAMPLER:        return new sampler(uname);
    case module::WAVFILEOUT:     return new wavfileout(uname);
    case module::STEREOCHANNEL:  return new stereo_channel(uname);
    case module::STEREOMIXER:    return new stereomixer(uname);
    case module::RANDTRIGGER:    return new randomtrigger(uname);
    case module::LOGICTRIGGER:   return new logictrigger(uname);
    case module::SWITCHER:       return new switcher(uname);
    case module::WCNTSIGNAL:     return new wcnt_signal(uname);
    case module::COMBINER:       return new combiner(uname);
    case module::TIMEMAP:        return new timemap(uname);
    case module::SERIALWAVFILEOUT:
        return new serialwavfileout(uname);
    case module::CONTRASTER:     return new contraster(uname);
    case module::DELAY:          return new delay(uname);
    case module::ECHO:           return new echo(uname);
    case module::MONOAMP:        return new mono_amp(uname);
    case module::MULTIPLIER:     return new multiplier(uname);
    case module::RANGELIMIT:     return new range_limit(uname);
    case module::PAN:            return new pan(uname);
    case module::RMS:            return new rms(uname);
    case module::DCFILTER:       return new dc_filter(uname);
    case module::DYNAMIC:        return new dynamic(uname);
    case module::SPREADER:       return new spreader(uname);
    case module::NOTETRAN:       return new notetran(uname);
    case module::WAITTRIG:       return new waittrig(uname);
    case module::PATTERNTRIG:    return new patterntrig(uname);
    case module::STATEGATETRIG:  return new stategatetrig(uname);
    case module::INVERT:         return new invert(uname);
    case module::TIMER:          return new timer(uname);
    case module::SYNCCLOCK:      return new sync_clock(uname);
    case module::WCNTTRIGGER:    return new wcnt_trigger(uname);
    case module::TRIGSWITCHER:   return new trigswitcher(uname);
    case module::ONOFFTRIG:      return new onofftrig(uname);
    case module::PEAKDETECTOR:   return new peak_detector(uname);
    case module::STEPPER:        return new stepper(uname);
    case module::ADDER:          return new adder(uname);
    case module::SUBTRACTER:     return new subtracter(uname);
    case module::TRIGDELAY:      return new trigdelay(uname);
    case module::SIMPLEDELAY:    return new simple_delay(uname);
    case module::DIFFFILTER:     return new diff_filter(uname);
    case module::IMPULSE:        return new impulse(uname);
    case module::ORBIT:          return new orbit(uname);
    #ifdef WITH_LADSPA
    case module::GLAME_BUTTERWORTH:
        return new glame_butterworth(uname);
    case module::FAST_LOOKAHEAD_LIMITER:
        return new fast_lookahead_limiter(uname);
    case module::DC_OFFSET_REMOVER:
        return new dc_offset_remover(uname);
    case module::SC1:
        return new sc1(uname);
    case module::SINGLE_BAND_PARA:
        return new single_band_para(uname);
    case module::GLAME_FILTER:
        return new glame_filter(uname);
    case module::BODE_FREQ_SHIFTER:
        return new bode_freq_shifter(uname);
    case module::CAPS_PLATE:
        return new caps_plate(uname);
    case module::CAPS_PLATE2X2:
        return new caps_plate2x2(uname);
    #endif // WITH_LADSPA
    case module::WAVE:           return new wave(uname);
    case module::WAVE_PHASE:     return new wave_phase(uname);
    case module::CONSTANT_FREQ:  return new constant_freq(uname);
    case module::CONSTANT_NOTE:  return new constant_note(uname);
    case module::BALANCE:        return new balance(uname);
    case module::TRIGECHO:       return new trigecho(uname);
    case module::INSPECT:        return new inspect(uname);
    case module::TRIGCOUNTER:    return new trigcounter(uname);
    case module::TRIGROUTER:     return new trigrouter(uname);
    case module::GROUPCONTROL:   return new group_control(uname);
    case module::FADER:          return new fader(uname);
    case module::SAMPLECLIMB:    return new sample_climb(uname);
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
synthmodlist::duplicate_group(const char* from, const char* to)
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

