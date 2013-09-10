#ifndef ADSR_H
#include "../include/adsr.h"

adsr::adsr(string uname) :
	synthmod(synthmodnames::MOD_ADSR, adsr_count, uname),
	in_note_on_trig(0), in_note_off_trig(0), in_velocity(0), output(0),
	out_off_trig(OFF), play_state(OFF), up_thresh(0), lo_thresh(0), 
	start_level(0), sustain_status(OFF), zero_retrigger(OFF), thresh_range(0),
	env(0), sect(0), sectsample(0), sectmaxsamples(0), levelsize(0), 
	coord_item(0), coord(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_outputlist()->add_output(this, outputnames::OUT_OFF_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
	get_inputlist()->add_input(this, inputnames::IN_NOTE_ON_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_NOTE_OFF_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_VELOCITY);
	#endif
	env = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	// sustain section is a bit wierd here: but here it is: and must remain:
	// note: user cannot add a sustain section.  note: times and levels are 
	// zero because these are dynamic, according to stuff like.....my code.
	coord = new adsr_coord(adsr_coord::ADSR_SUSTAIN,0 ,0 ,0 ,0);
	env->add_at_head(coord);
    adsr_count++;
	#ifndef BARE_MODULES
	create_params();
	create_moddobj();
	#endif
}

adsr::~adsr()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
    if (env) {
		goto_first();
		while (coord) {
			delete coord;
			goto_next();
		}
		delete env;
	}
}

void adsr::init()
{
	if (up_thresh < lo_thresh) {
		double ut = up_thresh;
		up_thresh = lo_thresh;
		lo_thresh = ut;
	}
	thresh_range = up_thresh - lo_thresh;
}

#ifndef BARE_MODULES
void const *
adsr::get_out(outputnames::OUT_TYPE ot)
{
    void const *o = 0;
    switch (ot) {
    case outputnames::OUT_OUTPUT:
        o = &output;
        break;
	case outputnames::OUT_OFF_TRIG:
		o = &out_off_trig;
		break;
    case outputnames::OUT_PLAY_STATE:
        o = &play_state;
        break;
    default:
        o = 0;
    }
    return o;
}

