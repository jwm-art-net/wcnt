#ifndef NOISEGENERATOR_H
#include "../include/noisegenerator.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"

noise_generator::noise_generator(char const* uname) :
 synthmod(synthmodnames::NOISEGEN, uname, SM_HAS_OUT_OUTPUT),
 output(0.00)
{
    srandom(time(0));
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
}

noise_generator::~noise_generator()
{
}

void const* noise_generator::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

#endif
