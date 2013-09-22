#include "../include/modinput.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"

modinput::modinput(synthmod * sm, inputnames::IN_TYPE it) :
 synthmodule(sm), input_type(it)
{
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

modinput::~modinput()
{
    #ifdef DATA_STATS
    STATS_DEC
    #endif
}

synthmodnames::SYNTH_MOD_TYPE modinput::get_moduletype() const
{
    return (this != 0)
     ? synthmodule->get_module_type()
     : synthmodnames::FIRST;
}

inputnames::IN_TYPE modinput::get_inputtype() const
{
    return (this != 0)
     ? input_type
     : inputnames::IN_FIRST;
}

#ifdef UNUSED
iocat::IOCAT modinput::get_inputcategory() const
{
    return (this != 0)
     ? inputnames::get_category(input_type)
     : iocat::FIRST;
}
#endif

#ifdef DATA_STATS
STATS_INIT(modinput)
#endif

