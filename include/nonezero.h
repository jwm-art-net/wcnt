#ifndef NONEZERO_H
#define NONEZERO_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#endif
/*
	There should be only one..........'cos there's no point 'aving two or more.
	
	the plan is that it should not be an option for the user to create them, 
	infact the user should not be aware that there is such a module.  I've 
	designed it as a module so that when the user selects an output to plugin,
	there is the option to have none, or zero, same difference.
	-- actually as far as the user is concerned the input is 'off'
	
	when I programmed in TURBO_C++_v3.00 I found no problem with using NULL 
	instead	of a pointer to a var whose value was 0 - zero. Using GNU C++ v2.95 
	using NULL creates seg faults.  But I was vaguely aware that I needed a 
	better solution so created the nonezero module.......
*/

class nonezero: public synthmod 
{
 public:
	nonezero(string uname);
	~nonezero();
	const double* get_output_none_double(){ return &out_none_double; }
	const short* get_output_none_short(){ return &out_none_short; }
	const unsigned long* get_output_none_ulong(){ return &out_none_ulong; }
	const STATUS* get_output_none_trig(){ return &out_none_STATUS; }
	const STATUS* get_output_none_state(){ return &out_none_STATUS; }
	// virtual funcs
	void run(){};
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const* data){ return false;}
	#endif
 private:
	const double out_none_double;
	const short out_none_short;
	const unsigned long out_none_ulong;
	const STATUS out_none_STATUS;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
