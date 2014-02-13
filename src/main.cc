//-----------------------------------------------------------------------|
//                                                                       |
//                     O---\      [:::>  <:::]      /---O                |
//                     \ ----------   wcnt   ---------- /                |
//                          Wav Composer Not Toilet!                     |
//                     / ----------   1.30   ---------- \                |
//                     O---/      [::>    <::]      \---O                |
//                                     ||                                |
//                          ========================                     |
//                          | 2007 James W. Morris.|                     |
//                          ------------------------                     |
//                          |  Made In Stourmouth  |                     |
//                          ------------------------                     |
//                          | 2014 James W. Morris.|                     |
//                          ------------------------                     |
//                          | Made In Cliftonville |                     |
//                          ========================                     |
//                                                                       /
// contact: James_W.Morris ( james@jwm-art.net )                         /
//                                                                       /
//----------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>

#include "../include/cmdline.h"
#include "../include/synth.h"
#include "../include/globals.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmodlist.h"
#include "../include/sanity_checks.h"
#include "../include/textstuff.h"


void title();
void exitramblings();

void test();

int main(const int argc, char** argv)
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

    ladspaloader->load_all();

    ladspa_lib* l = ladspaloader->goto_first();
    while(l) {
        debug("path: '%s'\n", l->get_path());
        for (int plug_ix = 0;; plug_ix++) {
            const LADSPA_Descriptor* descr = (l->get_descr_func())(plug_ix);
            if (!descr)
                break;
            debug("\tlabel: '%s'\n", descr->Label);
        }
        l = ladspaloader->goto_next();
    }




    wcnt::jwm.register_ladspaloader(ladspaloader);
    #endif
    synthmod::list* modlist = new synthmod::list(DELETE_DATA);
    wcnt::jwm.register_modlist(modlist);
    // the above is needed before cmd->scan because scan might be
    // prompted (by commandline) to create help (ie for modules etc).

    if (!cmd->scan()) {
        if (!wcnt::jwm.is_no_title())
            title();
        std::cout << cmd->get_message() << std::endl;
        delete cmd;
        delete modlist;
        #ifdef WITH_LADSPA
        delete ladspaloader;
        #endif
        return -1;
    }

    if (!wcnt::jwm.is_no_title())
        title();
/*
    int t;
    input::names::register_type("in:l", iocat::DOUBLE, "input for left stereo signal");
    input::names::register_type("Frequency", iocat::DOUBLE, "Frequency input hrtz");
    output::names::register_type("Output", iocat::DOUBLE, "Output");
    param::names::register_type("poopah(ms)", iocat::SAMP_T, "a pooey type thing");
    param::names::register_type("poopah bah bah", iocat::SAMP_T, "a pooey type thing");
    param::names::register_type("delay time", iocat::SAMP_T, "amount of time to delay");
    t = param::names::register_type("PooPah (ms)", iocat::SAMP_T, "a pooey type thing");
    const char* name = param::names::get(t);
    debug("name:'%s' found for id %d\n", name, t);
    param::names::register_type("Delay__Time__", iocat::DOUBLE, "amount of time to delay");
*/
    wcnt::synth* thesynth = new wcnt::synth();
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
    if (wcnt::jwm.is_no_title())
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
    std::cout << "              .    .          |||/" << std::endl;
    std::cout << "           [:::>  <:::]      /---O" << std::endl;
    std::cout << " .----------   wcnt   ---------- /" << std::endl;
    std::cout << "     Wav Composer Not Toilet!     " << std::endl;
    std::cout << "/ ---------- " << std::setw(8) << wcnt::wcnt_version
              << " ----------'" << std::endl;
    std::cout << "O---/     [::> ROTL <::]      " << std::endl;
    std::cout << "/|||" << std::endl;
}

void exitramblings()
{
    std::cout << "\nFlush complete. Please wash your &&s, ";
    std::cout << "naturally.\n";
}
