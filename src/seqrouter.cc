#include "../include/seqrouter.h"
#include "../include/globals.h"
#include "../include/group.h"
#include "../include/synthmodlist.h"

#include <iostream>
#include <sstream>

seq_router::seq_router(const char* uname) :
 synthmod::base(synthmod::SEQ_ROUTER, uname, SM_UNGROUPABLE),
 in_index(0), in_note_on_trig(0), in_note_slide_trig(0),
 in_note_off_trig(0), in_freq(0), in_velocity(0), in_all_off_trig(0),
 count(0), wrap(OFF),
 index(-1), last_on_ix(-1), last_slide_ix(-1), last_off_ix(-1),
 grp(0), on_trigs(0), slide_trigs(0), off_trigs(0), vels(0), freqs(0)
{
}

void seq_router::register_ui()
{
    register_param(param::COUNT);
    register_input(input::IN_INDEX);
    register_param(param::WRAP);
    register_input(input::IN_NOTE_ON_TRIG);
    register_input(input::IN_NOTE_SLIDE_TRIG);
    register_input(input::IN_NOTE_OFF_TRIG);
    register_input(input::IN_FREQ);
    register_input(input::IN_VELOCITY);
    register_input(input::IN_ALL_OFF_TRIG);
}

ui::moditem_list* seq_router::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

seq_router::~seq_router()
{
    if (grp)
        delete grp;
    if (on_trigs)
        delete [] on_trigs;
    if (slide_trigs)
        delete [] slide_trigs;
    if (off_trigs)
        delete [] off_trigs;
    if (vels)
        delete [] vels;
    if (freqs)
        delete [] freqs;
}

const void* seq_router::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_INDEX:
        return in_index = (const wcint_t*)o;
    case input::IN_NOTE_ON_TRIG:
        return in_note_on_trig = (const STATUS*)o;
    case input::IN_NOTE_SLIDE_TRIG:
        return in_note_slide_trig = (const STATUS*)o;
    case input::IN_NOTE_OFF_TRIG:
        return in_note_off_trig = (const STATUS*)o;
    case input::IN_FREQ:
        return in_freq = (const double*)o;
    case input::IN_VELOCITY:
        return in_velocity = (const double*)o;
    case input::IN_ALL_OFF_TRIG:
        return in_all_off_trig = (const STATUS*)o;
    default:
        return 0;
    }
}

const void* seq_router::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_INDEX:           return in_index;
    case input::IN_NOTE_ON_TRIG:    return in_note_on_trig;
    case input::IN_NOTE_SLIDE_TRIG: return in_note_slide_trig;
    case input::IN_NOTE_OFF_TRIG:   return in_note_off_trig;
    case input::IN_FREQ:            return in_freq;
    case input::IN_VELOCITY:        return in_velocity;
    case input::IN_ALL_OFF_TRIG:    return in_all_off_trig;
    default:
        return 0;
    }
}


