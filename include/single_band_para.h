#ifndef SINGLE_BAND_PARA_H
#define SINGLE_BAND_PARA_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "Single band parametric"
Plugin Label: "singlePara"
Plugin Unique ID: 1203
Maker: "Steve Harris <steve@plugin.org.uk>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports: 0 "Gain (dB)" input, control, -70 to 30, default 0
       1 "Frequency (Hz)" input, control, 0 to 0.4*srate, default 440
       2 "Bandwidth (octaves)" input, control, 0 to 4, default 1
       3 "Input" input, audio, -1 to 1
       4 "Output" output, audio, -1 to 1
*/
class single_band_para : public synthmod::base
{
 public:
    single_band_para(const char*);
    ~single_band_para();

    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool        set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
 private:
    // inputs
    const double* in_signal;
    const double* in_phase_step;
    const double* in_gain_mod;
    const double* in_bandwidth_mod;
    // output
    double output;
    // params
    double gain_db;
    double gain_mod_size;
    double bandwidth;
    double bandwidth_mod_size;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data* l_input;
    LADSPA_Data* l_output;
    LADSPA_Data  l_gain_db;
    LADSPA_Data  l_frequency;
    LADSPA_Data  l_bandwidth;

    // working
    // float min_freq;
    float max_freq;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif // WITH_LADSPA
#endif // SINGLE_BAND_PARA_H
