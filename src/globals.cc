#include "../include/globals.h"
#include "../include/modoutputlist.h"
#include "../include/fxsparamlist.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/connectorlist.h"
#include "../include/ladspa_loader.h"
#include "../include/synthmod.h"
#include "../include/dobj.h"
#include "../include/wave_tables.h"
#include "../include/conversions.h"
#include "../include/ui_moditem.h"
#include "../include/ui_dobjitem.h"
#include "../include/directivenames.h"

#ifdef DATA_STATS
#include "../include/linkedlist.h"
#include "../include/notedata.h"
#endif


namespace wcnt
{
 modoutputlist* get_outputlist()
 {
    static modoutputlist outlist;
    return &outlist;
 }

 connectorlist* get_connectlist()
 {
    static connectorlist conlist;
    return &conlist;
 }

 fxsparamlist* get_fxsparamlist()
 {
    static fxsparamlist fxsparlist;
    return &fxsparlist;
 }

 dobj::list* get_dobjlist()
 {
    static dobj::list dbjlist;
    return &dbjlist;
 }

bool could_be_header(const char* header)
{
    if (strncmp("wcnt", header, 4) != 0)
        return false;
    const char* ptr = header;
    while (*ptr++ != 0);
    ptr -= 9;
    if (ptr <= header)
        return false;
    if (strcmp(ptr, "jwmsynth") != 0)
        return false;
    return true;

}

 bool header_is_compatible(const char* header)
 {
    static const char* compathdrs [] = {
       wcnt::file_id,
       0
    };
    for (int i = 0; compathdrs[i] != 0; i++) {
       if (strcmp(header, compathdrs[i]) == 0)
          return true;
    }
    return false;
 }

 bool name_is_reserved_word(const char* name)
 {
    const char* reserved[] =
    {
         "off",
         dobj::names::get(dobj::LST_EDITS),
         dobj::names::get(dobj::SIN_EDIT),
         0
    };
    for (int i = 0; reserved[i] != 0; i++)
        if (strcmp(name, reserved[i]) == 0)
            return true;
    return false;
 }


 globals::globals() :
  modlist(0),
  wc_path(0),    wc_file(0),
  x_exit_bar(0), x_in_bar(0),
  verbose(false), dont_run(false), no_title(false), no_progress(false),
  sample_rate(wcnt::default_samplerate), mid_a_phase_step(0),
  #ifdef WITH_LADSPA
  ladspaloader(0)
  #endif
 {
    iocat::names::instantiate();
    input::names::instantiate();
    output::names::instantiate();
    param::names::instantiate();
    synthmod::names::instantiate();
    dobj::names::instantiate();
    directive::names::instantiate();
    errors::stock::instantiate();
    wave_tables::instantiate();
 }

 globals::~globals()
 {
    // these created by friend function cmdline::set_globals()
    delete [] wc_path;
    delete [] wc_file;

    #ifdef DATA_STATS
    STATS_DISPLAY(synthmod)
    STATS_DISPLAY(modinput)
    STATS_DISPLAY(modoutput)
    STATS_DISPLAY(connector)
    STATS_DISPLAY(modparam)
    STATS_DISPLAY(dobj)
    STATS_DISPLAY(note_data)
    std::cout << std::endl;
    #endif
 }

 void globals::samplerate(samp_t sr)
 {
    sample_rate = sr;
    mid_a_phase_step = freq_to_step(440);
 }

 globals jwm;

}; // namespace wcnt
