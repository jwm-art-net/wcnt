#ifndef CAPS_PLATE_H
#define CAPS_PLATE_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "CAPS: Plate - Versatile plate reverb"
Plugin Label: "Plate"
Plugin Unique ID: 1779
Maker: "Tim Goetze <tim@quitte.de>"
Copyright: "GPL, 2004-5"
Must Run Real-Time: No
Has activate() Function: Yes
Has deativate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports: 0 "in" input, audio, -1 to 1
       1 "bandwidth" input, control, 0.005 to 0.999, default 0.502
       2 "tail" input, control, 0 to 0.749, default 0.3745
       3 "damping" input, control, 0.0005 to 1, default 0.250375
       4 "blend" input, control, 0 to 1, default 0.25
       5 "out:l" output, audio
       6 "out:r" output, audio
*/

class caps_plate : public synthmod
{
 public:

    caps_plate(char const*);
    ~caps_plate();

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

    // output
    double out_left;
    double out_right;

    // params
    double bandwidth;
    double tail;
    double damping;
    double blend;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data* l_input;
    LADSPA_Data  l_bandwidth;
    LADSPA_Data  l_tail;
    LADSPA_Data  l_damping;
    LADSPA_Data  l_blend;
    LADSPA_Data* l_out_left;
    LADSPA_Data* l_out_right;

    void create_params();
    
};

#endif // WITH_LADSPA
#endif // CAPS_PLATE_H
