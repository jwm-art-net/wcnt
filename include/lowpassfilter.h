#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <math.h>
#include "dtr.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

#define FILTERARRAYSIZE 4096

class lpfilter : public synthmod 
{
 public:
	lpfilter(string uname);
	~lpfilter();
	void set_input_signal(const double* in){ in_signal = in;}
	void set_input_cutoff_deg_size(const double* ds){ in_deg_size = ds;}
	void set_input_resonance_amp_mod(const double* rm){ in_resampmod = rm;}
	const double* get_input_signal(){ return in_signal;}
	const double* get_input_deg_size(){ return in_deg_size;}
	const double* get_input_resonance_mod(){ return in_resampmod;}
	const double* get_output(){ return &output;}
	void set_resonance_level(double rl){ resamount = rl;}
	void set_resonant_amp_modsize(double rms){ resamp_modsize = rms;}
	double get_resonance_level(){ return resamount; }
	double get_resonant_amp_mod1size(){ return resamp_modsize; }
	// virtual funcs
	void run();
	void init(){};
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
		
 private:
	double output;
	const double* in_signal;
	const double* in_deg_size; //cutoff freq deg step size
	const double* in_resampmod;
	char k;
	double* filter;//array
	short filterarraymax;
	short fpos;
	short fp;
	double filtertotal;
	double resdeg;
	double resdegstsz;
	double resamount;
	double resamp_modsize;
	static int lpfilter_count;
	static void create_params();
	static bool done_params;
};

#endif
