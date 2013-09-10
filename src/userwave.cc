#ifndef USERWAVE_H
#include "../include/userwave.h"

user_wave::user_wave(string uname)
: synthmod(synthmodnames::MOD_USERWAVE, user_wave_count, uname),
  in_phase_trig(0), in_deg_size(0), in_h_mod(0), in_v_mod(0),
  output(0.0), play_state(OFF), env(0), vertex_item(0), vertex(0), 
  sect(0), sect_spanlvl(0.0), sect_startlvl(0.0), 
  counter_ratio(0), sectdegs(0), degs(360), recycle(OFF), zero_retrigger_mode(OFF)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_V_MOD)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_H_MOD)){
		invalidate();
		return;
	}
	if (!(env = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA)))
	{
		invalidate();
		return;
	}
	if (!add_vertex(0.0, 0.0, 0.0, 0.0))
	{
		invalidate();
		return;
	}
	if (!add_vertex(360.0, 0.0, 360.0, 0.0))
	{
		invalidate();
		return;
	}
	user_wave_count++;
	validate();
	create_params();
}

user_wave::~user_wave() 
{
	goto_first();
	while (vertex) 
	{
		tmp = vertex;
		goto_next();
		delete tmp;
	}
	delete env;
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* user_wave::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		case outputnames::OUT_PLAY_STATE:
			o = &play_state;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* user_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_PHASE_TRIG:
			i = in_phase_trig = (STATUS*)o;
			break;
		case inputnames::IN_DEG_SIZE:
			i = in_deg_size = (double*)o;
			break;
		case inputnames::IN_V_MOD:
			i = in_v_mod = (double*)o;
			break;
		case inputnames::IN_H_MOD:
			i = in_h_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool user_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_RECYCLE_MODE:
			set_recycle_mode(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_ZERO_RETRIGGER:
			set_zero_retrigger_mode(*(STATUS*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

wave_vertex* user_wave::add_vertex(wave_vertex* wv)
{
	if (!wv)
		return 0;
	if (!ordered_insert(env, wv, &wave_vertex::oi_get_position))
		return 0;
	return wv;
}

wave_vertex* user_wave::add_vertex(double udp, double ul, double ldp, double ll)
{
	wave_vertex* tmp = new wave_vertex(udp, ul, ldp, ll);
	if (!tmp)
		return 0;
	if (!ordered_insert(env, tmp, &wave_vertex::oi_get_position))
	{
		delete tmp;
		return 0;
	}
	return tmp;
}

bool user_wave::delete_vertex(wave_vertex* wv)
{
	if (!wv)
		return false;
	if (wv == get_first() || wv == get_last())
		return false;  // don't allow user to delete initial or end
	
	ll_item* tmp = env->find_data(wv);
	if (tmp == 0)
		return false;
	delete env->unlink_item(tmp);
	delete wv;
	return true;
}

void user_wave::run() 
{
	if (*in_phase_trig == ON) {
		play_state = ON;
		goto_first(); while(vertex) {
			vertex->modulate(*in_v_mod, *in_h_mod);
			goto_next(); } 
		goto_first(); // first is (normally) zero level, zero degs
		if (zero_retrigger_mode == ON) output = vertex->output.get_level();
		sect_startlvl = output;
		goto_next(); // first off
		sect_spanlvl = vertex->output.get_level() - sect_startlvl;
		sectdegs = vertex->output.get_position();
		degs = 0;
		pdegs = 0;
	}
	degs += *in_deg_size;
	if (play_state == ON) {
		counter_ratio = (degs - pdegs) / (sectdegs - pdegs);
		output = sect_startlvl + sect_spanlvl * counter_ratio;
		if (degs >= sectdegs) {
			tmp = vertex;
			goto_next();
			if (vertex) 	{
				sect_startlvl = output;
				sect_spanlvl = vertex->output.get_level() - sect_startlvl;
				pdegs = tmp->output.get_position();
				sectdegs = vertex->output.get_position();
			} else {
				if (recycle == ON) {
					play_state = ON;
					goto_first();
					while(vertex) {
						vertex->modulate(*in_v_mod, *in_h_mod);
						goto_next();
					} 
					goto_first();
					if (zero_retrigger_mode == ON) output = vertex->output.get_level();
					sect_startlvl = output;
					goto_next();
					sect_spanlvl = vertex->output.get_level() - sect_startlvl;
					sectdegs = vertex->output.get_position();
					degs = 0;
					pdegs = 0;
				} else {
					play_state = OFF;
					output = 0;
				}
			}
		}
	}
	if (degs >= 360) degs -= 360;
}

int user_wave::user_wave_count = 0;
bool user_wave::done_params = false;

void user_wave::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_USERWAVE, paramnames::PAR_RECYCLE_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_USERWAVE, paramnames::PAR_ZERO_RETRIGGER);
	done_params = true;
}


#endif
