#include "../include/modoutput.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"

modoutput::modoutput(synthmod * sm, output::TYPE ot) :
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

module::TYPE modoutput::get_moduletype() const
{
    return (this != 0) ? module->get_module_type()
                       : module::ERR_TYPE;
}

output::TYPE modoutput::get_outputtype() const
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

