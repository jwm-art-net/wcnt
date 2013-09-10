#ifndef FAST_LOOKAHEAD_LIMITER_H
#define FAST_LOOKAHEAD_LIMITER_H
#ifdef WITH_LADSPA

#include "synthmodule.h"
#include "ladspa_loader.h"

/*
Plugin Name: "Fast Lookahead limiter"
Plugin Label: "fastLookaheadLimiter"
Plugin Unique ID: 1913
Maker: "Steve Harris <steve@plugin.org.uk>"
Copyright: "GPL"
Must Run Real-Time: No
Has activate() Function: Yes
Has deactivate() Function: No
Has run_adding() Function: Yes
Environment: Normal or Hard Real-Time
Ports:
    0 "Input gain (dB)" input, control, -20 to 20, default 0
    1 "Limit (dB)" input, control, -20 to 0, default 0
    2 "Release time (s)" input, control, 0.01 to 2, default 0.5075
    3 "Attenuation (dB)" output, control, 0 to 70
    4 "Input 1" input, audio
    5 "Input 2" input, audio
    6 "Output 1" output, audio
    7 "Output 2" output, audio
    8 "latency" output, control
*/

class fast_lookahead_limiter : public synthmod
{
 public:
    fast_lookahead_limiter(char const*);
    ~fast_lookahead_limiter();
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
    const double* in_left;
    const double* in_right;

    // output
    double out_left;
    double out_right;

    // params
    double gain_db;
    double limit_db;
    double release_secs;

    // LADSPA data
    const LADSPA_Descriptor* l_descriptor;
    LADSPA_Handle            l_inst_handle;
    LADSPA_Data  l_gain_db;
    LADSPA_Data  l_limit_db;
    LADSPA_Data  l_release_secs;
    LADSPA_Data  l_out_atten;
    LADSPA_Data* l_in_left;
    LADSPA_Data* l_in_right;
    LADSPA_Data* l_out_left;
    LADSPA_Data* l_out_right;
    LADSPA_Data  l_out_latency;

    void create_params();
    static bool done_params;
};

#endif // WITH_LADSPA
#endif // FAST_LOOKAHEAD_LIMITER_H
