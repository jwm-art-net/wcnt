#ifndef SYNTHMODNAMES_H
#include "../include/synthmodnames.h"

synthmodnames::synthmodnames()
:mod_name(NULL)
{
	mod_name = new string[MOD_LAST + 1];
	if (mod_name == NULL)
	{
		cout << "\nextreme lack of memory, exiting....";
		cout << "\ncould not create list of module names.";
		exit(1);
	}
	mod_name[MOD_FIRST] = "BAD_MODULE_TYPE";
	mod_name[MOD_ADSR] = "adsr";
	mod_name[MOD_STEREOAMP] = "stereo_amp";
	mod_name[MOD_CLOCK] = "clock";
	mod_name[MOD_CONSTMOD] = "constant";
	mod_name[MOD_FREQGEN] = "freq_generator";
	mod_name[MOD_LFOCLOCK] = "lfo_clock";
	mod_name[MOD_LFOCONTROL]	= "lfo_controller";
	mod_name[MOD_LPFILTER] = "lp_filter";
	mod_name[MOD_MODIFIER] = "modifier";
	mod_name[MOD_NOISEGEN] = "noise_generator";
	mod_name[MOD_NONEZERO] = "none";
	mod_name[MOD_OSCCLOCK] = "osc_clock";
	mod_name[MOD_SAMPLEHOLD] = "sample_hold";
	mod_name[MOD_SEQUENCER] = "sequencer";
	mod_name[MOD_SINEWAVE] = "sine_wave";
	mod_name[MOD_SQUAREWAVE]	= "square_wave";
	mod_name[MOD_TRIGGER] = "trigger";
	mod_name[MOD_TRIWAVE] = "tri_wave";
	mod_name[MOD_TRIWAVE2] = "tri_wave2";
	mod_name[MOD_USERWAVE] = "user_wave";
	mod_name[MOD_SAMPLER]	= "sampler";
	mod_name[MOD_WAVFILEOUT] = "wavfile_out";
	mod_name[MOD_STEREOCHANNEL] = "mix_chan";
	mod_name[MOD_STEREOMIXER] = "mixer";
	mod_name[MOD_RANDTRIGGER] = "rnd_trigger";
	mod_name[MOD_LOGICTRIGGER] = "logic_trigger";
	mod_name[MOD_SWITCHER] = "switcher";
	mod_name[MOD_WCNTSIGNAL] = "wcnt_signal";
	mod_name[MOD_HPFILTER] = "hp_filter";
	mod_name[MOD_COMBINER] = "combiner";
}

synthmodnames::~synthmodnames()
{
	if (mod_name) 
		delete[] mod_name;
}

string & synthmodnames::get_name(SYNTH_MOD_TYPE id)
{
		if (id > MOD_FIRST && id < MOD_LAST) 
			return mod_name[id];
		else 
			return mod_name[MOD_FIRST];
}

synthmodnames::SYNTH_MOD_TYPE synthmodnames::get_type(string const* mname)
{
	int i;
	for (i = MOD_FIRST + 1; i < MOD_LAST; i++)
		if (mod_name[i] == *mname)
			return (SYNTH_MOD_TYPE)i;
	return MOD_FIRST;
}

#endif
