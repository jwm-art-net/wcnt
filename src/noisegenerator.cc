#ifndef NOISEGENERATOR_H
#include "../include/noisegenerator.h"

noise_generator::noise_generator(string uname)
: synthmod(synthmodnames::MOD_NOISEGEN, noise_generator_count, uname),
  output(0.00)
{
	srandom(time(0)); //srand(time(0));
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT))
	{
		invalidate();
		return;
	}
	noise_generator_count++;
	validate();
	create_params();
}

noise_generator::~noise_generator()
{
	get_outputlist()->delete_module_outputs(this);
}

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

void noise_generator::run() 
{
	output = (float) rand() / (RAND_MAX / 2) - 1;
}

int noise_generator::noise_generator_count = 0;
bool noise_generator::done_params = false;

void noise_generator::create_params()
{
	done_params = true;
	return;
}

#endif
