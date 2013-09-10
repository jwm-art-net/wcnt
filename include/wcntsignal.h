#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "paramnames.h"
#endif

class wcnt_signal : public synthmod
{
 public:
	wcnt_signal(string uname);
	~wcnt_signal();
	void set_input_signal(const double* is){ io_signal = is;}
	const double* get_input_signal(){ return io_signal;}
	const double* get_output(){ return io_signal;}
	// virtual funcs
	void run(){}; 	// check inputs/outputs for explanation of emptiness.....
	void init(){}; 	// or notice that get_input/get_output return same thing.
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	const double* io_signal;
	static int wcnt_signal_count;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
	#endif
};

#endif
