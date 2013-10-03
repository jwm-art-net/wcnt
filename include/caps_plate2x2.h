#ifndef CAPS_PLATE2X2_H
#define CAPS_PLATE2X2_H
#ifdef WITH_LADSPA

#include "synthmod.h"
#include "ladspa_loader.h"

/*
Plugin Name: "CAPS: Plate2x2 - Versatile plate reverb, stereo inputs"
Plugin Label: "Plate2x2"
Plugin Unique ID: 1795
Maker: "Tim Goetze <tim@quitte.de>"
Copyright: "GPL, 2004-5"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports: 0 "in:l" input, audio, -1 to 1
       1 "in:r" input, audio, -1 to 1
       2 "bandwidth" input, control, 0.005 to 0.999, default 0.502
       3 "tail" input, control, 0 to 0.749, default 0.3745
       4 "damping" input, control, 0.0005 to 1, default 0.250375
       5 "blend" input, control, 0 to 1, default 0.25
       6 "out:l" output, audio
       7 "out:r" output, audio
*/

class caps_plate2x2 : public synthmod
{
 public:

    caps_plate2x2(const char*);
    ~caps_plate2x2();

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool        set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:

    // inputs
    const double* in_left;
    const double* in_right;

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
    LADSPA_Data* l_in_left;
    LADSPA_Data* l_in_right;
    LADSPA_Data  l_bandwidth;
    LADSPA_Data  l_tail;
    LADSPA_Data  l_damping;
    LADSPA_Data  l_blend;
    LADSPA_Data* l_out_left;
    LADSPA_Data* l_out_right;

    void init_first();
};

#endif // WITH_LADSPA
#endif // CAPS_PLATE2X2_H
