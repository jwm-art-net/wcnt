#ifndef NOISEGENERATOR_H
#include "../include/noisegenerator.h"

noise_generator::noise_generator(string uname)
: synthmod(synthmodnames::MOD_NOISEGEN, noise_generator_count, uname),
  output(0.00)
{
	srandom(time(0)); //srand(time(0));
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	#endif
	noise_generator_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

noise_generator::~noise_generator()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* noise_generator::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		default:
			o = 0;
	}
	return o;
}
#endif

void noise_generator::run() 
{
	output = (float) rand() / (RAND_MAX / 2) - 1;
}

int noise_generator::noise_generator_count = 0;

#ifndef BARE_MODULES
bool noise_generator::done_params = false;

void noise_generator::create_params()
{
	done_params = true;
	return;
}
#endif
#endif
