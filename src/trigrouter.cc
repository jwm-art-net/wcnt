#include "../include/trigrouter.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/group.h"

#include <iostream>
#include <sstream>

trigrouter::trigrouter(char const* uname) :

 synthmod(synthmodnames::TRIGROUTER, uname, SM_UNGROUPABLE),
 in_trig(0), in_count(0),
 count(0), wrap(OFF),
 grp(0), trigs(0)
{
    register_input(inputnames::IN_TRIG);
    register_input(inputnames::IN_COUNT);
    init_first();
}

trigrouter::~trigrouter()
{
    if (grp)
        delete grp;
    if (trigs)
        delete [] trigs;
}

void const* trigrouter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS const*)o;
        case inputnames::IN_COUNT:return in_count = (short const*)o;
        default: return 0;
    }
}

void const* trigrouter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        case inputnames::IN_COUNT:return in_count;
        default: return 0;
    }
}


stockerrs::ERR_TYPE trigrouter::validate()
{
    if (!jwm.get_paramlist()->validate(this,
        paramnames::COUNT, stockerrs::ERR_ABOVE1))
    {
        *err_msg = paramnames::get_name(paramnames::COUNT);
        invalidate();
        return stockerrs::ERR_ABOVE1;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool trigrouter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::COUNT:
            count = *(short*)data;
            if (count > 0)
                create_wcnt_triggers();
            return true;
        case paramnames::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* trigrouter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::COUNT:     return &count;
        case paramnames::WRAP:      return &wrap;
        default: return 0;
    }
}

void trigrouter::create_wcnt_triggers()
{
    char const* wtn =
        synthmodnames::get_name(synthmodnames::WCNTTRIGGER);
    char const* un = get_username();
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
    synthmod* sm = 0;
    for (int i = 0; i <= count; i++){
        if (verbose){
            std::cout << "\n    creating " << wtn << "...";
            jwm.set_verbose(false);
        }
        std::ostringstream ostr;
        ostr << i;
        sm = sml->create_module(
            synthmodnames::WCNTTRIGGER, ostr.str().c_str());
        sml->add_module(sm);
        grp->group_module(sm);
        if (verbose){
            jwm.set_verbose(true);
            std::cout << "\n      grouped as " << sm->get_username();
            std::cout << "\n      connecting to trigger " << i;
        }
        if (!sm->set_in(inputnames::IN_TRIG, &trigs[i])) {
            std::cout << "\n***** MAJOR ERROR *****";
            std::cout << "\nin " << un << " could not set input for "
                "dynamically created " << wtn << "! bad, bad, bad";
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
    register_param(paramnames::COUNT);
    register_param(paramnames::WRAP);
}

