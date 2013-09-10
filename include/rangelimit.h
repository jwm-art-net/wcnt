#ifndef RANGELIMIT_H
#define RANGELIMIT_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class range_limit : public synthmod 
{
 public:
	range_limit(string uname);
	~range_limit();
 	// input
	void set_input_signal(const double* in){ in_signal = in; }
	const double* get_input_signal(){ return in_signal; }
	// output
	const double* get_output(){ return &out_output; }
	// params
	void set_signal_range_hi(double srhi){ sigrangehi = srhi; }
	void set_signal_range_lo(double srlo){ sigrangelo = srlo; }
	double get_signal_range_hi(){ return sigrangehi; }
	double get_signal_range_lo(){ return sigrangelo; }
	// virtual funcs
	void run();
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif	
 private:
	// inputs
	const double* in_signal;
	// outputs
	double out_output;
	// params
 	double sigrangehi;
 	double sigrangelo;
	static int range_limit_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};
#endif
