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
    synthmod *sm;
    switch (smt) {
    case synthmodnames::MOD_WCNT:
        sm = new wcnt_module(uname);
        break;
    case synthmodnames::MOD_ADSR:
        sm = new adsr(uname);
        break;
    case synthmodnames::MOD_STEREOAMP:
        sm = new stereo_amp(uname);
        break;
    case synthmodnames::MOD_CLOCK:
        sm = new clockclock(uname);
        break;
    case synthmodnames::MOD_CONSTMOD:
        sm = new constmod(uname);
        break;
    case synthmodnames::MOD_FREQGEN:
        sm = new freq_generator(uname);
        break;
    case synthmodnames::MOD_LFOCLOCK:
        sm = new lfo_clock(uname);
        break;
    case synthmodnames::MOD_LFOCONTROL:
        sm = new lfo_controller(uname);
        break;
    case synthmodnames::MOD_LPFILTER:
        sm = new lpfilter(uname);
        break;
    case synthmodnames::MOD_MODIFIER:
        sm = new modifier(uname);
        break;
    case synthmodnames::MOD_NOISEGEN:
        sm = new noise_generator(uname);
        break;
    case synthmodnames::MOD_OSCCLOCK:
        sm = new osc_clock(uname);
        break;
    case synthmodnames::MOD_SAMPLEHOLD:
        sm = new sample_hold(uname);
        break;
    case synthmodnames::MOD_SEQUENCER:
        sm = new sequencer(uname);
        break;
    case synthmodnames::MOD_SINEWAVE:
        sm = new sine_wave(uname);
        break;
    case synthmodnames::MOD_SQUAREWAVE:
        sm = new square_wave(uname);
        break;
    case synthmodnames::MOD_TRIGGER:
        sm = new trigger(uname);
        break;
    case synthmodnames::MOD_TRIWAVE:
        sm = new triangle_wave(uname);
        break;
    case synthmodnames::MOD_TRIWAVE2:
        sm = new triangle_wave2(uname);
        break;
    case synthmodnames::MOD_USERWAVE:
        sm = new user_wave(uname);
        break;
    case synthmodnames::MOD_SAMPLER:
        sm = new sampler(uname);
        break;
    case synthmodnames::MOD_WAVFILEOUT:
        sm = new wavfileout(uname);
        break;
    case synthmodnames::MOD_STEREOCHANNEL:
        sm = new stereo_channel(uname);
        break;
    case synthmodnames::MOD_STEREOMIXER:
        sm = new stereomixer(uname);
        break;
    case synthmodnames::MOD_RANDTRIGGER:
        sm = new randomtrigger(uname);
        break;
    case synthmodnames::MOD_LOGICTRIGGER:
        sm = new logictrigger(uname);
        break;
    case synthmodnames::MOD_SWITCHER:
        sm = new switcher(uname);
        break;
    case synthmodnames::MOD_WCNTSIGNAL:
        sm = new wcnt_signal(uname);
        break;
    case synthmodnames::MOD_HPFILTER:
        sm = new hpfilter(uname);
        break;
    case synthmodnames::MOD_COMBINER:
        sm = new combiner(uname);
        break;
    case synthmodnames::MOD_TIMEMAP:
        sm = new timemap(uname);
        break;
    case synthmodnames::MOD_SERIALWAVFILEOUT:
        sm = new serialwavfileout(uname);
        break;
    case synthmodnames::MOD_CONTRASTER:
        sm = new contraster(uname);
        break;
    case synthmodnames::MOD_DELAY:
        sm = new delay(uname);
        break;
    case synthmodnames::MOD_ECHO:
        sm = new echo(uname);
        break;
    case synthmodnames::MOD_MONOAMP:
        sm = new mono_amp(uname);
        break;
    case synthmodnames::MOD_MULTIPLIER:
        sm = new multiplier(uname);
        break;
    case synthmodnames::MOD_RANGELIMIT:
        sm = new range_limit(uname);
        break;
    case synthmodnames::MOD_PAN:
        sm = new pan(uname);
        break;
    case synthmodnames::MOD_RMS:
        sm = new rms(uname);
        break;
    case synthmodnames::MOD_DCFILTER:
        sm = new dc_filter(uname);
        break;
    case synthmodnames::MOD_DYNAMIC:
        sm = new dynamic(uname);
        break;
    case synthmodnames::MOD_SPREADER:
        sm = new spreader(uname);
        break;
    case synthmodnames::MOD_NOTETRAN:
        sm = new notetran(uname);
        break;
    case synthmodnames::MOD_WAITTRIG:
        sm = new waittrig(uname);
        break;
    case synthmodnames::MOD_PATTERNTRIG:
        sm = new patterntrig(uname);
        break;
    case synthmodnames::MOD_STATEGATETRIG:
        sm = new stategatetrig(uname);
        break;
    case synthmodnames::MOD_INVERT:
        sm = new invert(uname);
        break;
    case synthmodnames::MOD_TIMER:
        sm = new timer(uname);
        break;
    case synthmodnames::MOD_SYNCCLOCK:
        sm = new sync_clock(uname);
        break;
    case synthmodnames::MOD_WCNTTRIGGER:
        sm = new wcnt_trigger(uname);
        break;
    case synthmodnames::MOD_TRIGSWITCHER:
        sm = new trigswitcher(uname);
        break;
    case synthmodnames::MOD_ONOFFTRIG:
        sm = new onofftrig(uname);
        break;
    default:
        sm = 0;
    }
    return sm;
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
