#ifndef SEQUENCER_H
#include "../include/sequencer.h"

riff_node::riff_node(riffdata* rd, short barpos) 
: start_bar(barpos), riff_source(rd)
{
}

riff_node::~riff_node() 
{
}

sequencer::sequencer(string uname)
: synthmod(synthmodnames::MOD_SEQUENCER, sequencer_count, uname),
out_note_on_trig(OFF), out_note_slide_trig(OFF), out_note_off_trig(OFF), out_riff_start_trig(OFF),
out_riff_end_trig(OFF), out_start_trig(OFF), out_end_trig(OFF), out_freq(0), out_velocity(0),
out_total_sample_pos(0), play_state(OFF), riff_play_state(OFF), note_play_state(OFF),
riffnodelist(0), riff_node_ptr(NULL), riff_ptr(NULL), riffnodeitem(NULL), note_ptr(NULL), noteitem(NULL),
note_on_sample_pos(0), note_off_sample_pos(0), next_note_sample_pos(0), riff_sample_pos(0),
riff_start_sample(0), riff_end_sample(0), next_riff_start_sample(0), 
vel_response(0), velrsp_max_samps(0), velrsp_samp(0), vel_stpsize(0)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_NOTE_ON_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NOTE_SLIDE_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NOTE_OFF_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_RIFF_START_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_RIFF_END_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_START_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_END_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_FREQ)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_VELOCITY)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_TOTAL_SAMPLE_POS)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_RIFF_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_NOTE_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!(riffnodelist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA))){
		invalidate();
		return;
	}
	sequencer_count++;
	validate();
	create_params();
}

sequencer::~sequencer() 
{
	ll_item* tmp = riffnodelist->goto_first();
	while (tmp != NULL)
	{
		delete (riff_node*) tmp->get_data();
		tmp = riffnodelist->goto_next();
	}
	get_outputlist()->delete_module_outputs(this);
}

void sequencer::init()
{ 
	velrsp_max_samps = convert_ms_to_samples(vel_response);
}									  


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
		case outputnames::OUT_START_TRIG:
			o = &out_start_trig;
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
		case outputnames::OUT_TOTAL_SAMPLE_POS:
			o = &out_total_sample_pos;
			break;
		case outputnames::OUT_PLAY_STATE:
			o = &play_state;
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

bool sequencer::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_VELOCITY_RESPONSE:
			set_velocity_response_time(*(double*)data);
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

