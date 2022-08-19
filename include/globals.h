#ifndef GLOBALS_H
#define GLOBALS_H

#include "cmdline.h"
#include "ui_moditem.h"
#include "wcntexit.h"


// forward definitions
class modoutputlist;
class connectorlist;
class fxsparamlist;
#ifdef WITH_LADSPA
class ladspa_loader;
#endif
class stockerrs;
class wave_tables;

namespace dobj {
    class list;
};

namespace synthmod {
    class list;
};


class synthfilereader;

namespace wcnt
{
 const char* const wcnt_version = "1.30.2022";
 const char* const wcnt_id = "wcnt-1.30/jwmsynth";
 const samp_t default_samplerate          = 44100UL;
 const wcint_t min_bpm                    = 10;
 const wcint_t max_bpm                    = 1000;
 const wcint_t default_beats_per_measure  = 4;
 const wcint_t default_beat_value         = 4;
 const wcint_t note_array_size            = 10;
 const wcint_t note_name_len              = 9;
 const wcint_t filter_array_size          = 8192;
 const wcint_t wav_buffer_size            = 4096; //16384;
 const wcint_t max_xfade_samples          = wav_buffer_size / 2 - 1;
 const wcint_t sr_div_bi                  = 8;

 const char* const ladspa_path_if_env_not_set =
    "/usr/local/lib/ladspa/:/usr/lib/ladspa/";

 const wcint_t group_control_stopping_samples = 1;

 modoutputlist*     get_outputlist();
 connectorlist*     get_connectlist();
 fxsparamlist*      get_fxsparamlist();
 dobj::list*        get_dobjlist();

 class globals
 {
  public:
    enum { MIN_BPM = 20, MAX_BPM = 1000 };

    bool friend cmdline::set_jwm_globals();
    void friend wcnt_exit::init();

    globals();
    ~globals();

    wcint_t         exit_bar() const    { return x_exit_bar; }
    const wcint_t*  exit_in_bar() const { return x_in_bar; }

    const char* path() const    { return wc_path;   }
    const char* file() const    { return wc_file;   }
    bool is_verbose() const     { return verbose;   }
    bool is_dont_run() const    { return dont_run;  }
    bool is_no_title() const    { return no_title;  }
    bool is_no_progress() const { return no_progress; }

    void set_verbose(bool v)    { verbose = v; }

    samp_t  samplerate() const  { return sample_rate;   }
    void    samplerate(samp_t);

    double get_mid_a_phase_step() const { return mid_a_phase_step; }

    // synthmodlist and ladspa_loader need to be created elsewhere
    void register_modlist(synthmod::list* sml)  { modlist = sml;   }
    synthmod::list* get_modlist() const         { return modlist;  }
    #ifdef WITH_LADSPA
    void register_ladspaloader(ladspa_loader* ll)   { ladspaloader = ll;}
    ladspa_loader*  get_ladspaloader() const        { return ladspaloader; }
    #endif

  private:
    synthmod::list* modlist;

    char* wc_path;
    char* wc_file;

    wcint_t x_exit_bar;
    wcint_t const* x_in_bar;

    bool verbose;
    bool dont_run;
    bool no_title;
    bool no_progress;

    samp_t sample_rate;

    double mid_a_phase_step;

    #ifdef WITH_LADSPA
    ladspa_loader*  ladspaloader;
    #endif
 };

 extern globals jwm;

}; // namespace wcnt
#endif
