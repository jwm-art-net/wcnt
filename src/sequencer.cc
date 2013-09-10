#ifndef SEQUENCER_H
#include "../include/sequencer.h"

sequencer::sequencer(string uname) : 
	synthmod(synthmodnames::MOD_SEQUENCER, sequencer_count, uname),
 	in_bar_trig(0), in_bar(0), in_pos_step_size(0), out_note_on_trig(OFF),
	out_note_slide_trig(OFF), out_note_off_trig(OFF), out_riff_start_trig(OFF),
 	out_riff_end_trig(OFF), out_end_trig(OFF), out_notename(0), out_freq(0),
	out_velocity(0), out_velocity_ramp(0), riff_play_state(OFF), 
	note_play_state(OFF), vel_response(0), hold_notename(OFF),
	riffnodelist(0), riff_node_ptr(0), riff_ptr(0), riffnodeitem(0),
	note_ptr(0), noteitem(0), zero(0), riff_start_bar(0), barpos(0),
	note_on_pos(0), note_off_pos(0), posconv(0), velrsp_max_samps(0),
	velrsp_samp(0), vel_stpsize(0)
{
	#ifndef BARE_MODULES
	get_inputlist()->add_input(this, inputnames::IN_BAR);
	get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_POS_STEP_SIZE);
	get_outputlist()->add_output(this, outputnames::OUT_NOTE_ON_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_NOTE_SLIDE_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_NOTE_OFF_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_RIFF_START_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_RIFF_END_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_END_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_NOTENAME);
	get_outputlist()->add_output(this, outputnames::OUT_FREQ);
	get_outputlist()->add_output(this, outputnames::OUT_VELOCITY);
	get_outputlist()->add_output(this, outputnames::OUT_VELOCITY_RAMP);
	get_outputlist()->add_output(this, outputnames::OUT_RIFF_PLAY_STATE);
	get_outputlist()->add_output(this, outputnames::OUT_NOTE_PLAY_STATE);
	#endif
	riffnodelist = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	sequencer_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

sequencer::~sequencer() 
{
	ll_item* tmp = riffnodelist->goto_first();
	while (tmp != NULL)
	{
		delete (riff_node*) tmp->get_data();
		tmp = riffnodelist->goto_next();
	}
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	#endif
}

void sequencer::init()
{ 
	velrsp_max_samps = convert_ms_to_samples(vel_response);
	riff_node_ptr = (riff_node*)
		(riffnodeitem = riffnodelist->goto_first())->get_data();
	riff_start_bar = riff_node_ptr->get_start_bar();
//  this no longer needed as user specs 1/4 val in each riff
//	posconv = (short)(timemap::QUARTER_VALUE / timemap::USER_QUARTER_VALUE);
}									  

