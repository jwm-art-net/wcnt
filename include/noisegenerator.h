#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include <stdlib.h>
#include <time.h>
#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modinputslist.h"
#include "modoutputslist.h"
#include "modparamlist.h"
#endif

class noise_generator : public synthmod 
{
 public:
	noise_generator(string uname);
	~noise_generator();
	double const* get_output_signal();
	// virtual funcs
	void run();
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*){ return false;}
	bool set_param(paramnames::PAR_TYPE, void const*){ return false;}
	#endif
 private:
	double output;
	static int noise_generator_count;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
