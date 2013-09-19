#include "../include/jwm_globals.h"
#include "../include/jwm_init.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"
#include "../include/dobjparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/topdobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/connectorlist.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"
#include "../include/wave_tables.h"
#include "../include/conversions.h"

#ifdef MOD_STATS
#define SHOWCOUNTS
#endif

#ifdef DOBJ_STATS
#define SHOWCOUNTS
#endif

#ifdef LIST_STATS
#define SHOWCOUNTS
#include "../include/linkedlist.h"
#endif

#ifdef NOTE_STATS
#define SHOWCOUNTS
#include "../include/notedata.h"
#endif

#ifdef SHOWCOUNTS
#include <iostream>
#endif

jwm_globals::jwm_globals() :
 wc_path(0),    wc_file(0),
 x_exit_bar(0), x_in_bar(0),
 verbose(false), dont_run(false), no_title(false),
 sample_rate(   jwm_init::default_samplerate),
 iocatnames(0), modnames(0), innames(0), outnames(0), parnames(0),
 dbjnames(0), modlist(0), inputlist(0), outputlist(0), paramlist(0),
 connectlist(0), moddbjlist(0), fxsparlist(0), dbjlist(0), dparlist(0),
 topdbjlist(0),
 #ifdef WITH_LADSPA
 ladspaloader(0),
 #endif
 stock_errs(0)
{
    iocatnames  = new iocat;
    modnames    = new synthmodnames;
    innames     = new inputnames;
    outnames    = new outputnames;
    parnames    = new paramnames;
    dbjnames    = new dobjnames;
    inputlist   = new modinputlist;
    outputlist  = new modoutputlist;
    paramlist   = new modparamlist;
    fxsparlist  = new fxsparamlist;
    dparlist    = new dobjparamlist;
    dbjlist     = new dobjlist;
    topdbjlist  = new topdobjlist;
    moddbjlist  = new moddobjlist;
    connectlist = new connectorlist;
    stock_errs  = new stockerrs;
    wavetables  = new wave_tables;
}

jwm_globals::~jwm_globals()
{
    delete iocatnames;
    delete modnames;
    delete innames;
    delete outnames;
    delete parnames;
    delete dbjnames;
    delete inputlist;
    delete outputlist;
    delete paramlist;
    delete fxsparlist;
    delete dparlist;
    delete dbjlist;
    delete topdbjlist;
    delete moddbjlist;
    delete connectlist;
    delete stock_errs;
    delete wavetables;
    // these created by friend function cmdline::set_jwm_globals()
    delete [] wc_path;
    delete [] wc_file;

#ifdef SHOWCOUNTS
#ifdef LIST_STATS
STATS_DISPLAY(ll_item)
STATS_DISPLAY(linkedlist)
#endif

#ifdef MOD_STATS
STATS_DISPLAY(synthmod)
#endif

#ifdef DOBJ_STATS
STATS_DISPLAY(dobj)
#endif

#ifdef NOTE_STATS
STATS_DISPLAY(note_data)
#endif
    std::cout << std::endl;
#endif
}

void jwm_globals::samplerate(unsigned long sr)
{
    sample_rate = sr;
    mid_a_phase_step = freq_to_step(440);
}

jwm_globals jwm;

