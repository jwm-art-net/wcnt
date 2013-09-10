#ifndef SYNTHMODULELIST_H
#include "../include/synthmodulelist.h"

synthmodlist::synthmodlist()
:  smodlist(0), smod_item(0), smod(0)
{
    smodlist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	if (!smodlist) {
        cerr << "Memory shortage, try flushing the loo.";
        exit(-1);
    }
    // only need one of these so make it here:- note it shall not be made anywhere else
    smod = new nonezero("off");
	if (!smod) {
        cerr << "Memory shortage, try washing your hands.";
        exit(-1);
    }
    if (!smodlist->add_at_head(smod)) {
        cerr << "Error: try improving your aim, more memory may help.";
        exit(-1);
    }
}

synthmodlist::~synthmodlist()
{
    goto_first();
    while (smod) {
        delete smod;
        goto_next();
    }
    delete smodlist;
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

synthmod *
synthmodlist::create_module(synthmodnames::SYNTH_MOD_TYPE smt, string uname)
{
    synthmod *sm;
    switch (smt) {
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
    default:
        sm = 0;
    }
    if (sm) {
        if (!ordered_insert(smodlist, sm, &synthmod::get_module_type)) {
            delete sm;
            sm = 0;
        }
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

synthmod* synthmodlist::get_synthmod_by_name(string const* smname)
{
	goto_first();
	while(smod)
	{
		if (*smod->get_username() == *smname)
			return smod;
		goto_next();
	}
	return 0;
}

#endif