riff_node* sequencer::add_riff(riffdata* rd, short barpos)
{
	if (rd == NULL) 
		return NULL;
	riff_node* newriffnode = new riff_node(rd, barpos);
	if (newriffnode == NULL)
		return NULL;
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

void sequencer::calc_riff_sample_positions() 
{
	if (riff_node_ptr == NULL) 
	{
		riff_start_sample = 4294967295ul;
		riff_end_sample = 4294967295ul;
		next_riff_start_sample = 4294967295ul;
		out_end_trig = ON;
//		play_state = OFF;
		riff_ptr = NULL;
	} 
	else 
	{
		riff_start_sample = convert_notelen_to_samples(riff_node_ptr->get_start_bar() * beats_per_measure * (256 / beat_value));
		riff_end_sample = riff_start_sample + convert_notelen_to_samples(riff_node_ptr->get_riff_source()->getrifflength());
		riff_node* next_riff_node = (riff_node*)riffnodeitem->get_next()->get_data();
		if (next_riff_node == NULL)
			next_riff_start_sample = 4294967295ul;
		else 
			next_riff_start_sample = convert_notelen_to_samples(next_riff_node->get_start_bar() * beats_per_measure * (256 / beat_value));
		riff_ptr = riff_node_ptr->get_riff_source();
	}
}

void sequencer::calc_note_sample_positions() 
{
	if (note_ptr == NULL) 
	{
		note_on_sample_pos = 4294967295ul;
		note_off_sample_pos = 4294967295ul;
		next_note_sample_pos = 4294967295ul;
	} 
	else 
	{
		note_on_sample_pos = convert_notelen_to_samples(note_ptr->get_position());
		note_off_sample_pos = note_on_sample_pos + convert_notelen_to_samples(note_ptr->get_length());
		note_data* next_note = (note_data*)noteitem->get_next()->get_data();
		if (next_note == NULL)
			next_note_sample_pos = 4294967295ul;
		else 
			next_note_sample_pos =	convert_notelen_to_samples(next_note->get_position());
	}
}

void sequencer::run() 
{
	if (play_state == OFF) {
		out_total_sample_pos = 0;
		riff_node_ptr = (riff_node*)(riffnodeitem = riffnodelist->goto_first())->get_data();
		calc_riff_sample_positions();
		out_start_trig = ON;
		note_on_sample_pos = 4294967295ul;
		note_off_sample_pos = 4294967295ul;
		next_note_sample_pos = 4294967295ul;
		play_state = ON;
	}else if (out_start_trig == ON) out_start_trig = OFF;
	if (out_total_sample_pos == riff_start_sample) {
		riff_sample_pos = 0;
		note_ptr = (note_data*)(noteitem = riff_ptr->sneak_first())->get_data(); // use sneak not goto, because there may be another sequencer using the same riff at the same time
		calc_note_sample_positions();
		riff_play_state = ON;
		out_riff_start_trig = ON;
	}else if (out_total_sample_pos == next_riff_start_sample) {
		riff_sample_pos = 0;
		riff_start_sample = out_total_sample_pos;
		riff_node_ptr = (riff_node *)(riffnodeitem = riffnodelist->goto_next())->get_data();
		calc_riff_sample_positions();
		note_ptr = (note_data*)(noteitem = riff_ptr->sneak_first())->get_data();
		calc_note_sample_positions();
		riff_play_state = ON;
		out_riff_end_trig = ON;
		out_riff_start_trig = ON;
	}else if (out_total_sample_pos == riff_end_sample) {
		riff_sample_pos = 0;
		riff_play_state = OFF;
		out_riff_end_trig = ON;
		note_on_sample_pos = 4294967295ul;
		next_note_sample_pos = 4294967295ul;
		riff_node_ptr = (riff_node *)(riffnodeitem = riffnodelist->goto_next())->get_data();
		calc_riff_sample_positions();
	}else {
		if (out_riff_start_trig == ON) out_riff_start_trig = OFF;
		if (out_riff_end_trig == ON) out_riff_end_trig = OFF;
		if (out_note_off_trig == ON)	out_note_on_trig = OFF;
	}
	if (riff_play_state == ON) {
		if  (riff_sample_pos == note_on_sample_pos) {
			out_note_on_trig = ON;
			out_freq = note_ptr->get_frequency();
			if (velrsp_max_samps > 0) {
				vel_stpsize = (double)(note_ptr->get_velocity() - out_velocity) / velrsp_max_samps;
				velrsp_samp = velrsp_max_samps;
			} else	out_velocity = note_ptr->get_velocity();
			note_play_state = ON;
		}else if (riff_sample_pos == next_note_sample_pos)	{
			note_ptr = (note_data*)(noteitem = noteitem->get_next())->get_data();
			calc_note_sample_positions();
			out_note_slide_trig = ON;
			out_freq = note_ptr->get_frequency();
			if (velrsp_max_samps > 0) {
				vel_stpsize = (double)(note_ptr->get_velocity() - out_velocity) / velrsp_max_samps;
				velrsp_samp = velrsp_max_samps;
			} else	out_velocity = note_ptr->get_velocity();
			note_play_state = ON;
		}else if (riff_sample_pos == note_off_sample_pos) {
			note_ptr = (note_data*)(noteitem = noteitem->get_next())->get_data();
			calc_note_sample_positions();
			out_note_off_trig = ON;
			note_play_state = OFF;
		} else {
			if (out_note_on_trig == ON) out_note_on_trig = OFF;
			if (out_note_slide_trig == ON) out_note_slide_trig = OFF;
			if (out_note_off_trig == ON) out_note_off_trig = OFF;
		}
		riff_sample_pos++;
	} else if (note_play_state == ON) {//last note in riff continues past
		if (riff_sample_pos == note_off_sample_pos)	{ // bar it started in.
			out_note_off_trig = ON;
			note_play_state = OFF;
		}
	}
	if (velrsp_samp > 0) {
		velrsp_samp--;
		out_velocity += vel_stpsize;
	}
	out_total_sample_pos++;
}

riff_list* sequencer::rifflist = 0;
int sequencer::sequencer_count = 0;
bool sequencer::done_params = false;

void sequencer::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SEQUENCER, paramnames::PAR_VELOCITY_RESPONSE);
	done_params = true;
}

#endif
