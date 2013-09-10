#ifndef DC_FILTER_H
#define DC_FILTER_H

#include <math.h>
#include "dtr.h"
#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

#define FILTERARRAYSIZE 8192


// I have wanted one of these for quite a long while.. had a look at
// musicdsp.org etc and found some code snippets and mathamatical
// formalae which I did n't really understand, and then it occurred
// to me how to do it and this is the result, fairly straight forward
// actually.....

// copied existing filter classes and just subtract original signal
// from the average of the sum of x amount of samples as specified
// by dc_time.

// obviously it's not perfect method, but it does remove majority
// of dc, but some waveforms get inverted etc, but it does not
// seem to add artifacts that i have noticed....

class dc_filter : public synthmod 
{
 public:
	dc_filter(string uname);
	~dc_filter();
	void set_input_signal(const double* in){ in_signal = in;}
	const double* get_input_signal(){ return in_signal;}
	void set_dc_time(double dct){ dc_time = dct;}
	double get_dc_time(){ return dc_time;}
	const double* get_output(){ return &output;}
	// virtual funcs
	void run();
	void init();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif	
 private:
	// inputs
	const double* in_signal;
 	// output
	double output;
 	// params
	double dc_time;
 	// working
	double* filter;//array
	short filterarraymax;
	short fpos;
	double filtertotal;
	static int dc_filter_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
