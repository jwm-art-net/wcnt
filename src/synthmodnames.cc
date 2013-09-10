#ifndef SYNTHMODNAMES_H
#include "../include/synthmodnames.h"

synthmodnames::synthmodnames()
        :mod_name(NULL)
{
    mod_name = new char*[MOD_LAST];
    mod_name[MOD_FIRST] = "BAD_MODULE_TYPE";
    mod_name[MOD_NONEZERO] = "none";
    mod_name[MOD_ADSR] = "adsr";
    mod_name[MOD_STEREOAMP] = "stereo_amp";
    mod_name[MOD_CLOCK] = "clock";
    mod_name[MOD_CONSTMOD] = "constant";
    mod_name[MOD_FREQGEN] = "freq_generator";
    mod_name[MOD_LFOCLOCK] = "lfo_clock";
    mod_name[MOD_LFOCONTROL] = "lfo_controller";
    mod_name[MOD_LPFILTER] = "lp_filter";
    mod_name[MOD_MODIFIER] = "modifier";
    mod_name[MOD_NOISEGEN] = "noise_generator";
    mod_name[MOD_OSCCLOCK] = "osc_clock";
    mod_name[MOD_SAMPLEHOLD] = "sample_hold";
    mod_name[MOD_SEQUENCER] = "sequencer";
    mod_name[MOD_SINEWAVE] = "sine_wave";
    mod_name[MOD_SQUAREWAVE] = "square_wave";
    mod_name[MOD_TRIGGER] = "trigger";
    mod_name[MOD_TRIWAVE] = "tri_wave";
    mod_name[MOD_TRIWAVE2] = "tri_wave2";
    mod_name[MOD_USERWAVE] = "user_wave";
    mod_name[MOD_SAMPLER] = "sampler";
    mod_name[MOD_WAVFILEOUT] = "wavfile_out";
    mod_name[MOD_STEREOCHANNEL] = "mix_chan";
    mod_name[MOD_STEREOMIXER] = "mixer";
    mod_name[MOD_RANDTRIGGER] = "rnd_trigger";
    mod_name[MOD_LOGICTRIGGER] = "logic_trigger";
    mod_name[MOD_SWITCHER] = "switcher";
    mod_name[MOD_WCNTSIGNAL] = "wcnt_signal";
    mod_name[MOD_HPFILTER] = "hp_filter";
    mod_name[MOD_COMBINER] = "combiner";
    mod_name[MOD_TIMEMAP] = "time_map";
    mod_name[MOD_CONTRASTER] = "contraster";
    mod_name[MOD_SERIALWAVFILEOUT] = "serial_wavout";
    mod_name[MOD_DELAY] = "delay";
    mod_name[MOD_ECHO] = "echo";
    mod_name[MOD_MONOAMP] = "mono_amp";
    mod_name[MOD_WCNT] = "wcnt_exit";
    mod_name[MOD_MULTIPLIER] = "multiplier";
    mod_name[MOD_RANGELIMIT] = "range_limit";
    mod_name[MOD_PAN] = "panner";
    mod_name[MOD_RMS] = "rms";
    mod_name[MOD_DCFILTER] = "dc_filter";
    mod_name[MOD_DYNAMIC] = "dynamic";
    mod_name[MOD_SPREADER] = "spreader";
    mod_name[MOD_NOTETRAN] = "note_tran";
    mod_name[MOD_WAITTRIG] = "wait_trig";
    mod_name[MOD_PATTERNTRIG] = "pattern_trig";
    mod_name[MOD_STATEGATETRIG] = "state_gate_trig";
    mod_name[MOD_INVERT] = "invert";
    mod_name[MOD_TIMER] = "timer";
}

synthmodnames::~synthmodnames()
{
    delete [] mod_name;
}

char const* synthmodnames::get_name(SYNTH_MOD_TYPE id)
{
    if (id > MOD_FIRST && id < MOD_LAST)
        return mod_name[id];
    else
        return mod_name[MOD_FIRST];
}

synthmodnames::SYNTH_MOD_TYPE synthmodnames::get_type(char const* mname)
{
    int i;
    for (i = MOD_FIRST + 1; i < MOD_LAST; i++)
        if (strcmp(mod_name[i], mname) == 0)
            return (SYNTH_MOD_TYPE)i;
    return MOD_FIRST;
}

#endif
