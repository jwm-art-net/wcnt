#include "../include/noisegenerator.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"

noise_generator::noise_generator(const char* uname) :
 synthmod(module::NOISEGEN, uname, SM_HAS_OUT_OUTPUT),
 output(0.00)
{
    srandom(time(0));
    register_output(output::OUT_OUTPUT);
}

noise_generator::~noise_generator()
{
}

const void* noise_generator::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

