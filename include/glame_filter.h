#ifndef GLAME_FILTER_H
#define GLAME_FILTER_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "Glame Highpass Filter"
Plugin Label: "highpass_iir"
Plugin Unique ID: 1890
Maker: "Alexander Ehlert <mag@glame.de>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports:  "Cutoff Frequency" input, control, 1e-04*srate to 0.45*srate, default 0.000819036*srate, logarithmic
        "Stages(2 poles per stage)" input, control, 1 to 10, default 1, integer
        "Input" input, audio
        "Output" output, audio
*/

class glame_filter : public synthmod
{
 public:
    glame_filter(char const*);
    ~glame_filter();

    enum F_TYPE{ LOPASS, HIPASS };

    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool        set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
 private:
    // inputs
    const double* in_signal;
    const double* in_freq_mod1;
    // output
    double output;
    // params
    F_TYPE type;
    double cutoff_freq;
    double freq_mod1size;
    short  stages;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data l_cutoff_freq;
    LADSPA_Data l_stages;
    LADSPA_Data* l_input;
    LADSPA_Data* l_output;

    const char* type_names[2];

    // working
    float min_cutoff;
    float max_cutoff;

    void create_params();
    static bool done_params;
};

#endif // WITH_LADSPA
#endif // GLAME_FILTER_H
