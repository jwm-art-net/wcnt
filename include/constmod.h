#ifndef CONSTMOD_H
#define CONSTMOD_H

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modparamlist.h"
#endif

class constmod : public synthmod 
{
 public:
	constmod(string uname);
	~constmod();
	
	void set_value(double v){output = v;}
	double get_value(){ return output;}
	const double* get_output(){ return &output;}
	// virtual funcs
	void run(){};
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE it, void const* o){ return 0;}
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif	
 private:
	double output;
	static int constmod_count;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
