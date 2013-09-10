#ifndef JWM_GLOBALS_H
#define JWM_GLOBALS_H

// forward definitions
class iocat;
class synthmodnames;
class inputnames;
class outputnames;
class paramnames;
class dobjnames;
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

const char* const wcnt_version = "1.26.3";
const char* const wcnt_id = "wcnt-1.26/jwmsynth";

class jwm_globals
{
 public:
    jwm_globals();
    ~jwm_globals();

    enum { MIN_BPM = 20, MAX_BPM = 1000 };

    bool friend cmdline::set_jwm_globals();
    void friend wcnt_exit::init();

    short get_exit_bar(){ return x_exit_bar; }
    short const* get_exit_in_bar(){ return x_in_bar; }

    const char* const path()    { return wc_path;   }
    const char* const file()    { return wc_file;   }
    bool is_verbose()           { return verbose;   }
    bool is_dont_run()          { return dont_run;  }
    bool is_no_title()          { return no_title;  }

    void set_verbose(bool v)    { verbose = v; }

    unsigned long samplerate()  { return sample_rate;   }

    void    samplerate(unsigned long sr);

    double & get_mid_a_phase_step() { return mid_a_phase_step; }

    const iocat*            get_iocatnames() const  { return iocatnames;}
    const synthmodnames*    get_modnames() const    { return modnames;  }
    const inputnames*       get_inputnames() const  { return innames;   }
    const outputnames*      get_outputnames() const { return outnames;  }
    const paramnames*       get_paramnames() const  { return parnames;  }
    const dobjnames*        get_dobjnames() const   { return dbjnames;  }

    modinputlist*   get_inputlist() const   { return inputlist;    }
    modoutputlist*  get_outputlist() const  { return outputlist;   }
    modparamlist*   get_paramlist() const   { return paramlist;    }
    connectorlist*  get_connectlist() const { return connectlist;  }
    moddobjlist*    get_moddobjlist() const { return moddbjlist;   }
    fxsparamlist*   get_fxsparamlist() const{ return fxsparlist;   }
    dobjlist*       get_dobjlist() const    { return dbjlist;      }
    dobjparamlist*  get_dparlist() const    { return dparlist;     }
    topdobjlist*    get_topdobjlist() const { return topdbjlist;   }
    stockerrs*      get_stockerrs() const   { return stock_errs;   }
    wave_tables*    get_wave_tables() const { return wavetables;   }

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

    short x_exit_bar;
    short const* x_in_bar;

    bool verbose;
    bool dont_run;
    bool no_title;

    unsigned long sample_rate;

    double mid_a_phase_step;

    iocat*          iocatnames;
    synthmodnames*  modnames;
    inputnames*     innames;
    outputnames*    outnames;
    paramnames*     parnames;
    dobjnames*      dbjnames;
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
    stockerrs*      stock_errs;
    wave_tables*    wavetables;
} extern jwm;

#endif
