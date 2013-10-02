#include "../include/constmod.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"

constmod::constmod(const char* uname) :
 synthmod(module::CONSTMOD, uname, SM_EMPTY_RUN | SM_HAS_OUT_OUTPUT),
 output(0)
{
    register_output(output::OUT_OUTPUT);
    init_first();
}

constmod::~constmod()
{
}

const void* constmod::get_out(output::TYPE ot) const
{
    switch(ot) 
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

bool constmod::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
        case param::VALUE:
            output = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* constmod::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        // parameter /is/ output.
        case param::VALUE: return &output;
        default: return 0;
    }
}



void constmod::init_first()
{
    if (done_first())
        return;
    register_param(param::VALUE);
}

