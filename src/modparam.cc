#ifndef MODPARAM_H
#include "../include/modparam.h"

#ifndef BARE_MODULES

modparam::modparam(synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt)
: synthmodule_type(smt), param_type(pt)
{
}

#endif
#endif
