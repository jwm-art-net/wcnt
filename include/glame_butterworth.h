#ifndef GLAME_BUTTERWORTH_H
#define GLAME_BUTTERWORTH_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "GLAME Butterworth Highpass"
Plugin Label: "butthigh_iir"
Plugin Unique ID: 1904
Maker: "Alexander Ehlert <mag@glame.de>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time

Ports:
    "Cutoff Frequency (Hz)" input, control,
        1e-04*srate to 0.45*srate, default 0.000819036*srate, logarithmic

    "Resonance" input, control, 
        0.1 to 1.41, default 0.755

    "Input" input, audio

    "Output" output, audio
*/

class glame_butterworth : public synthmod
{
 public:
    glame_butterworth(char const*);
    ~glame_butterworth();
    enum GB_TYPE{ LOPASS, HIPASS };
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
    const double* in_phase_step; // cutoff freq 
    const double* in_res_mod;
    // output
    double output;
    // params
    GB_TYPE type;
    double resonance;
    double res_mod_size;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data* l_input;
    LADSPA_Data* l_output;
    LADSPA_Data  l_cut_off;
    LADSPA_Data  l_resonance;

    const char* type_names[2];

    // working
    float min_cut_off;
    float max_cut_off;

    void create_params();
    static bool done_params;
};

#endif // WITH_LADSPA
#endif // GLAME_BUTTERWORTH_H
