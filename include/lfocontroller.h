#ifndef LFOCONTROLLER_H
#define LFOCONTROLLER_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class lfo_controller : public synthmod 
{
 public:
	lfo_controller(string uname);
	~lfo_controller();
	// inputs
	void set_input_trig(const STATUS* ti){ in_trig = ti; }
	void set_input_wave(const double* wfi){ in_wave = wfi; }
	void set_input_amp_mod(const double* am){ in_amp_mod = am; }
	const STATUS* get_input_trig(){ return in_trig; }
	const double* get_input_wave(){ return in_wave; }
	const double* get_input_amp_mod(){ return in_amp_mod; }
	// outputs
	const double* get_output(){ return &output; }
	const double* get_output_preampmod(){ return &out_preampmod; }
	// params
	void set_delay_time(double dt){ delay_time = dt; }
	void set_ramp_time(double rt){ ramp_time = rt; }
	void set_start_level(double sl){ start_level = sl; }
	void set_end_level(double el){ end_level = el; }
	void set_response_time(double rt){ response_time = rt;}
	void set_amp_modsize(double ams);
	double get_delay_time(){ return delay_time; }
	double get_ramp_time(){ return ramp_time; }
	double get_start_level(){ return start_level; }
	double get_end_level(){ return end_level; }
	double get_amp_modsize(){ return amp_modsize; }
	double get_response_time(){ return response_time;}
	// virtual funcs
	void run();
	void init();
	bool validate();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	// inputs
	const STATUS* in_trig;
	const double* in_wave;
	const double* in_amp_mod;
	// outputs
	double out_preampmod;
	double output;
 	// params
	double delay_time;
	double ramp_time;
	double start_level;
	double end_level;
	double response_time;
	double amp_modsize;
	// working
	double ams_r;
	unsigned long delay_samples;
	unsigned long ramp_samples;
	double resp_size;
 	double resp_fa_level;
	double resp_ac;
	double level_size;
	double current_level;
	static int lfo_controller_count;
 	#ifndef BARE_MODULES
	void create_params();
	static bool done_params;
 	#endif
};

#endif
