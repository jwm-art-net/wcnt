#ifndef SC1_H
#define SC1_H
#ifdef WITH_LADSPA

#include "ladspa_loader.h"
#include "synthmodule.h"

/*
Plugin Name: "SC1"
Plugin Label: "sc1"
Plugin Unique ID: 1425
Maker: "Steve Harris <steve@plugin.org.uk>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: No
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports: 0 "Attack time (ms)" input, control, 2 to 400, default 101.5
       1 "Release time (ms)" input, control, 2 to 800, default 401
       2 "Threshold level (dB)" input, control, -30 to 0, default 0
       3 "Ratio (1:n)" input, control, 1 to 10, default 1
       4 "Knee radius (dB)" input, control, 1 to 10, default 3.25
       5 "Makeup gain (dB)" input, control, 0 to 24, default 0
       6 "Input" input, audio
       7 "Output" output, audio
*/

class sc1 : public synthmod
{
 public:
    sc1(char const*);
    ~sc1();
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
    const double* input;
    const double* in_thresh_mod;
    const double* in_ratio_mod;
    const double* in_knee_mod;
    const double* in_makeup_mod;
    // output
    double output;

    // params
    double attack;
    double release;
    double thresh;
    double ratio;
    double knee;
    double makeup;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data  l_attack;
    LADSPA_Data  l_release;
    LADSPA_Data  l_thresh;
    LADSPA_Data  l_ratio;
    LADSPA_Data  l_knee;
    LADSPA_Data  l_makeup;
    LADSPA_Data* l_input;
    LADSPA_Data* l_output;

    void create_params();
    static bool done_params;
};

#endif // WITH_LADSPA
#endif // SC1_H

