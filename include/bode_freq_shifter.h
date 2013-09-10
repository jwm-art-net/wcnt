#ifndef BODE_FREQ_SHIFTER_H
#define BODE_FREQ_SHIFTER_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "Bode frequency shifter"
Plugin Label: "bodeShifter"
Plugin Unique ID: 1431
Maker: "Steve Harris <steve@plugin.org.uk>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: No
Has deativate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports:  "Frequency shift" input, control, 0 to 5000, default 0
        "Input" input, audio
        "Down out" output, audio
        "Up out" output, audio
        "latency" output, control
*/

class bode_freq_shifter : public synthmod
{
 public:

    bode_freq_shifter(char const*);
    ~bode_freq_shifter();

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
    const double* in_shift_mod;

    // output
    double out_up;
    double out_down;

    // params
    double freq_shift;
    double shift_modsize;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data l_freq_shift;
    LADSPA_Data* l_input;
    LADSPA_Data* l_out_down;
    LADSPA_Data* l_out_up;
    LADSPA_Data* l_out_latency;

    void create_params();
    static bool done_params;
};

#endif // WITH_LADSPA
#endif // BODE_FREQ_SHIFTER_H
