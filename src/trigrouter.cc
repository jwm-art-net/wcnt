#include "../include/trigrouter.h"
#include "../include/globals.h"
#include "../include/synthmodlist.h"
#include "../include/group.h"

#include <iostream>
#include <sstream>

trigrouter::trigrouter(const char* uname) :

 synthmod::base(synthmod::TRIGROUTER, uname, SM_UNGROUPABLE),
 in_trig(0), in_index(0), in_all_off_trig(0),
 count(0), wrap(OFF),
 grp(0), trigs(0), last_ix(-1)
{
}

void trigrouter::register_ui()
{
    register_input(input::IN_TRIG);
    register_input(input::IN_INDEX);
    register_input(input::IN_ALL_OFF_TRIG);
    register_param(param::COUNT);
    register_param(param::WRAP);
}

trigrouter::~trigrouter()
{
    if (grp)
        delete grp;
    if (trigs)
        delete [] trigs;
}

const void* trigrouter::set_in(input::TYPE it, const void* o)
{
    switch(it) {
    case input::IN_TRIG:        return in_trig = (STATUS const*)o;
    case input::IN_INDEX:       return in_index = (wcint_t const*)o;
    case input::IN_ALL_OFF_TRIG:return in_all_off_trig = (STATUS const*)o;
    default:
        return 0;
    }
}

const void* trigrouter::get_in(input::TYPE it) const
{
    switch(it) {
    case input::IN_TRIG:        return in_trig;
    case input::IN_INDEX:       return in_index;
    case input::IN_ALL_OFF_TRIG:return in_all_off_trig;
    default:
        return 0;
    }
}


errors::TYPE trigrouter::validate()
{
    if (!validate_param(param::COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

bool trigrouter::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::COUNT:
            count = *(wcint_t*)data;
            if (count > 0)
                create_wcnt_triggers();
            return true;
        case param::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* trigrouter::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::COUNT:     return &count;
        case param::WRAP:      return &wrap;
        default: return 0;
    }
}

void trigrouter::create_wcnt_triggers()
{
    const char* wtn = synthmod::names::get(synthmod::WCNTTRIGGER);
    const char* un = get_username();
    bool verbose = wcnt::jwm.is_verbose();
    if (verbose){
        std::cout << "\nPre-initialising " << un << " module...";
        std::cout << "\n  creating group " << un;
    }
    grp = new group;
    grp->set_username(get_username());
    if (trigs)
        delete [] trigs;
    trigs = new STATUS[count];
    synthmod::list* sml = wcnt::jwm.get_modlist();
    for (int i = 0; i < count; i++) {
        trigs[i] = OFF;
        if (verbose) {
            std::cout << "\n    creating " << wtn << "...";
            wcnt::jwm.set_verbose(false);
        }
        std::ostringstream ostr;
        ostr << i;
        synthmod::base* sm = sml->create_module(synthmod::WCNTTRIGGER,
                                                    ostr.str().c_str());
        sml->add_module(sm);
        grp->group_module(sm);
        if (verbose) {
            wcnt::jwm.set_verbose(true);
            std::cout << "\n      grouped as " << sm->get_username();
            std::cout << "\n      connecting to trigger " << i;
        }
        if (!sm->set_in(input::IN_TRIG, &trigs[i])) {
            sm_err("%s in %s could not set input for "
                            "dynamically created %s.", errors::stock::major,
                                                                un, wtn);
        }
    }
    if (verbose)
        std::cout << "\nDone...ok.";
}

void trigrouter::run()
{
    if (last_ix > -1)
        trigs[last_ix] = OFF;

    if (*in_trig == ON) {
        wcint_t ix = *in_index;
        if (ix > -1) {
            if (wrap == ON)
                ix %= count;
            if (ix < count)
                trigs[last_ix = ix] = ON;
        }
    }
}

