#ifndef NONEZERO_H
#include "../include/nonezero.h"

nonezero::nonezero(string uname)
: synthmod(synthmodnames::MOD_NONEZERO, 0, uname),
out_none_double(0.00), out_none_short(0), out_none_ulong(0), out_none_STATUS(OFF)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_NONE_DOUBLE)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NONE_SHORT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NONE_ULONG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NONE_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NONE_STATE)){
		invalidate();
		return;
	}
	validate();
}

nonezero::~nonezero()
{
	get_outputlist()->delete_module_outputs(this);
}

void const* nonezero::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_NONE_DOUBLE:
			o = &out_none_double;
			break;
		case outputnames::OUT_NONE_SHORT:
			o = &out_none_short;
			break;
		case outputnames::OUT_NONE_ULONG:
			o = &out_none_ulong;
			break;
		case outputnames::OUT_NONE_TRIG:
			o = &out_none_STATUS;
			break;
		case outputnames::OUT_NONE_STATE:
			o = &out_none_STATUS;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* nonezero::set_in(inputnames::IN_TYPE it, void const* o)
{				//															                                                     | |
	return 0; // no inputs mo finkuts go splankyts to fukwhitz -i fink (this == sea larky) then is gosub(getting to me [:/<-<
}				//															                                                     | |


#endif
