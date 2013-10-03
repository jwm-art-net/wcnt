#ifndef JWM_GLOBALS_H
#define JWM_GLOBALS_H

// forward definitions
class synthmodlist;
class modinputlist;
class modoutputlist;
class modparamlist;
class connectorlist;
class moddobjlist;
class fxsparamlist;
class dobjlist;
class dobjparamlist;
class topdobjlist;
#ifdef WITH_LADSPA
class ladspa_loader;
#endif
class stockerrs;
class wave_tables;

#include "cmdline.h"
#include "wcntexit.h"

class synthfilereader;

const char* const wcnt_version = "1.26.5 ";
const char* const wcnt_id = "wcnt-1.26/jwmsynth";

class jwm_globals
{
 public:
    jwm_globals();
    ~jwm_globals();

    enum { MIN_BPM = 20, MAX_BPM = 1000 };

    bool friend cmdline::set_jwm_globals();
    void friend wcnt_exit::init();

    wcint_t get_exit_bar() const { return x_exit_bar; }
    wcint_t const* get_exit_in_bar() const { return x_in_bar; }

    const char* path() const    { return wc_path;   }
    const char* file() const    { return wc_file;   }
    bool is_verbose() const     { return verbose;   }
    bool is_dont_run() const    { return dont_run;  }
    bool is_no_title() const    { return no_title;  }

    void set_verbose(bool v)    { verbose = v; }

    samp_t  samplerate() const  { return sample_rate;   }
    void    samplerate(samp_t);

    double get_mid_a_phase_step() const { return mid_a_phase_step; }

    modinputlist*   get_inputlist() const   { return inputlist;    }
    modoutputlist*  get_outputlist() const  { return outputlist;   }
    modparamlist*   get_paramlist() const   { return paramlist;    }
    connectorlist*  get_connectlist() const { return connectlist;  }
    moddobjlist*    get_moddobjlist() const { return moddbjlist;   }
    fxsparamlist*   get_fxsparamlist() const{ return fxsparlist;   }
    dobjlist*       get_dobjlist() const    { return dbjlist;      }
    dobjparamlist*  get_dparlist() const    { return dparlist;     }
    topdobjlist*    get_topdobjlist() const { return topdbjlist;   }

/* these two dislike global space */
    void register_modlist(synthmodlist* sml)        { modlist = sml;   }
    synthmodlist*   get_modlist() const             { return modlist;  }
    #ifdef WITH_LADSPA
    void register_ladspaloader(ladspa_loader* ll)   { ladspaloader = ll;}
    ladspa_loader*  get_ladspaloader() const    { return ladspaloader; }
    #endif

 private:

    char* wc_path;
    char* wc_file;

    wcint_t x_exit_bar;
    wcint_t const* x_in_bar;

    bool verbose;
    bool dont_run;
    bool no_title;

    samp_t sample_rate;

    double mid_a_phase_step;

    synthmodlist*   modlist;
    modinputlist*   inputlist;
    modoutputlist*  outputlist;
    modparamlist*   paramlist;
    connectorlist*  connectlist;
    moddobjlist*    moddbjlist;
    fxsparamlist*   fxsparlist;
    dobjlist*       dbjlist;
    dobjparamlist*  dparlist;
    topdobjlist*    topdbjlist;
    #ifdef WITH_LADSPA
    ladspa_loader*  ladspaloader;
    #endif
    wave_tables*    wavetables;
} extern jwm;

#endif
