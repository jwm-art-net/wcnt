#include "../include/modoutput.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"

modoutput::modoutput(synthmod * sm, outputnames::OUT_TYPE ot) :
 synthmodule(sm), output_type(ot)
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

synthmodnames::SYNTH_MOD_TYPE modoutput::get_moduletype() const
{
    return (this != 0)
     ? synthmodule->get_module_type()
     : synthmodnames::FIRST;
}

outputnames::OUT_TYPE modoutput::get_outputtype() const
{
    return (this != 0)
     ? output_type
     : outputnames::OUT_FIRST;
}

iocat::IOCAT modoutput::get_outputcategory() const
{
    return (this != 0)
     ? outputnames::get_category(output_type)
     
     : iocat::FIRST;
}

#ifdef DATA_STATS
STATS_INIT(modoutput)
#endif

