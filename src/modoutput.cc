#include "../include/modoutput.h"
#include "../include/globals.h"
#include "../include/synthmod.h"

modoutput::modoutput(synthmod::base* sm, output::TYPE ot) :
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
    return module->get_module_type();
}

output::TYPE modoutput::get_outputtype() const
{
    return output_type;
}

iocat::TYPE modoutput::get_outputcategory() const
{
    return output::names::category(output_type);
}

#ifdef DATA_STATS
STATS_INIT(modoutput)
#endif

