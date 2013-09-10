#ifndef JWMSYNTH_H
#include "../include/jwmsynth.h"
#include "../include/jwm_globals.h"
#include "../include/synthmodulelist.h"
#include "../include/dobj.h"
#include "../include/synthfilereader.h"
#include "../include/connectorlist.h"
#include "../include/stockerrs.h"
#include "../include/wcntexit.h"

#include <stdlib.h> // qsort
#include <iostream>

jwmsynth::jwmsynth() :
 synthmod_err_msg(0), dobj_err_msg(0),
 synthfile_reader(0), valid(false),
 exit_bar(0), in_bar_trig(0), in_bar(0)
{
    synthfile_reader = new synthfilereader(synthfilereader::WC_MAIN_FILE);
    synthfile_reader->set_wc_filename(jwm.file());
    valid = true;
}

jwmsynth::~jwmsynth()
{
    delete synthfile_reader;
}

bool jwmsynth::generate_synth()
{
    if (!synthfile_reader->read_and_create()) {
        err_msg = synthfile_reader->get_wc_error_msg();
        return false;
    }
    return true;
}

bool jwmsynth::connect_synth()
{
    if (jwm.is_verbose()) {
        std::cout << "\n\nWill now attempt to connect inputs and outputs";
        std::cout << ", hold your horses:\n";
    }
    if (!jwm.get_connectlist().make_connections()) {
        err_msg = *connector::get_connect_err_msg();
        return false;
    }
    return true;
}

bool jwmsynth::validate_synth()
{
    if (jwm.is_verbose())
        std::cout << "\nValidating modules...";
    stockerrs::ERR_TYPE et;
    synthmod* sm;
    sm = jwm.get_modlist().goto_first();
    while(sm) {
        if ((et = sm->validate()) != stockerrs::ERR_NO_ERROR) {
            err_msg = "\nModule ";
            err_msg += sm->get_username();
            err_msg += " is a little odd, ";
            err_msg += *synthmod::get_error_msg();
            err_msg += jwm.get_stockerrs().get_prefix_err(et);
            err_msg += jwm.get_stockerrs().get_err(et);
            return false;
        }
        if (!sm->check_inputs()) {
            err_msg = *synthmod::get_error_msg();
            return false;
        }
        sm->init();
        if (!sm->is_valid()){
            err_msg = "\nModule ";
            err_msg += sm->get_username();
            err_msg += " had problems initialising...\n";
            err_msg += *synthmod::get_error_msg();
            return false;
        }
        sm = jwm.get_modlist().goto_next();
    }
    if (jwm.get_modlist().get_first_of_type(synthmodnames::WCNTEXIT)==0)
    {
        err_msg += "\nNo wcnt_exit module created.";
        err_msg += "\nWithout this module I don't know when to stop.";
        return false;
    }
    return true;
}

bool jwmsynth::execute_synth()
{
    if (jwm.is_dont_run()) {
        std::cout << "\nNot running synth, as instructed.";
        return true;
    }
    synthmod* sm;
    // only use first wcnt module created, don't bother with any others
    // although user should not have been allowed to create > 1
    wcnt_exit* wcntexit = (wcnt_exit*) 
        jwm.get_modlist().get_first_of_type(synthmodnames::WCNTEXIT);
    const short* bar = wcntexit->get_input_bar();
    short exit_bar = wcntexit->get_exit_bar();
    // unlink any constant modules from list as it's pointless
    // calling run() on them...
    if (jwm.is_verbose())
        std::cout << "\nUnlinking constant modules from run list:";
    jwm.get_modlist().remove_constants();
    unsigned long sample = 0;
    char bigcount = '|';
    char littlecount = '~';
    int samplesperbig = jwm.samplerate();
    int divisions = 5;
    int samplespersmall = samplesperbig / divisions;
    int counter = 0;
    int divcounter = 0;
    if (jwm.is_verbose())
        std::cout << "\n";
    std::cout << "\nRunning synth (one '";
    std::cout << bigcount << "' per second done)\n";
    const STATUS* force_abort = synthmod::get_abort_status();
    while (*bar < exit_bar && *force_abort == OFF)
    {
        sm = jwm.get_modlist().goto_first();
        while(sm) {
            sm->run();
            sm = jwm.get_modlist().goto_next();
        }
        sample++;
        counter++;
        if (counter == samplespersmall)	{
            divcounter++;
            if (divcounter == divisions) {
                std::cout << bigcount;
                divcounter = 0;
            }
            else std::cout << littlecount;
            std::cout.flush();
            counter = 0;
        }
    }
    return true;
}

#endif
