#ifndef MODPARAM_H
#define MODPARAM_H

#include "synthmodule.h"

class modparam
{
 public:
	modparam(synthmodnames::SYNTH_MOD_TYPE, paramnames::PAR_TYPE);
	~modparam(){};
	synthmodnames::SYNTH_MOD_TYPE get_moduletype(){ return (this) ? synthmodule_type : synthmodnames::MOD_FIRST; }
	paramnames::PAR_TYPE get_paramtype(){return (this) ? param_type: paramnames::PAR_FIRST; }
	IOCAT get_paramcategory(){return (this) ? synthmod::get_paramnames()->getcategory(param_type) : CAT_FIRST;}
 private:
	synthmodnames::SYNTH_MOD_TYPE synthmodule_type;
	paramnames::PAR_TYPE param_type;
};

#endif
