#include "../include/synthmodlist.h"
#include "../include/modules.h"
#include "../include/globals.h"
#include "../include/dobjlist.h"
#include "../include/group.h"
#include "../include/stockerrs.h"

#include <iostream>

namespace synthmod
{

    nonezero* list::off = 0;

 list::list(DESTRUCTION d) :
  linkedlist(MULTIREF_OFF, d),
  emptyrunlist(0),
  search(synthmod::ERR_TYPE),
  result(0)
 {
    // nonezero module is not user accessable.
    // it's outputs are used when ever a module's input is set to off.
    // only need one for the main list (ie the first synthmodlist created)
    if (!off) {
        off = new nonezero("off");
        add_at_head(off);
    }
 }

 list::~list()
 {
    if (emptyrunlist)
        delete emptyrunlist;
 }

 synthmod::base* list::create_module(synthmod::TYPE smt, const char* uname)
 {
    synthmod::base* sm = 0;

    switch (smt) {
     case synthmod::WCNTEXIT:       sm = new wcnt_exit(uname);      break;
     case synthmod::ADSR:           sm = new adsr(uname);           break;
     case synthmod::STEREOAMP:      sm = new stereo_amp(uname);     break;
     case synthmod::CLOCK:          sm = new clockclock(uname);     break;
     case synthmod::CONSTMOD:       sm = new constmod(uname);       break;
     case synthmod::FREQGEN:        sm = new freq_generator(uname); break;
     case synthmod::LFOCLOCK:       sm = new lfo_clock(uname);      break;
     case synthmod::LFOCONTROL:     sm = new lfo_controller(uname); break;
     case synthmod::MODIFIER:       sm = new modifier(uname);       break;
     case synthmod::NOISEGEN:       sm = new noise_generator(uname);break;
     case synthmod::OSCCLOCK:       sm = new osc_clock(uname);      break;
     case synthmod::SAMPLEHOLD:     sm = new sample_hold(uname);    break;
     case synthmod::SEQUENCER:      sm = new sequencer(uname);      break;
     case synthmod::SQUAREWAVE:     sm = new square_wave(uname);    break;
     case synthmod::TRIGGER:        sm = new trigger(uname);        break;
     case synthmod::TRIWAVE2:       sm = new triangle_wave2(uname); break;
     case synthmod::USERWAVE:       sm = new user_wave(uname);      break;
     case synthmod::SAMPLER:        sm = new sampler(uname);        break;
     case synthmod::WAVFILEOUT:     sm = new wavfileout(uname);     break;
     case synthmod::STEREOCHANNEL:  sm = new stereo_channel(uname); break;
     case synthmod::STEREOMIXER:    sm = new stereomixer(uname);    break;
     case synthmod::RANDTRIGGER:    sm = new randomtrigger(uname);  break;
     case synthmod::LOGICTRIGGER:   sm = new logictrigger(uname);   break;
     case synthmod::SWITCHER:       sm = new switcher(uname);       break;
     case synthmod::WCNTSIGNAL:     sm = new wcnt_signal(uname);    break;
     case synthmod::COMBINER:       sm = new combiner(uname);       break;
     case synthmod::TIMEMAP:        sm = new timemap(uname);        break;
     case synthmod::SERIALWAVFILEOUT:
        sm = new serialwavfileout(uname);
        break;
     case synthmod::CONTRASTER:     sm = new contraster(uname);     break;
     case synthmod::DELAY:          sm = new delay(uname);          break;
     case synthmod::SIMPLEDELAY:    sm = new simple_delay(uname);   break;
     case synthmod::ECHO:           sm = new echo(uname);           break;
     case synthmod::MONOAMP:        sm = new mono_amp(uname);       break;
     case synthmod::MULTIPLIER:     sm = new multiplier(uname);     break;
     case synthmod::RANGELIMIT:     sm = new range_limit(uname);    break;
     case synthmod::PAN:            sm = new pan(uname);            break;
     case synthmod::RMS:            sm = new rms(uname);            break;
     case synthmod::DCFILTER:       sm = new dc_filter(uname);      break;
     case synthmod::DYNAMIC:        sm = new dynamic(uname);        break;
     case synthmod::SPREADER:       sm = new spreader(uname);       break;
     case synthmod::NOTETRAN:       sm = new notetran(uname);       break;
     case synthmod::WAITTRIG:       sm = new waittrig(uname);       break;
     case synthmod::PATTERNTRIG:    sm = new patterntrig(uname);    break;
     case synthmod::STATEGATETRIG:  sm = new stategatetrig(uname);  break;
     case synthmod::INVERT:         sm = new invert(uname);         break;
     case synthmod::TIMER:          sm = new timer(uname);          break;
     case synthmod::SYNCCLOCK:      sm = new sync_clock(uname);     break;
     case synthmod::WCNTTRIGGER:    sm = new wcnt_trigger(uname);   break;
     case synthmod::TRIGSWITCHER:   sm = new trigswitcher(uname);   break;
     case synthmod::ONOFFTRIG:      sm = new onofftrig(uname);      break;
     case synthmod::PEAKDETECTOR:   sm = new peak_detector(uname);  break;
     case synthmod::STEPPER:        sm = new stepper(uname);        break;
     case synthmod::ADDER:          sm = new adder(uname);          break;
     case synthmod::SUBTRACTER:     sm = new subtracter(uname);     break;
     case synthmod::TRIGDELAY:      sm = new trigdelay(uname);      break;
     case synthmod::DIFFFILTER:     sm = new diff_filter(uname);    break;
     case synthmod::IMPULSE:        sm = new impulse(uname);        break;
     case synthmod::ORBIT:          sm = new orbit(uname);          break;
     #ifdef WITH_LADSPA
     case synthmod::GLAME_BUTTERWORTH:
        sm = new glame_butterworth(uname);      break;
     case synthmod::FAST_LOOKAHEAD_LIMITER:
        sm = new fast_lookahead_limiter(uname); break;
     case synthmod::DC_OFFSET_REMOVER:
        sm = new dc_offset_remover(uname);      break;
     case synthmod::SC1:
        sm = new sc1(uname);                    break;
     case synthmod::SINGLE_BAND_PARA:
        sm = new single_band_para(uname);       break;
     case synthmod::GLAME_FILTER:
        sm = new glame_filter(uname);           break;
     case synthmod::BODE_FREQ_SHIFTER:
        sm = new bode_freq_shifter(uname);      break;
     case synthmod::CAPS_PLATE:
        sm = new caps_plate2x2(uname);          break;
     #endif // WITH_LADSPA
     case synthmod::WAVE:           sm = new wave(uname);            break;
     case synthmod::WAVE_PHASE:     sm = new wave_phase(uname);      break;
     case synthmod::CONSTANT_FREQ:  sm = new constant_freq(uname);   break;
     case synthmod::CONSTANT_NOTE:  sm = new constant_note(uname);   break;
     case synthmod::BALANCE:        sm = new balance(uname);         break;
     case synthmod::TRIGECHO:       sm = new trigecho(uname);        break;
     case synthmod::INSPECT:        sm = new inspect(uname);         break;
     case synthmod::TRIGCOUNTER:    sm = new trigcounter(uname);     break;
     case synthmod::TRIGROUTER:     sm = new trigrouter(uname);      break;
     case synthmod::GROUPCONTROL:   sm = new group_control(uname);   break;
     case synthmod::FADER:          sm = new fader(uname);           break;
     case synthmod::SAMPLECLIMB:    sm = new sample_climb(uname);    break;
     case synthmod::SEQ_ROUTER:     sm = new seq_router(uname);      break;
     case synthmod::WCNT_NOTE:      sm = new wcnt_note(uname);       break;
     case synthmod::SEQ_ECHO:       sm = new seq_echo(uname);        break;
     default:
        sm = 0;
    }
    if (sm)
        sm->ui_register();

    return sm;
 }

 bool
 list::delete_module(synthmod::base* const sm)
 {
    llitem *tmp = find_data(sneak_first(), sm);
    if (tmp) {
        delete unlink_item(tmp);
        delete sm;
        return true;
    }
    return false;
 }

 list::linkedlist* list::duplicate_group(const char* from, const char* to)
 {
    linkedlist* grplist = new linkedlist(MULTIREF_OFF, PRESERVE_DATA);
    llitem* i = find_in_data(sneak_first(), groupname(from));
    while(i) {
        synthmod::base* frommod = i->get_data();
        const char* const
            togrp = set_groupname(to, frommod->get_username());
        synthmod::base* tomod
            = frommod->duplicate_module(togrp, synthmod::base::NO_CONNECT);
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

 void list::remove_empty_run_modules()
 {
    emptyrunlist = move_to_new_list_of_by(this,
                                            synthmod::base::SM_EMPTY_RUN);
    if (wcnt::jwm.is_verbose()) {
        synthmod::base* sm = emptyrunlist->goto_first();
        while(sm) {
            std::cout << synthmod::names::get(sm->get_module_type())
                      << " " << sm->get_username() << std::endl;
            sm = emptyrunlist->goto_next();
        }
    }
 }
}; // namespace synthmod
