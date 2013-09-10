#ifndef ADSR_H
#define ADSR_H

#include "adsrcoord.h"
#include "linkedlist.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

/* 
ADSR - Attack Decay Sustain Release Envelope
--------------------------------------------
	not quite your standard ADSR, yes it has the attack, decay, sustain, release
	sections, but any number of sub sections can be added (except to sustain - 
	wait for advanced_adsr class to be designed!)  Another un-standard feature is
	that each ADSR is really two ADSRs: lower and upper. Using the velocity input
	will modulate the output between these two adsrs. The value of the velocity
	input is only checked at the begining of each section or sub section not 
	constantly. You can also turn OFF zero re-triggering and the sustain section 
	if you wish.
*/


class adsr : public synthmod
{
 public:
	adsr(string uname);
	~adsr();
	adsr_coord* insert_coord(adsr_coord*);
	adsr_coord* insert_coord(adsr_coord::SECT, double ut, double ul, double lt, double ll);
	adsr_coord* insert_coord_after(adsr_coord*);
	adsr_coord* insert_coord_after(adsr_coord*, double ut, double ul, double lt, double ll);
	void delete_coord(adsr_coord*);
	adsr_coord* goto_section(adsr_coord::SECT);
	adsr_coord* goto_first() { return coord = (adsr_coord*)(coord_item = env->goto_first())->get_data();}
	adsr_coord* goto_last() { return coord = (adsr_coord*)(coord_item = env->goto_last())->get_data(); }
	adsr_coord* goto_prev() { return coord = (adsr_coord*)(coord_item = env->goto_prev())->get_data(); }
	adsr_coord* goto_next() { return coord = (adsr_coord*)(coord_item = env->goto_next())->get_data(); }
	void scale_section(adsr_coord::SECT, double ratio);
	// inputs
	void set_input_velocity(double const* v){ in_velocity = v;}
	void set_input_note_on_trig(STATUS const* ot){ in_note_on_trig = ot;}
	void set_input_note_off_trig(STATUS const* ot){ in_note_off_trig = ot;}
	double const* get_input_velocity(){ return in_velocity;}
	STATUS const* get_input_note_on_trig(){ return in_note_on_trig;}
	STATUS const* get_input_note_off_trig(){ return in_note_off_trig;}
	// outputs
	double const* get_output() { return &output; }
	STATUS const* get_output_off_trig(){ return &out_off_trig;}
	STATUS const* get_play_state() { return &play_state; }
	// params
	void set_start_level(double sl){ start_level = sl;}
	void set_sustain_status(STATUS s){ sustain_status = s;}
	void set_zero_retrigger_mode(STATUS zrm) { zero_retrigger = zrm; }
	STATUS get_sustain_status() { return sustain_status; }
	STATUS get_zero_retrigger_mode() { return zero_retrigger; }
	// virtual funcs
	void run();
	void init();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
	
 private:
	// inputs
	STATUS const* in_note_on_trig;
	STATUS const* in_note_off_trig;
	double const* in_velocity;
	// outputs
	double output;
	STATUS out_off_trig;
	STATUS play_state;
	// params
	double start_level;
	STATUS sustain_status;
	STATUS zero_retrigger;
	// working
	double end_level;
	linkedlist* env;
	short sect;
	unsigned long sectsample;
	unsigned long sectmaxsamples;
	double levelsize;
	ll_item* coord_item;
	adsr_coord* coord;
	static short adsr_count;
	static void create_params();
	static bool done_params;
};

#endif
