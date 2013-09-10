#ifndef SYNTHMODULELIST_H
#include "../include/synthmodulelist.h"

synthmodlist::synthmodlist() :
 smodlist(0), constlist(0), smod_item(0), smod(0),
 search_modtype(synthmodnames::MOD_FIRST), search_result(0)
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
    case synthmodnames::MOD_WCNTEXIT:
        return new wcnt_exit(uname);
    case synthmodnames::MOD_ADSR:
        return new adsr(uname);
    case synthmodnames::MOD_STEREOAMP:
        return new stereo_amp(uname);
    case synthmodnames::MOD_CLOCK:
        return new clockclock(uname);
    case synthmodnames::MOD_CONSTMOD:
        return new constmod(uname);
    case synthmodnames::MOD_FREQGEN:
        return new freq_generator(uname);
    case synthmodnames::MOD_LFOCLOCK:
        return new lfo_clock(uname);
    case synthmodnames::MOD_LFOCONTROL:
        return new lfo_controller(uname);
    case synthmodnames::MOD_LPFILTER:
        return new lpfilter(uname);
    case synthmodnames::MOD_MODIFIER:
        return new modifier(uname);
    case synthmodnames::MOD_NOISEGEN:
        return new noise_generator(uname);
    case synthmodnames::MOD_OSCCLOCK:
        return new osc_clock(uname);
    case synthmodnames::MOD_SAMPLEHOLD:
        return new sample_hold(uname);
    case synthmodnames::MOD_SEQUENCER:
        return new sequencer(uname);
    case synthmodnames::MOD_SINEWAVE:
        return new sine_wave(uname);
    case synthmodnames::MOD_SQUAREWAVE:
        return new square_wave(uname);
    case synthmodnames::MOD_TRIGGER:
        return new trigger(uname);
    case synthmodnames::MOD_TRIWAVE:
        return new triangle_wave(uname);
    case synthmodnames::MOD_TRIWAVE2:
        return new triangle_wave2(uname);
    case synthmodnames::MOD_USERWAVE:
        return new user_wave(uname);
    case synthmodnames::MOD_SAMPLER:
        return new sampler(uname);
    case synthmodnames::MOD_WAVFILEOUT:
        return new wavfileout(uname);
    case synthmodnames::MOD_STEREOCHANNEL:
        return new stereo_channel(uname);
    case synthmodnames::MOD_STEREOMIXER:
        return new stereomixer(uname);
    case synthmodnames::MOD_RANDTRIGGER:
        return new randomtrigger(uname);
    case synthmodnames::MOD_LOGICTRIGGER:
        return new logictrigger(uname);
    case synthmodnames::MOD_SWITCHER:
        return new switcher(uname);
    case synthmodnames::MOD_WCNTSIGNAL:
        return new wcnt_signal(uname);
    case synthmodnames::MOD_HPFILTER:
        return new hpfilter(uname);
    case synthmodnames::MOD_COMBINER:
        return new combiner(uname);
    case synthmodnames::MOD_TIMEMAP:
        return new timemap(uname);
    case synthmodnames::MOD_SERIALWAVFILEOUT:
        return new serialwavfileout(uname);
    case synthmodnames::MOD_CONTRASTER:
        return new contraster(uname);
    case synthmodnames::MOD_DELAY:
        return new delay(uname);
    case synthmodnames::MOD_ECHO:
        return new echo(uname);
    case synthmodnames::MOD_MONOAMP:
        return new mono_amp(uname);
    case synthmodnames::MOD_MULTIPLIER:
        return new multiplier(uname);
    case synthmodnames::MOD_RANGELIMIT:
        return new range_limit(uname);
    case synthmodnames::MOD_PAN:
        return new pan(uname);
    case synthmodnames::MOD_RMS:
        return new rms(uname);
    case synthmodnames::MOD_DCFILTER:
        return new dc_filter(uname);
    case synthmodnames::MOD_DYNAMIC:
        return new dynamic(uname);
    case synthmodnames::MOD_SPREADER:
        return new spreader(uname);
    case synthmodnames::MOD_NOTETRAN:
        return new notetran(uname);
    case synthmodnames::MOD_WAITTRIG:
        return new waittrig(uname);
    case synthmodnames::MOD_PATTERNTRIG:
        return new patterntrig(uname);
    case synthmodnames::MOD_STATEGATETRIG:
        return new stategatetrig(uname);
    case synthmodnames::MOD_INVERT:
        return new invert(uname);
    case synthmodnames::MOD_TIMER:
        return new timer(uname);
    case synthmodnames::MOD_SYNCCLOCK:
        return new sync_clock(uname);
    case synthmodnames::MOD_WCNTTRIGGER:
        return new wcnt_trigger(uname);
    case synthmodnames::MOD_TRIGSWITCHER:
        return new trigswitcher(uname);
    case synthmodnames::MOD_ONOFFTRIG:
        return new onofftrig(uname);
    case synthmodnames::MOD_PEAKDETECTOR:
        return new peak_detector(uname);
    case synthmodnames::MOD_STEPPER:
        return new stepper(uname);
    case synthmodnames::MOD_ADDER:
        return new adder(uname);
    case synthmodnames::MOD_SUBTRACTER:
        return new subtracter(uname);
    case synthmodnames::MOD_TRIGDELAY:
        return new trigdelay(uname);
    case synthmodnames::MOD_SIMPLEDELAY:
        return new simple_delay(uname);
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
    if (smt <= synthmodnames::MOD_FIRST || smt >= synthmodnames::MOD_LAST)
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

void synthmodlist::remove_constants(bool verbose)
{
    if (constlist)
        return;
    constlist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    get_first_of_type(synthmodnames::MOD_CONSTMOD);
    while (smod) {
        if (verbose)
            cout << "\nunlinking " << smod->get_username();
        smodlist->unlink_item(search_result);
        constlist->add_at_tail(search_result->get_data());
        delete search_result;
        search_result = 0;
        get_first_of_type(synthmodnames::MOD_CONSTMOD);
    }
    // and wcnt_triggers!
    get_first_of_type(synthmodnames::MOD_WCNTTRIGGER);
    while (smod) {
        if (verbose)
            cout << "\nunlinking " << smod->get_username();
        smodlist->unlink_item(search_result);
        constlist->add_at_tail(search_result->get_data());
        delete search_result;
        search_result = 0;
        get_first_of_type(synthmodnames::MOD_WCNTTRIGGER);
    }
}

#endif
