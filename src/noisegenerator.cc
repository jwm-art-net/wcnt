#ifndef NOISEGENERATOR_H
#include "../include/noisegenerator.h"

noise_generator::noise_generator(char const* uname) :
 synthmod(synthmodnames::MOD_NOISEGEN, noise_generator_count, uname),
 output(0.00)
{
    srandom(time(0)); //srand(time(0));
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    noise_generator_count++;
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

int noise_generator::noise_generator_count = 0;

#endif
