#ifndef USERWAVE_H
#define USERWAVE_H

#include "userwavevertex.h"
#include "synthmodule.h"
#include "linkedlist.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class user_wave : public synthmod 
{
 public:
	user_wave(string uname);
	~user_wave();
	wave_vertex* add_vertex(wave_vertex* wv);
	wave_vertex* add_vertex(double udp, double ul, double ldp, double ll);
	bool delete_vertex(wave_vertex* wv);
	wave_vertex* goto_first(){ return vertex = (wave_vertex*)(vertex_item = env->goto_first())->get_data();}
	wave_vertex* goto_last(){ return vertex = (wave_vertex*)(vertex_item = env->goto_last())->get_data();}
	wave_vertex* goto_prev(){ return vertex = (wave_vertex*)(vertex_item = env->goto_prev())->get_data();}
	wave_vertex* goto_next(){ return vertex = (wave_vertex*)(vertex_item = env->goto_next())->get_data();}
	wave_vertex* get_first(){ return (wave_vertex*)(env->sneak_first())->get_data();}
	wave_vertex* get_last(){ return (wave_vertex*)(env->sneak_last())->get_data();}
	wave_vertex* get_prev(){ return (wave_vertex*)(env->sneak_prev())->get_data();}
	wave_vertex* get_next(){ return (wave_vertex*)(env->sneak_next())->get_data();}
	void set_input_phase_trig(STATUS const* pt){ in_phase_trig = pt;}
	void set_input_deg_size(double const* ds){ in_deg_size = ds; }
	void set_input_h_mod(double const* hm ){ in_h_mod = hm; }
	void set_input_v_mod(double const* vm ){ in_v_mod = vm; }
	STATUS const* get_input_phase_trig(){ return in_phase_trig;}
	double const* get_input_deg_size(){ return in_deg_size; }
	double const* get_input_h_mod(){ return in_h_mod;}
	double const* get_input_v_mod(){ return in_v_mod;}
	double const* get_output(){ return &output;}
	STATUS const* get_play_state(){ return &play_state;}
	void set_recycle_mode(STATUS rm){ recycle = rm;}
	void set_zero_retrigger_mode(STATUS zrm){ zero_retrigger_mode = zrm;}
	STATUS get_recycle_mode(){ return recycle;}
	STATUS get_zero_retrigger_mode(){ return zero_retrigger_mode;}
	// virtual funcs
	void run();
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	STATUS const* in_phase_trig;
	double const* in_deg_size;
	double const* in_pwm;
	double const* in_h_mod;
	double const* in_v_mod;
	double output;
	STATUS play_state;
	linkedlist* env;
	ll_item* vertex_item;
	wave_vertex* vertex;
	wave_vertex* tmp;
	short sect;
	double sect_spanlvl;
	double sect_startlvl;
	double counter_ratio;
	double sectdegs;
	double degs;
	double pdegs;
	STATUS recycle;
	STATUS zero_retrigger_mode;
	static int user_wave_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
	#endif
};

#endif
