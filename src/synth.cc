#include "../include/synth.h"
#include "../include/globals.h"
#include "../include/synthmodlist.h"
#include "../include/dobj.h"
#include "../include/synthfilereader.h"
#include "../include/connectorlist.h"
#include "../include/stockerrs.h"
#include "../include/wcntexit.h"

#include <iostream>

namespace wcnt
{
 #ifdef DEBUG
 #define jwm_err(fmt, ... )                              \
 {                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(err_msg, STRBUFLEN, fmt, __VA_ARGS__);  \
 }
 #else
 #define jwm_err(fmt, ... ) \
    cfmt(err_msg, STRBUFLEN, fmt, __VA_ARGS__)
 #endif


 synth::synth() :
  synthfile_reader(0), valid(false),
  exit_bar(0), in_bar_trig(0), in_bar(0)
 {
    synthfile_reader = new synthfilereader(synthfilereader::WC_MAIN_FILE);
    synthfile_reader->set_wc_filename(wcnt::jwm.file());
    valid = true;
    err_msg[0] = '\0';
 }

 synth::~synth()
 {
    delete synthfile_reader;
 }

 bool synth::generate_synth()
 {
    if (!synthfile_reader->read_and_create()) {
        jwm_err("%s", synthfile_reader->get_wc_error_msg());
        return false;
    }
    return true;
 }

 bool synth::connect_synth()
 {
    if (wcnt::jwm.is_verbose()) {
        std::cout << "\n\nWill now attempt to connect inputs and outputs";
        std::cout << ", hold your horses:" << std::endl;
    }
    if (!wcnt::get_connectlist()->make_connections()) {
        jwm_err("%s", connector::get_connect_err_msg());
        return false;
    }
    return true;
 }

 bool synth::validate_synth()
 {
    if (wcnt::jwm.is_verbose())
        std::cout << "\nValidating modules..." << std::endl;
    synthmod::base* sm;
    sm = wcnt::jwm.get_modlist()->goto_first();
    while(sm) {
        errors::TYPE et = sm->validate();
        if (et != errors::NO_ERROR) {
            jwm_err("Module %s is a little odd, %s %s %s",
                            sm->get_username(),
                            synthmod::base::get_error_msg(),
                            errors::stock::get_prefix_msg(et),
                            errors::stock::get(et));
            return false;
        }
        if (!sm->flag(synthmod::base::SM_VALID)){
            jwm_err("Module %s had problems initialising...",
                    sm->get_username(), synthmod::base::get_error_msg());
            return false;
        }
        sm = wcnt::jwm.get_modlist()->goto_next();
    }
    if (wcnt::jwm.get_modlist()->get_first_of_type(synthmod::WCNTEXIT)==0)
    {
        jwm_err("%s", "No wcnt_exit module created. Without this module "
                                         "I won't know when to stop.");
        return false;
    }
    return true;
 }

 bool synth::init_synth()
 {
    if (wcnt::jwm.is_verbose())
        std::cout << "\nInitialising modules..." << std::endl;
    synthmod::base* sm;
    synthmod::list* sml = wcnt::jwm.get_modlist();
    sm = sml->goto_first();
    while(sm) {
        sm->init();
        #ifdef DEBUG
        if (!sm->check_inputs()) {
            jwm_err("%s", synthmod::base::get_error_msg());
            return false;
        }
        #endif
        if (!sm->flag(synthmod::base::SM_VALID)){
            jwm_err("%s", synthmod::base::get_error_msg());
            return false;
        }
        sm = sml->goto_next();
    }
    return true;
 }

 bool synth::execute_synth()
 {
    if (wcnt::jwm.is_dont_run()) {
        std::cout << "Not running synth, as instructed." << std::endl;
        return true;
    }
    const wcint_t* bar = wcnt::jwm.exit_in_bar();
    wcint_t exit_bar = wcnt::jwm.exit_bar();
    // unlink any constant modules from list as it's pointless
    // calling run() on them...
    if (wcnt::jwm.is_verbose())
        std::cout << "\nUnlinking empty-run modules from run list:"
                                                        << std::endl;
    wcnt::jwm.get_modlist()->remove_empty_run_modules();
    if (wcnt::jwm.is_verbose())
        std::cout << "\n";
    const STATUS* force_abort = synthmod::base::get_abort_status();
    synthmod::base** runlist = wcnt::jwm.get_modlist()->get_run_list();

    std::cout << "samplerate: " << wcnt::jwm.samplerate() << std::endl;

    if (wcnt::jwm.is_no_progress()) {
        while (*bar < exit_bar && *force_abort == OFF)
        {
            int count = 0;
            synthmod::base* sm = runlist[count];
            while(sm) {
                sm->run();
                sm = runlist[++count];
            }
        }
    }
    else {
        samp_t sample = 0;
        char bigcount = '|';
        char littlecount = '~';
        int samplesperbig = wcnt::jwm.samplerate();
        int divisions = 4;
        int samplespersmall = samplesperbig / divisions;
        int counter = 0;
        int divcounter = 0;

        std::cout << "Running synth (one '" << bigcount
                  << "' per second done)" << std::endl;

        while (*bar < exit_bar && *force_abort == OFF)
        {
            int count = 0;
            synthmod::base* sm = runlist[count];
            while(sm) {
                sm->run();
                sm = runlist[++count];
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
    }
    if (runlist)
        destroy_array_moved_from_list(runlist);
    return true;
 }
}; // namespace wcnt
