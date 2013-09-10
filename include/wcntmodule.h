#ifndef WCNTMODULE_H
#define WCNTMODULE_H

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

/* 
	wcnt_module
	
	this module is purely in existance for jwmsynth::execute() to
	grab a out_bar from a single instance of the time_map synthmod 
	class, and to know which bar to stop at.
	
	it does not serve any other purpose, which is why it does 
	absolutely nothing.
	
	because: some wierdo might create more than one time_map module
	in thier wcnt file, that same wierdo must also specify which one
	to use.  
	
	Discounted option: unless of course, jwmsynth searched through the 
	module list for time_map modules, and somehow worked out which one 
	would cause the longest execution. then of course it may be running
	longer than necessary.  I'd rather leave the responsibility on the 
	user to decide.
	
*/


class wcnt_module: public synthmod
{
 public:
	wcnt_module(string uname);
	~wcnt_module();
	// inputs
	void set_input_bar(const short* b){ in_bar = b;}
	const short* get_input_bar(){ return in_bar;}
	// no outputs
	// params
	void set_exit_bar(short eb){ exit_bar = eb;}
	short get_exit_bar() { return exit_bar; }
	// virtual funcs
	void run(){};
	bool validate();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE) { return 0; }
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	// inputs
 	const short* in_bar;
 	// no outputs
 	// params
 	short exit_bar;
	// synthmod stuff
	static short wcnt_module_count;
	void create_params();
	static bool done_params;
};

#endif // BARE_MODULES
#endif
