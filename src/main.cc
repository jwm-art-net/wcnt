//-----------------------------------------------------------------------|
//                                                                       |
//                     O---\      [:::>  <:::]      /---O                |
//                     \ ----------   wcnt   ---------- /                |
//                          Wav Composer Not Toilet!                     |
//                     / ----------   1.26   ---------- \                |
//                     O---/      [::>    <::]      \---O                |
//                                     ||                                |
//                          ========================                     |
//                          | 2007 James W. Morris.|                     |
//                          ------------------------                     |
//                          |  Made In Stourmouth  |                     |
//                          ------------------------                     |
//                                                                       /
// contact: James_W.Morris ( james@jwm-art.net )                         /
//                                                                       /
//----------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "../include/cmdline.h"
#include "../include/jwmsynth.h"
#include "../include/jwm_globals.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmodlist.h"
#include "../include/sanity_checks.h"

void title();
void exitramblings();

void test();

int main(const int argc, const char** const argv)
{
    #ifdef SANITY_CHECKS
    sanity_checks();
    return 0;
    #endif
    cmdline* cmd = new cmdline(argc, argv);
    // ladspa_loader uses dynamic linker - which disliked it when
    // dlclose was called while globals were destructing... so
    // ladspa_loader is created/deleted here now instead... along
    // with modlist which needed to be delete BEFORE ladspa_loader.
    #ifdef WITH_LADSPA
    ladspa_loader* ladspaloader = new ladspa_loader;
    jwm.register_ladspaloader(ladspaloader);
    #endif
    synthmodlist*
        modlist = new synthmodlist(DELETE_DATA);
    jwm.register_modlist(modlist);
    // the above is needed before cmd->scan because scan might be
    // prompted (by commandline) to create help (ie for modules etc).
    cmd->scan();
/*    cmd->set_jwm_globals();*/
    if (!jwm.is_no_title())
        title();
    if (!cmd->is_good_opts()) {
        std::cout << cmd->get_message() << std::endl;
        delete cmd;
        delete modlist;
        #ifdef WITH_LADSPA
        delete ladspaloader;
        #endif
        return -1;
    }
    jwmsynth* thesynth = new jwmsynth();
    if (   !thesynth->is_valid()       || !thesynth->generate_synth()
        || !thesynth->validate_synth() || !thesynth->connect_synth()
        || !thesynth->init_synth()     || !thesynth->execute_synth())
    {
        std::cout << thesynth->get_error_msg();
        std::cout << "\n... exiting program ...\n";
        delete thesynth;
        delete cmd;
        delete modlist;
        #ifdef WITH_LADSPA
        delete ladspaloader;
        #endif
        return -1;
    }
    delete thesynth;
    delete cmd;
    if (jwm.is_no_title())
        std::cout << std::endl;
    else
        exitramblings();
    delete modlist;
    #ifdef WITH_LADSPA
    delete ladspaloader;
    #endif
    return 0;
}

void title()
{
    std::cout << "O---\\      [:::>  <:::]      /---O" << std::endl;
    std::cout << "\\ ----------   wcnt   ---------- /" << std::endl;
    std::cout << "     Wav Composer Not Toilet!     " << std::endl;
    std::cout << "/ ---------- " << std::setw(8) << wcnt_version
              << " ---------- \\" << std::endl;
    std::cout << "O---/      [::>    <::]      \\---O" << std::endl;
}

void exitramblings()
{
    std::cout << "\nFlush complete.  Please wash your &&s, ";
    std::cout << "naturally.\n";
}
