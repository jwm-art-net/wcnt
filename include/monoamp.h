#ifndef MONOAMP_H
#define MONOAMP_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class mono_amp : public synthmod 
{
 public:
	mono_amp(string uname);
	~mono_amp();
	void set_input_signal(const double* in){ in_signal = in; }
	void set_input_amp_eg(const double* aeg){ in_amp_eg = aeg; }
	void set_input_amp_mod(const double* am){ in_amp_mod = am; }
	const double* get_input_signal(){ return in_signal; }
	const double* get_input_amp_eg(){ return in_amp_eg; }
	const double* get_input_amp_mod(){ return in_amp_mod; }
	const short* get_output_mono(){ return &out_mono; }
	const double* get_output_m(){ return &out_m; }
	void set_amplitude(short a){ amplitude = a; }
	void set_amp_modsize(double ams){ amp_modsize = ams; }
	void set_clip_level(short cl){ clip_level = cl; }
	int get_amplitude(){ return amplitude; }
	double get_amp_modsize(){ return amp_modsize; }
	short get_clip_level(){ return clip_level; }
	// virtual funcs
	void run();
	bool validate();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	// inputs
	const double* in_signal;
	const double* in_amp_eg;
	const double* in_amp_mod;
 	// outputs
	short out_mono;
	double out_m;
 	// params
	short amplitude;
	double amp_modsize;
	short clip_level;
 	// working
	double amp_level;
	double ampsig;
	static int monoamp_count;
 	#ifndef BARE_MODULES
	void create_params();
	static bool done_params;
	#endif
};

#endif