#ifndef BARE_MODULES
void const* sequencer::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_NOTE_ON_TRIG:
			o = &out_note_on_trig;
			break;
		case outputnames::OUT_NOTE_SLIDE_TRIG:
			o = &out_note_slide_trig;
			break;
		case outputnames::OUT_NOTE_OFF_TRIG:
			o = &out_note_off_trig;
			break;
		case outputnames::OUT_RIFF_START_TRIG:
			o = &out_riff_start_trig;
			break;
		case outputnames::OUT_RIFF_END_TRIG:
			o = &out_riff_end_trig;
			break;
		case outputnames::OUT_END_TRIG:
			o = &out_end_trig;
			break;
		case outputnames::OUT_FREQ:
			o = &out_freq;
			break;
		case outputnames::OUT_VELOCITY:
			o = &out_velocity;
			break;
		case outputnames::OUT_VELOCITY_RAMP:
			o = &out_velocity_ramp;
			break;
		case outputnames::OUT_RIFF_PLAY_STATE:
			o = &riff_play_state;
			break;
		case outputnames::OUT_NOTE_PLAY_STATE:
			o = &note_play_state;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* sequencer::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_BAR_TRIG:
			i = in_bar_trig = (STATUS*)o;
			break;
		case inputnames::IN_BAR:
			i = in_bar = (short*)o;
			break;
		case inputnames::IN_POS_STEP_SIZE:
			i = in_pos_step_size = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool sequencer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_VELOCITY_RESPONSE:
			set_velocity_response_time(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_HOLD_NOTENAME:
			set_hold_notename(*(STATUS*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

riff_node* sequencer::add_riff(string const* riffname, short barpos)
{
	riffdata* rd = rifflist->get_riff_by_name(riffname);
	if (!rd)
		return 0;
	return add_riff(rd, barpos);
}

#endif // BARE_MODULES

riff_node* sequencer::add_riff(riffdata* rd, short barpos)
{
	if (rd == NULL) 
		return NULL;
	riff_node* newriffnode = new riff_node(rd, barpos);
	if (lookup_data_match(riffnodelist, newriffnode, &riff_node::get_start_bar))
	{ // cannot have more than one riff at same bar position.
		delete newriffnode;
		return 0;
	}
	riff_node* tmp = ordered_insert(riffnodelist, newriffnode, &riff_node::get_start_bar);
	if (tmp == NULL)
	{
		delete newriffnode;
		return NULL;
	}
	return tmp;
}

bool sequencer::delete_riff_node(riff_node* rn)
{
	if (!rn)
		return false;
	ll_item* tmp = riffnodelist->find_data(rn);
	if (!tmp)
		return false;
	delete rn;
	delete riffnodelist->unlink_item(tmp);
	return true;
}

void sequencer::run() 
{
	if (*in_bar_trig == ON) {
		if (*in_bar == riff_start_bar) {
			riff_play_state = ON;
			out_riff_start_trig = ON;
			riff_ptr = riff_node_ptr->get_riff_source();
			posconv = (double)timemap::QUARTER_VALUE 
				/ riff_ptr->get_quartervalue();
			note_ptr = (note_data*)
				(noteitem = riff_ptr->sneak_first())->get_data();
			note_on_pos = (long)(note_ptr->get_position() * posconv);
			// note from last riff could still be playing
			if (note_play_state == ON) 
				note_off_pos -= (long)barpos;
			barpos = 0; // noteoff pos will now be relative to new bar
			// setup riff_node & riff_start_bar for next riff
			riff_node_ptr = (riff_node*)
				(riffnodeitem = riffnodelist->goto_next())->get_data();
			riff_start_bar = riff_node_ptr->get_start_bar();
		}
	} else if (out_riff_start_trig == ON) out_riff_start_trig = OFF;
	if (riff_play_state == ON) {
		if ((long)barpos == note_on_pos) {
			if (note_play_state == OFF) {
				note_play_state = ON;
				out_note_on_trig = ON;
			}
			else out_note_slide_trig = ON;
			out_notename = note_ptr->get_name();
			out_freq = note_ptr->get_frequency();
			out_velocity = note_ptr->get_velocity();
			if (velrsp_max_samps > 0) {
				vel_stpsize = (double) 
					(out_velocity - out_velocity_ramp) / velrsp_max_samps;
				velrsp_samp = velrsp_max_samps;
			} else out_velocity_ramp = out_velocity;
			note_off_pos = (long)
				(note_on_pos + note_ptr->get_length() * posconv);
			note_ptr = (note_data*) 
				(noteitem = noteitem->get_next())->get_data();
			if (!note_ptr) note_on_pos = -1;
			else note_on_pos = (long)(note_ptr->get_position() * posconv);
		} // endif ((short)barpos == note_on_pos)
		else {
			if (out_note_on_trig == ON) out_note_on_trig = OFF;
			if (out_note_slide_trig == ON) out_note_slide_trig = OFF;
		}
		if (note_play_state == ON) {
			if ((long)barpos == note_off_pos) {
				note_play_state = OFF;
				out_note_off_trig = ON;
				if (!note_ptr) riff_play_state = OFF;
				else note_off_pos = (long)
						(note_on_pos + note_ptr->get_length() * posconv);
				if (hold_notename == OFF) out_notename = &zero;
			}
		} 
		else if (out_note_off_trig == ON) out_note_off_trig = OFF;
		if (velrsp_samp > 0) {
			out_velocity_ramp += vel_stpsize;
			velrsp_samp--;
		}
		barpos += *in_pos_step_size;
	} // end if (riff play state == ON)
	else if (out_note_off_trig == ON) out_note_off_trig = OFF;
}

int sequencer::sequencer_count = 0;

#ifndef BARE_MODULES
riff_list* sequencer::rifflist = 0;
bool sequencer::done_params = false;

void sequencer::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SEQUENCER, paramnames::PAR_VELOCITY_RESPONSE);
	get_paramlist()->add_param(synthmodnames::MOD_SEQUENCER, paramnames::PAR_HOLD_NOTENAME);
	done_params = true;
}
#endif
#endif
