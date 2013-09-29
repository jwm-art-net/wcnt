#ifndef MODPARAM_H
#define MODPARAM_H

#include "paramnames.h"
#include "stockerrs.h"
#include "synthmodnames.h"
#include "types.h"

class synthmod;

class modparam
{
 public:
    modparam(module::TYPE, param::TYPE);
    ~modparam();

    module::TYPE get_moduletype() const
        { return (this) ? synthmodule_type : module::ERR_TYPE; }
    param::TYPE get_paramtype() const
        { return (this) ? param_type: param::ERR_TYPE; }
    #ifdef UNUSED
    iocat::TYPE get_paramcategory();
    #endif
    bool validate(synthmod* sm, stockerrs::ERR_TYPE et);

    bool operator()(module::TYPE & smt) const
        { return synthmodule_type == smt; }

    bool operator()(param::TYPE & pt) const
        { return param_type == pt; }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    module::TYPE synthmodule_type;
    param::TYPE  param_type;

    #ifdef DATA_STATS
    STATS_VARS
    #endif
};

#endif
