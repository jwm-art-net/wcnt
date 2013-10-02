#include "../include/modinput.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"

modinput::modinput(synthmod * sm, input::TYPE it) :
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

module::TYPE modinput::get_moduletype() const
{
    return (this != 0)
     ? synthmodule->get_module_type()
     : module::ERR_TYPE;
}

input::TYPE modinput::get_inputtype() const
{
    return (this != 0)
     ? input_type
     : input::ERR_TYPE;
}

#ifdef UNUSED
iocat::TYPE modinput::get_inputcategory() const
{
    return (this != 0)
     ? inputnames::get_category(input_type)
     : iocat::FIRST;
}
#endif

#ifdef DATA_STATS
STATS_INIT(modinput)
#endif

