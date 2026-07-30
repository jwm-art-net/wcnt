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
 const char* const version = "1.32.2026";
 const char* const file_id = "wcnt-1.32/jwmsynth";
 const char* const email = "jwm.art.net@gmail.com";
 const samp_t samplerate_default = 44100UL;
 const samp_t samplerate_min     = 100UL;
 const samp_t samplerate_max     = 500000UL;

 const wcint_t bpm_min                    = 10;
 const wcint_t bpm_max                    = 1000;
 const wcint_t beats_per_measure_default  = 4;
 const wcint_t beat_value_default         = 4;
 const wcint_t note_array_size            = 10;
 const wcint_t note_name_len              = 9;
 const wcint_t filter_array_size          = 8192;
 const wcint_t wav_buffer_size            = 4096; //16384;
 const wcint_t max_xfade_samples          = wav_buffer_size / 2 - 1;
 const wcint_t sr_div_bi                  = 8;
 const wcint_t max_connection_postponement_level = 10;
 const wcint_t max_copies                 = 50;


 const char* const ladspa_path_if_env_not_set =
    "/usr/local/lib/ladspa/:/usr/lib/ladspa/";

 const wcint_t group_control_stopping_samples = 1;

 modoutputlist*     get_outputlist();
 connectorlist*     get_connectlist();
 fxsparamlist*      get_fxsparamlist();
 dobj::list*        get_dobjlist();

 bool could_be_header(const char* header);
 bool header_is_compatible(const char* header);
 bool name_is_reserved_word(const char*);


 class globals
 {
  public:
    enum { MIN_BPM = 20, MAX_BPM = 1000 };

    enum GFLAGS {
        VERBOSE =    0x0001,
        DONTRUN =    0x0002,
        NOTITLE =    0x0004,
        NOPROGRESS = 0x0008,
        SR_DEFAULT = 0x0010
    };

    bool friend cmdline::set_jwm_globals();
    void friend wcnt_exit::init();

    globals();
    ~globals();

    wcint_t         exit_bar() const     { return x_exit_bar; }
    const wcint_t*  exit_in_bar() const  { return x_in_bar; }
    const STATUS*   exit_in_trig() const { return x_in_trig; }

    const char* path() const    { return wc_path;   }
    const char* file() const    { return wc_file;   }
    bool is_verbose() const     { return flags & VERBOSE; }
    bool is_dont_run() const    { return flags & DONTRUN; }
    bool is_no_title() const    { return flags & NOTITLE; }
    bool is_no_progress() const { return flags & NOPROGRESS; }
    bool is_sr_default() const  { return flags & SR_DEFAULT; }

    void set_verbose(bool f)    { flags &= flags | (f ? VERBOSE : 0); }
    void set_verbose()     { flags |= VERBOSE; }
    void set_dont_run()    { flags |= DONTRUN; }
    void set_no_title()    { flags |= NOTITLE; }
    void set_no_progress() { flags |= NOPROGRESS; }

    samp_t  samplerate() const  { return sample_rate;   }
    samp_t  samplerate(samp_t);

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
    STATUS const* x_in_trig;

    int flags;

    samp_t sample_rate;

    double mid_a_phase_step;

    #ifdef WITH_LADSPA
    ladspa_loader*  ladspaloader;
    #endif
 };

 extern globals jwm;

}; // namespace wcnt
#endif
