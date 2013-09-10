#ifndef SPREADER_H
#include "../include/spreader.h"

spreader::spreader(string uname) :
	synthmod(synthmodnames::MOD_SPREADER, spreader_count, uname),
	in_mod(0), out_output(0), start_level(0), end_level(0),
	level_dif(0), sig_count(0), wcntsiglist(0), wcntsig_item(0), 
	wcntsig(0), sig(0), prevsig(0), zero(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_MODULATION);
	#endif
	wcntsiglist = 
		new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
	spreader_count++;
	#ifndef BARE_MODULES
	create_params();
	create_moddobj();
	#endif
	
}

spreader::~spreader()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	goto_first();
	while (wcntsig_item) {
		delete wcntsig;
		goto_next();
	}
	delete wcntsiglist;
}

#ifndef BARE_MODULES
void const* spreader::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &out_output;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* spreader::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_MODULATION:
			i = in_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool spreader::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_START_LEVEL:
			set_start_level(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_END_LEVEL:
			set_end_level(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

bool spreader::validate()
{
	if (!goto_first()) {
		*err_msg = "\nthere must be at least two signals to switch between";
		invalidate();
	} 
	else if (!goto_next()){
		*err_msg = "\nthere must be at least two signals to switch between";
		invalidate();
	}
	else {
		goto_first();
		while(wcntsig) {
			if (wcntsig->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
				*err_msg += "\nmodule ";
				*err_msg += *(wcntsig->get_username());
				*err_msg += " is not a wcnt_signal";
				invalidate();
			}
			goto_next();
		}
	}
	return is_valid();
}

dobj* spreader::add_dobj(dobj* dbj)
{
	if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
		synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
		if (sm->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
			*err_msg = "\n" + *sm->get_username() + 
				" is not a wcnt_signal";
			return 0;
		}
		if (!add_signal((wcnt_signal*)sm)) {
			*err_msg = "\ncould not insert " + *sm->get_username() +
				" into spreader";
			return 0;
		}
		// add the dobj synthmod wrapper to the dobjlist 
		// so it gets deleted in the end.
		dobj::get_dobjlist()->add_dobj(dbj);
		return dbj;
	}
	*err_msg = "\n***major error*** attempt made to add an ";
	*err_msg += "\ninvalid object type to " + *get_username();
	return 0;
}

#endif // BARE_MODULES

void spreader::init()
{
	goto_first();
	sig_count = 0;
	while (wcntsig) {
		goto_next();
		if (wcntsig) 
			sig_count++;
	}
	level_dif = end_level - start_level;
	seg_lvl = level_dif / sig_count;
}

void spreader::run() 
{
	out_output = 0;
	short sig_no = 0;
	double im_rl = sqrt(*in_mod * *in_mod);
	if (im_rl < start_level) im_rl = start_level;
	else if (im_rl > end_level) im_rl = end_level;
	goto_first();
	while (wcntsig) {
		double lvl = sig_no * seg_lvl;
		double vol = 0;
		if (im_rl >= lvl && im_rl <= lvl + seg_lvl)
			vol = (seg_lvl - (im_rl - lvl)) / seg_lvl;
		if (im_rl + seg_lvl >= lvl && im_rl + seg_lvl < lvl + seg_lvl)
			vol = (im_rl + seg_lvl - lvl) / seg_lvl;
		out_output += *(wcntsig->get_output()) * vol;
		sig_no++;
		goto_next();
	}
}

int spreader::spreader_count = 0;

#ifndef BARE_MODULES
bool spreader::done_params = false;

void spreader::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SPREADER, paramnames::PAR_START_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_SPREADER, paramnames::PAR_END_LEVEL);
	done_params = true;
}

bool spreader::done_moddobj = false;

void spreader::create_moddobj()
{
	if (done_moddobj == true)
		return;
	get_moddobjlist()->
		add_moddobj(synthmodnames::MOD_SPREADER, dobjnames::LIN_SIGNALS);
	// several modules use LIN_SIGNALS, if they all add a dobjdobj to
	// it, then wcnt will want to read as many as has been added - in one go
	// , ie it will expect several sets instead of several items.
	// glamorous eh?
	if (!dobj::get_dobjdobjlist()->
			get_dobjdobjlist_for_dobjtype(dobjnames::LIN_SIGNALS)->
				goto_first()) 
	{
		dobj::get_dobjdobjlist()->
			add_dobjdobj(dobjnames::LIN_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
	}	
	done_moddobj = true; 
}

#endif
#endif
