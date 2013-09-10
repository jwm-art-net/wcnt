#ifndef LFOCONTROLLER_H
#define LFOCONTROLLER_H

#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class lfo_controller : public synthmod 
{
 public:
	lfo_controller(string uname);
	~lfo_controller();
	void set_input_trig(const STATUS* ti){ in_trig = ti; }
	void set_input_wave(const double* wfi){ in_wave = wfi; }
	void set_input_amp_mod(const double* am){ in_amp_mod = am; }
	const STATUS* get_input_trig(){ return in_trig; }
	const double* get_input_wave(){ return in_wave; }
	const double* get_input_amp_mod(){ return in_amp_mod; }
	const double* get_output(){ return &output; }
	const double* get_output_preampmod(){ return &out_preampmod; }
	void set_delay_time(double dt){ delay_time = dt; }
	void set_ramp_time(double rt){ ramp_time = rt; }
	void set_start_level(double sl){ start_level = sl; }
	void set_end_level(double el){ end_level = el; }
	void set_amp_modsize(double ams);
	double get_delay_time(){ return delay_time; }
	double get_ramp_time(){ return ramp_time; }
	double get_start_level(){ return start_level; }
	double get_end_level(){ return end_level; }
	double get_amp_modsize(){ return amp_modsize; }
	// virtual funcs
	void run();
	void init(){};
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
		
 private:
	double out_preampmod;
	double output;
	const STATUS* in_trig;
	const double* in_wave;
	const double* in_amp_mod;
	double delay_time;
	double ramp_time;
	double amp_modsize;
	double ams_r;
	unsigned long delay_samples;
	unsigned long ramp_samples;
	double start_level;
	double end_level;
	double level_size;
	double current_level;
	static int lfo_controller_count;
	static void create_params();
	static bool done_params;
};

#endif
