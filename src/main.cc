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

#include "../include/cmdline.h"
#include "../include/jwmsynth.h"
#include "../include/jwm_globals.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmodulelist.h"
#include "../include/sanity_checks.h"

void title();
void exitramblings();

int main(const int argc, const char** const argv)
{
    #ifdef SANITY_CHECKS
    sanity_checks();
    return 0;
    #endif
    cmdline* cmd = new cmdline(argc, argv);
    cmd->scan();
    cmd->set_jwm_globals();
    if (!jwm.is_no_title())
        title();
    if (!cmd->is_good_opts()) {
        std::cout << cmd->get_message() << "\n";
        delete cmd;
        return -1;
    }
    // ladspa_loader uses dynamic linker - which disliked it when
    // dlclose was called while globals were destructing... so
    // ladspa_loader is created/deleted here now instead... along
    // with modlist which needed to be delete BEFORE ladspa_loader.
    #ifdef WITH_LADSPA
    ladspa_loader* ladspaloader = new ladspa_loader;
    jwm.register_ladspaloader(ladspaloader);
    #endif
    synthmodlist* modlist = new synthmodlist;
    jwm.register_modlist(modlist);
    // the above is needed before cmd->scan because scan might be
    // prompted (by commandline) to create help (ie for modules etc).
    jwmsynth* thesynth = new jwmsynth();
    if (   !thesynth->is_valid()       || !thesynth->generate_synth()
        || !thesynth->connect_synth()  || !thesynth->validate_synth()
        || !thesynth->execute_synth())
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
        std::cout << "\n";
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
    std::cout << "\n  \\,  wcnt_\\\\.\\\\...\\.\\..  .       ^";
    std::cout << "\n-   )< Wav Composer Not Toilet  >(";
    std::cout << "\n  /'    '/'''/''///'  '   " << wcnt_version;
    std::cout << "!-" << std::endl;
}

void exitramblings()
{
    std::cout << "\nFlush complete.  Please wash your &&s, ";
    std::cout << "naturally.\n";
}