void const *
adsr::set_in(inputnames::IN_TYPE it, void const *o)
{
    void const *i = 0;
    switch (it) {
    case inputnames::IN_VELOCITY:
        i = in_velocity = (double *) o;
        break;
    case inputnames::IN_NOTE_ON_TRIG:
        i = in_note_on_trig = (STATUS *) o;
        break;
    case inputnames::IN_NOTE_OFF_TRIG:
        i = in_note_off_trig = (STATUS *) o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool adsr::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_START_LEVEL:
			set_start_level(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_SUSTAIN_STATUS:
			set_sustain_status(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_ZERO_RETRIGGER:
			set_zero_retrigger_mode(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_UP_THRESH:
			set_upper_thresh(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_LO_THRESH:
			set_lower_thresh(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

bool adsr::validate()
{
	if (!goto_section(adsr_coord::ADSR_ATTACK)) {
		*err_msg = "\nadsr lacks attack section!";
		invalidate();
	}
	if (!goto_section(adsr_coord::ADSR_DECAY)) {
		*err_msg += "\nadsr lacks decay section!";
		invalidate();
	}
	if (!goto_section(adsr_coord::ADSR_RELEASE)) {
		*err_msg += "\nadsr lacks release section!";
		invalidate();
	}
	if (start_level < -1.0 || start_level > 1.0) {
		*err_msg += "\n" + 
			get_paramnames()->get_name(paramnames::PAR_START_LEVEL);
		*err_msg += " should be within -1.0 to +1.0";
		invalidate();
	}
	return is_valid();
}

dobj* adsr::add_dobj(dobj* dbj)
{
	dobj* retv = 0;
	dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
	switch(dbjtype)
	{
		case dobjnames::SIN_COORD:
			if (!(retv = insert_coord((adsr_coord*)dbj)))
				*err_msg="\ncould not section to " + *get_username();
			break;
		default:
			*err_msg = "\n***major error*** attempt made to add an ";
			*err_msg += "\ninvalid object type to " + *get_username();
			retv = 0;
	}
	return retv;
}

#endif

adsr_coord* adsr::insert_coord(adsr_coord* ac)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return 0; // don't let user try it on.
	if (ac->get_adsr_section() < goto_first()->get_adsr_section())
		return (adsr_coord*)(env->add_at_head(ac))->get_data();
    adsr_coord *tmp = 0;
	ll_item* lltmp = 0;
    while (coord) {
		lltmp = coord_item;
		tmp = coord;
        goto_next();
		if (coord) 
			if (coord->get_adsr_section() > ac->get_adsr_section())
				break;
    }
	return (adsr_coord*)(env->insert_after(lltmp, ac))->get_data();
}

adsr_coord* adsr::insert_coord(	adsr_coord::SECT adsrsect, 
								double ut, double ul, double lt, double ll)
{
    if (adsrsect == adsr_coord::ADSR_SUSTAIN)
        return 0; // don't let user try it on.
    goto_first();
	if (coord->get_adsr_section() > adsrsect)
		return (adsr_coord*)env->add_at_head(
			new adsr_coord(adsrsect, ut, ul, lt, ll))->get_data();
    while (coord) {
        if (coord->get_adsr_section() == adsrsect) {
			ll_item* tmp = coord_item;
			while(coord) {
				goto_next();
				if (coord) {
					if (coord->get_adsr_section() > adsrsect)
						return (adsr_coord*)env->insert_after(tmp, new 
							adsr_coord(adsrsect, ut, ul, lt, ll))->get_data();
					tmp = coord_item;
				}
			}
		}
        goto_next();
    }
	return (adsr_coord*)env->add_at_tail(
		new adsr_coord(adsrsect, ut, ul, lt, ll))->get_data();
}

adsr_coord * adsr::insert_coord_after(adsr_coord * ac)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return NULL;            /* wait for advanced_adsr implementation for multi-sectioned SUSTAIN sections! */
    ll_item* tmp = env->find_data(ac);
    if (tmp == NULL)
        return NULL;
    adsr_coord* nac = (adsr_coord *)tmp->get_next()->get_data();
    double nut = ac->get_upper_time();
    double nlt = ac->get_lower_time();
    double nul;
    double nll;
    if (nac == NULL) {          /* as there is no coord after ac, generate it's levels from the previous coord */
        nul = 0;
        nll = 0;
        adsr_coord* pac = (adsr_coord *)tmp->get_prev()->get_data();
        if (pac == NULL) {      /* ok fuck this, I'm outa here (paranoia?) */
            invalidate();
            return NULL;
        }
        ac->set_upper_level(pac->get_upper_level() / 2);
        ac->set_lower_level(pac->get_lower_level() / 2);
    } else {
        nul = (ac->get_upper_level() + nac->get_upper_level()) / 2;
        nll = (ac->get_lower_level() + nac->get_lower_level()) / 2;
    }
    adsr_coord* newcoord = new adsr_coord(ac->get_adsr_section(), nut, nul, nlt, nll);
	if (!env->insert_after(tmp, newcoord)->get_data()) {
		delete newcoord;
		return 0;
	}
	return newcoord;
}

adsr_coord *
adsr::insert_coord_after(adsr_coord * ac, double ut, double ul, double lt, double ll)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return NULL;
    ll_item *tmp = env->find_data(ac);
    if (tmp == NULL)
        return NULL;
    adsr_coord *newcoord = new adsr_coord(ac->get_adsr_section(), ut, ul, lt, ll);
    if (!env->insert_after(tmp, newcoord)->get_data()) {
		delete newcoord;
		return 0;
	}
	return newcoord;
}

void
adsr::delete_coord(adsr_coord * ac)
{
    ll_item *tmp = env->find_data(ac);
    if (tmp == NULL)
        return;
    adsr_coord *pac = (adsr_coord *) tmp->get_prev()->get_data();
    adsr_coord *nac = (adsr_coord *) tmp->get_next()->get_data();
    if (pac == NULL || nac == NULL)
        return;                 /* don't delete first or last coord */
    if (pac->get_adsr_section() == ac->get_adsr_section() || nac->get_adsr_section() == ac->get_adsr_section()) 
	{
        delete env->unlink_item(tmp);
        delete ac;
    }
    /* else ac is the only coord in section, so do not delete it */
    return;
}

adsr_coord *
adsr::goto_section(adsr_coord::SECT n)
{
    goto_first();
    while (coord) {
        if (coord->get_adsr_section() == n)
            return coord;
        goto_next();
    }
    return NULL;
}

void
adsr::scale_section(adsr_coord::SECT n, double ratio)
{
    if (ratio <= 0.00)
        return;
    if (n <= adsr_coord::ADSR_FIRST || n >= adsr_coord::ADSR_LAST)
        return;
    goto_first();
    while (coord->get_adsr_section() != n)
        goto_next();
    while (coord->get_adsr_section() == n) {
        coord->set_upper_time(coord->get_upper_time() * ratio);
        coord->set_lower_time(coord->get_lower_time() * ratio);
        if (!goto_next())
            return;
    }
}

void
adsr::run()
{
    if (*in_note_on_trig == ON) {
        play_state = ON;
        goto_first();
		double v = (*in_velocity - lo_thresh) / thresh_range;
		if (v <= 0.0) coord->run(0.0);
		else if (v >= up_thresh) coord->run(1.0);
		else coord->run(v);
        sectsample = 0;
        if (coord->output_time == 0) {
            sectmaxsamples = 1;
            levelsize = 0;
            output = coord->output_level;
        } 
		else {
            if (zero_retrigger == ON)
                output = start_level;
            sectmaxsamples = ms_to_samples(coord->output_time);
            levelsize = (coord->output_level - output) / sectmaxsamples;
        }
    }
    if (play_state == ON) {
        if (*in_note_off_trig == ON && sustain_status == ON) {
            goto_section(adsr_coord::ADSR_RELEASE);
			double v = (*in_velocity - lo_thresh) / thresh_range;
			if (v <= 0.0) coord->run(0.0);
			else if (v >= up_thresh) coord->run(1.0);
			else coord->run(v);
            sectsample = 0;
            if (coord->output_time == 0) {
                sectmaxsamples = 1;
                levelsize = 0;
                output = coord->output_level;
            } 
			else {
                sectmaxsamples = ms_to_samples(coord->output_time);
                levelsize = (coord->output_level - output) / sectmaxsamples;
            }
        }
        sectsample++;
        output += levelsize;
        if (sectsample >= sectmaxsamples) {
			end_level = coord->output_level;// grab it while I can, uhhm.
            goto_next();
            if (!coord) {
                output = end_level;
                play_state = OFF;
				out_off_trig = ON;
            } 
			else if (coord->get_adsr_section() == adsr_coord::ADSR_SUSTAIN
                       && sustain_status == ON) {
                sectmaxsamples = 4294967295ul;  
                levelsize = 0;
            }
			else {
                if (coord->get_adsr_section() == adsr_coord::ADSR_SUSTAIN
                    && sustain_status == OFF)
                    goto_section(adsr_coord::ADSR_RELEASE);
				double v = (*in_velocity - lo_thresh) / thresh_range;
				if (v <= 0.0) coord->run(0.0);
				else if (v >= up_thresh) coord->run(1.0);
				else coord->run(v);
                if (coord->output_time == 0) {
                    sectmaxsamples = 1;
                    levelsize = 0;
                    output = coord->output_level;
                }
				else {
                    sectmaxsamples = ms_to_samples(coord->output_time);
                    levelsize =
                        (coord->output_level - output) / sectmaxsamples;
                }
            }
            sectsample = 0;
        }
    }
	else if (out_off_trig == ON) out_off_trig = OFF;
}

short adsr::adsr_count = 0;

#ifndef BARE_MODULES
bool adsr::done_params = false;
void adsr::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->
		add_param(synthmodnames::MOD_ADSR, paramnames::PAR_UP_THRESH);
	get_paramlist()->
		add_param(synthmodnames::MOD_ADSR, paramnames::PAR_LO_THRESH);
	get_paramlist()->
		add_param(synthmodnames::MOD_ADSR, paramnames::PAR_START_LEVEL);
	get_paramlist()->
		add_param(synthmodnames::MOD_ADSR, paramnames::PAR_SUSTAIN_STATUS);
	get_paramlist()->
		add_param(synthmodnames::MOD_ADSR, paramnames::PAR_ZERO_RETRIGGER);
	done_params = true;
}

bool adsr::done_moddobj = false;
void adsr::create_moddobj()
{
	if (done_moddobj == true)
		return;
	get_moddobjlist()->add_moddobj(synthmodnames::MOD_ADSR, dobjnames::LIN_ENVELOPE);
	// also add a dobjdobj becuase there is only a dobj type and not a dobj.
	// if the envelope was a dobj, it would do this, but then it would
	// have to not be encapsulated by the adsr anymore.
	dobj::get_dobjdobjlist()->add_dobjdobj(dobjnames::LIN_ENVELOPE,	dobjnames::SIN_COORD);
	done_moddobj = true; 
}
#endif
#endif
