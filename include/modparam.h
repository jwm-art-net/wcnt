#ifndef MODPARAM_H
#define MODPARAM_H

#include "synthmodnames.h"
#include "paramnames.h"
#include "stockerrs.h"

class synthmod;

class modparam
{
 public:
    modparam(synthmodnames::SYNTH_MOD_TYPE, paramnames::PAR_TYPE);
    ~modparam(){};
    synthmodnames::SYNTH_MOD_TYPE get_moduletype() {
        return (this) ? synthmodule_type : synthmodnames::FIRST;
    }
    paramnames::PAR_TYPE get_paramtype() {
        return (this) ? param_type: paramnames::FIRST;
    }
    iocat::IOCAT get_paramcategory();
    bool validate(synthmod* sm, stockerrs::ERR_TYPE et);

    bool operator()(synthmodnames::SYNTH_MOD_TYPE & smt) const {
        return synthmodule_type == smt;
    }

    bool operator()(paramnames::PAR_TYPE & pt) const {
        return param_type == pt;
    }

 private:
    synthmodnames::SYNTH_MOD_TYPE synthmodule_type;
    paramnames::PAR_TYPE param_type;
};

#endif
