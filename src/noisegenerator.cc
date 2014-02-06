#include "../include/noisegenerator.h"
#include "../include/globals.h"
#include "../include/modoutputlist.h"

noise_generator::noise_generator(const char* uname) :
 synthmod::base(synthmod::NOISEGEN, uname, SM_HAS_OUT_OUTPUT),
 output(0.00)
{
    srandom(time(0));
    register_output(output::OUT_OUTPUT);
}

noise_generator::~noise_generator()
{
}

const void* noise_generator::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

