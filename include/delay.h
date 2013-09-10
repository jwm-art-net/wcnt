#ifndef DELAY_H
#define DELAY_H

#include <math.h>
#include "dtr.h"
#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class delay : public synthmod 
{
 public:
	delay(string uname);
	~delay();
	void set_input_signal(const double* in){ in_signal = in;}
	void set_input_gain_mod(const double* gm){ in_gainmod = gm;}
	const double* get_input_signal(){ return in_signal;}
	const double* get_input_gain_mod(){ return in_gainmod;}
	const double* get_output(){ return &output;}
	void set_delay_time(double dt){ delay_time = dt;}
	void set_gain_level(double gl){ gain = gl;}
	void set_gain_modsize(double rms){ gain_modsize = rms;}
	void set_wetdry(double wd){ wetdry = wd;}
	double get_delay_time(){ return delay_time;}
	double get_gain_level(){ return gain; }
	double get_gain_modsize(){ return gain_modsize; }
	double get_wetdry(){ return wetdry;}
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
	const double* in_signal;
	const double* in_gainmod;
	// outputs
	double out_output;
	// params
 	double delay_time;
	double gain;
	double gain_modsize;
 	double wetdry;
 	// working
 	double output;
	double* filter;//array
	long filterarraymax;
	long fpos;
	double filtertotal;
 	double gainamount;
	static int delay_count;
	#ifndef BARE_MODULES
	void create_params();
	static bool done_params;
 	#endif
};

#endif