errors::TYPE seq_router::validate()
{
    if (!validate_param(param::COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

bool seq_router::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::COUNT:
            count = *(wcint_t*)data;
            if (count > 0)
                create_wcnt_notes();
            return true;
        case param::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* seq_router::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::COUNT:     return &count;
        case param::WRAP:      return &wrap;
        default: return 0;
    }
}

void seq_router::create_wcnt_notes()
{
    const char* wtn = synthmod::names::get(synthmod::WCNT_NOTE);
    const char* un = get_username();
    bool verbose = wcnt::jwm.is_verbose();
    if (verbose){
        std::cout << "\nPre-initialising " << un << " module...";
        std::cout << "\n  creating group " << un;
    }
    grp = new group;
    grp->set_username(get_username());
    if (on_trigs)
        delete [] on_trigs;
    if (slide_trigs)
        delete [] slide_trigs;
    if (off_trigs)
        delete [] off_trigs;
    if (vels)
        delete [] vels;
    if (freqs)
        delete [] freqs;

    on_trigs = new STATUS[count];
    slide_trigs = new STATUS[count];
    off_trigs = new STATUS[count];
    freqs = new double[count];
    vels = new double[count];

    synthmod::list* sml = wcnt::jwm.get_modlist();
    synthmod::list::linkedlist*
                grpmodlist = sml->remove_modules_of_group(get_username());
    if (grpmodlist) {
        grpmodlist->empty_list(DELETE_DATA);
        delete grpmodlist;
    }

    for (int i = 0; i < count; i++) {
        on_trigs[i] = OFF;
        slide_trigs[i] = OFF;
        off_trigs[i] = OFF;
        freqs[i] = 0.0f;
        vels[i] = 0.0f;
        if (verbose) {
            std::cout << "\n    creating " << wtn << "...";
            wcnt::jwm.set_verbose(false);
        }
        std::ostringstream ostr;
        ostr << i;
        synthmod::base* sm = sml->create_module(synthmod::WCNT_NOTE,
                                                    ostr.str().c_str());
        sml->add_module(sm);
        grp->group_module(sm);
        if (verbose) {
            wcnt::jwm.set_verbose(true);
            std::cout << "\n      grouped as " << sm->get_username();
            std::cout << "\n      connecting to wcnt_note " << i;
        }
        if (!sm->set_in(input::IN_NOTE_ON_TRIG, &on_trigs[i]))
            sm_err("%s in %s could not set in_note_on_trig for "
                   "dynamically created %s.", errors::stock::major, un,
                                                                    wtn);
        if (!sm->set_in(input::IN_NOTE_SLIDE_TRIG, &slide_trigs[i]))
            sm_err("%s in %s could not set in_note_slide_trig for "
                   "dynamically created %s.", errors::stock::major, un,
                                                                    wtn);
        if (!sm->set_in(input::IN_NOTE_OFF_TRIG, &off_trigs[i]))
            sm_err("%s in %s could not set in_note_off_trig for "
                   "dynamically created %s.", errors::stock::major, un,
                                                                    wtn);
        if (!sm->set_in(input::IN_FREQ, &freqs[i]))
            sm_err("%s in %s could not set in_freq for "
                   "dynamically created %s.", errors::stock::major, un,
                                                                    wtn);
        if (!sm->set_in(input::IN_VELOCITY, &vels[i]))
            sm_err("%s in %s could not set in_velocity for "
                   "dynamically created %s.", errors::stock::major, un,
                                                                    wtn);
    }
    if (verbose)
        std::cout << "\nDone...ok.";
}

void seq_router::run()
{
    if (*in_all_off_trig == ON) {
        for (int i = 0; i < count; ++i) {
            on_trigs[i] = OFF;
            slide_trigs[i] = OFF;
            off_trigs[i] = ON;
            freqs[i] = 0.0f;
            vels[i] = 0.0f;
        }
        return;
    }

    if (last_on_ix > -1) {
        on_trigs[last_on_ix] = OFF;
        last_on_ix = -1;
    }
    if (last_slide_ix > -1) {
        slide_trigs[last_slide_ix] = OFF;
        last_slide_ix = -1;
    }
    if (last_off_ix > -1) {
        off_trigs[last_off_ix] = OFF;
        last_off_ix = -1;
    }

    if (*in_note_on_trig == ON) {
        index = *in_index;
        if (wrap == ON)
            index %= count;
        if (index > -1 && index < count) {
            last_on_ix = index;
            on_trigs[index] = ON;
            freqs[index] = *in_freq;
            vels[index] = *in_velocity;
        }
        else
            index = -1;
    }
    if (*in_note_slide_trig == ON) {
        if (index > -1) {
            last_slide_ix = index;
            slide_trigs[index] = ON;
            freqs[index] = *in_freq;
        }
    }
    if (*in_note_off_trig == ON) {
        if (index > -1) {
            last_off_ix = index;
            off_trigs[index] = ON;
        }
    }
}
