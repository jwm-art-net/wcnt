#include "../include/modoutput.h"
#include "../include/globals.h"
#include "../include/synthmod.h"

modoutput::modoutput(synthmod::base* sm, int ot) :
 module(sm), output_type(ot)
{
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

modoutput::~modoutput()
{
    #ifdef DATA_STATS
    STATS_DEC
    #endif
}

synthmod::TYPE modoutput::get_moduletype() const
{
    return (this != 0) ? module->get_module_type()
                       : synthmod::ERR_TYPE;
}

int modoutput::get_outputtype() const
{
    return (this != 0) ? output_type
                       : output::ERR_TYPE;
}

iocat::TYPE modoutput::get_outputcategory() const
{
    return (this != 0) ? output::names::category(output_type)
                       : iocat::ERR_TYPE;
}

#ifdef DATA_STATS
STATS_INIT(modoutput)
#endif

