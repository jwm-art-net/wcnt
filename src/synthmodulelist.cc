#ifndef SYNTHMODULELIST_H
#include "../include/synthmodulelist.h"
#include "../include/modules.h"
#include "../include/jwm_globals.h"

#include <iostream>

synthmodlist::synthmodlist() :
 smodlist(0), constlist(0), smod_item(0), smod(0),
 search_modtype(synthmodnames::FIRST), search_result(0)
{
    smodlist =
        new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    // nonezero module is not user accessable.
    // it's outputs are used when ever a module's input is set to off.
    smod = new nonezero("off");
    smodlist->add_at_head(smod);
}

synthmodlist::~synthmodlist()
{
    goto_first();
    while (smod) {
        delete smod;
        goto_next();
    }
    delete smodlist;
    if (constlist) {
        smodlist = constlist;
        goto_first();
        while (smod) {
            delete smod;
            goto_next();
        }
        delete constlist;
    }
}

synthmod *
synthmodlist::add_module(synthmod * sm)
{
    ll_item* llitem = smodlist->add_at_tail(sm);
    if (!llitem)
        return 0;
    synthmod* mod = (synthmod*)llitem->get_data();
    return mod;
}

synthmod * synthmodlist::create_module(
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
    case synthmodnames::LPFILTER:       return new lpfilter(uname);
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
    case synthmodnames::HPFILTER:       return new hpfilter(uname);
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
    case synthmodnames::COSINEWAVE:     return new cosine_wave(uname);
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
    default:
        return 0;
    }
}

bool
synthmodlist::delete_module(synthmod * const sm)
{
    ll_item *tmp = smodlist->find_data(sm);
    if (tmp) {
        smodlist->unlink_item(tmp);
        delete sm;
        delete tmp;
        return true;
    }
    return false;
}

synthmod* synthmodlist::get_synthmod_by_name(char const* smname)
{
    goto_first();
    while(smod)	{
        if (strcmp(smod->get_username(), smname) == 0)
            return smod;
        goto_next();
    }
    return 0;
}

synthmod* synthmodlist::get_first_of_type(
                            synthmodnames::SYNTH_MOD_TYPE smt)
{
    search_modtype = smt;
    search_result = 0;
    if (smt <= synthmodnames::FIRST || smt >= synthmodnames::LAST)
        return 0;
    goto_first();
    while (smod != 0) {
        if (smod->get_module_type() == smt)
            return ((synthmod*)(search_result = smod_item)->get_data());
        goto_next();
    }
    return 0;
}

synthmod* synthmodlist::get_next_of_type()
{
    if (search_result == 0) return 0;
    smod = (synthmod*)(search_result->get_data());
    while(smod != 0) {
        if (smod->get_module_type() == search_modtype)
            return ((synthmod*)(search_result = smod_item)->get_data());
        goto_next();
    }
    search_result = 0;
    return 0;
}

void synthmodlist::remove_constants()
{
    if (constlist)
        return;
    constlist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    goto_first();
    while (smod) {
        switch(smod->get_module_type())
        {
            case synthmodnames::CONSTMOD:
            case synthmodnames::CONSTANT_FREQ:
            case synthmodnames::CONSTANT_NOTE:
            case synthmodnames::WCNTTRIGGER:
                unlink_constant(smod);
                break;
            default:
                break;
        }
        goto_next();
    }
}

void synthmodlist::unlink_constant(synthmod* sm)
{
    if (jwm.is_verbose())
        std::cout << "\nunlinking " << smod->get_username();
    smodlist->unlink_item(smod_item);
    delete smod_item;
    smod_item = smodlist->sneak_current();
    constlist->add_at_tail(sm);
}

#endif
