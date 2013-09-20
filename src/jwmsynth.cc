#include "../include/jwmsynth.h"
#include "../include/jwm_globals.h"
#include "../include/synthmodlist.h"
#include "../include/dobj.h"
#include "../include/synthfilereader.h"
#include "../include/connectorlist.h"
#include "../include/stockerrs.h"
#include "../include/wcntexit.h"

#include <iostream>

#ifdef DEBUG_MSG
#define jwm_err(fmt, ... )                           \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(err_msg, STRBUFLEN, fmt, __VA_ARGS__);  \
}
#else
#define jwm_err(fmt, ... ) \
    cfmt(err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif


jwmsynth::jwmsynth() :
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
        jwm_err("%s", synthfile_reader->get_wc_error_msg());
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
    if (!jwm.get_connectlist()->make_connections()) {
        jwm_err("%s", connector::get_connect_err_msg());
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
    sm = jwm.get_modlist()->goto_first();
    while(sm) {
        if ((et = sm->validate()) != stockerrs::ERR_NO_ERROR) {
            jwm_err("Module %s is a little odd, %s %s %s",
                    sm->get_username(), synthmod::get_error_msg(),
                    jwm.get_stockerrs()->get_prefix_err(et),
                    jwm.get_stockerrs()->get_err(et));
            return false;
        }
        if (!sm->flag(synthmod::SM_VALID)){
            jwm_err("Module %s had problems initialising...",
                    sm->get_username(), synthmod::get_error_msg());
            return false;
        }
        sm = jwm.get_modlist()->goto_next();
    }
    if (jwm.get_modlist()->get_first_of_type(synthmodnames::WCNTEXIT)==0)
    {
        jwm_err("%s", "No wcnt_exit module created. Without this module "
                                         "I won't know when to stop.");
        return false;
    }
    return true;
}

bool jwmsynth::init_synth()
{
    if (jwm.is_verbose())
        std::cout << "\nInitialising modules...";
    synthmod* sm;
    synthmodlist* sml = jwm.get_modlist();
    sm = sml->goto_first();
    while(sm) {
        sm->init();
        #ifdef IO_DEBUG
        if (!sm->check_inputs()) {
            jwm_err("%s", synthmod::get_error_msg());
            return false;
        }
        #endif
        if (!sm->flag(synthmod::SM_VALID)){
            jwm_err("%s", synthmod::get_error_msg());
            return false;
        }
        sm = sml->goto_next();
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
    const short* bar = jwm.get_exit_in_bar();
    short exit_bar = jwm.get_exit_bar();
    // unlink any constant modules from list as it's pointless
    // calling run() on them...
    if (jwm.is_verbose())
        std::cout << "\nUnlinking empty-run modules from run list:";
    jwm.get_modlist()->remove_empty_run_modules();
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
    synthmod** runlist = jwm.get_modlist()->get_run_list();
    long count;

    std::cout << "samplerate: " << jwm.samplerate() << std::endl;

    while (*bar < exit_bar && *force_abort == OFF)
    {
        sm = runlist[count = 0];
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
    if (runlist)
        destroy_array_moved_from_list(runlist);
    return true;
}
