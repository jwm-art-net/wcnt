#include "../include/trigrouter.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/group.h"

#include <iostream>
#include <sstream>

trigrouter::trigrouter(const char* uname) :

 synthmod(module::TRIGROUTER, uname, SM_UNGROUPABLE),
 in_trig(0), in_count(0),
 count(0), wrap(OFF),
 grp(0), trigs(0)
{
    register_input(input::IN_TRIG);
    register_input(input::IN_COUNT);
    init_first();
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
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS const*)o;
        case input::IN_COUNT:return in_count = (short const*)o;
        default: return 0;
    }
}

const void* trigrouter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        case input::IN_COUNT:return in_count;
        default: return 0;
    }
}


stockerrs::ERR_TYPE trigrouter::validate()
{
    if (!jwm.get_paramlist()->validate(this,
        param::COUNT, stockerrs::ERR_ABOVE1))
    {
        sm_err("%s", param::names::get(param::COUNT));
        invalidate();
        return stockerrs::ERR_ABOVE1;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool trigrouter::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::COUNT:
            count = *(short*)data;
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
    const char* wtn =
        module::names::get(module::WCNTTRIGGER);
    const char* un = get_username();
    bool verbose = jwm.is_verbose();
    if (verbose){
        std::cout << "\nPre-initialising " << un << " module...";
        std::cout << "\n  creating group " << un;
    }
    grp = new group;
    grp->set_username(get_username());
    if (trigs)
        delete [] trigs;
    trigs = new STATUS[count + 1];
    synthmodlist* sml = jwm.get_modlist();
    for (int i = 0; i <= count; i++){
        if (verbose){
            std::cout << "\n    creating " << wtn << "...";
            jwm.set_verbose(false);
        }
        std::ostringstream ostr;
        ostr << i;
        synthmod* sm = sml->create_module(
            module::WCNTTRIGGER, ostr.str().c_str());
        sml->add_module(sm);
        grp->group_module(sm);
        if (verbose){
            jwm.set_verbose(true);
            std::cout << "\n      grouped as " << sm->get_username();
            std::cout << "\n      connecting to trigger " << i;
        }
        if (!sm->set_in(input::IN_TRIG, &trigs[i])) {
            sm_err("%s in %s could not set input for "
                            "dynamically created %s.", stockerrs::major,
                                                                un, wtn);
        }
    }
    if (verbose)
        std::cout << "\nDone...ok.";
}

void trigrouter::run()
{
    STATUS* trg = trigs;
    for (short ix = 0; ix <= count; trg++, ix++)
        *trg = OFF;
    if (*in_trig == ON) {
        short ix = *in_count;
        if (wrap == ON) {
            while(ix < 0)
                ix += count;
            while(ix > count)
                ix -= count;
        }
        if (ix >= 0 && ix <= count)
            trigs[ix] = ON;
    }
}

void trigrouter::init_first()
{
    if (done_first())
        return;
    register_param(param::COUNT);
    register_param(param::WRAP);
}

