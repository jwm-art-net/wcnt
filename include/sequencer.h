/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "rifflist.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

/* 
	class riff_node -- 'ghost' one riff many times without duplicating riffdata.
	I could turn on multiple referencing in the linked list to do the same thing, 
	but then I'd need some other way of telling where to put the riff in the 
	sequence, or make riffs of nothing to fill in the gaps.
	I choose this way. I shall also design a global riff_list which does not
	use the riff_node, but if you've got more than one sequencer, it can choose
	riffs from the list ie riffs are n't tied to any particular sequencer.
	therefore, sequencer cannot use linkedlist->get_current() of riff etc, so, 
	sequencer will have to have it's own members to remember where in the riff 
	it's at, which note it's at.  nb one riff can be used by multiple sequencers,
	at the same time, or different times.
*/	

class riff_node 
{
 public:
	riff_node(riffdata*, short barpos);
	~riff_node();
	short get_start_bar() { return (this == NULL) ? 0 : start_bar; }
	riffdata* get_riff_source() { return (this == NULL) ? NULL : riff_source; }
 private:
	short start_bar;
	riffdata* riff_source;
};

class sequencer : public synthmod
{
 public:
	sequencer(string uname);
	~sequencer();
	// first add_riff func I probably won't use as there is no way of knowing
	// if it returned 0 because it could not find riff called riffname or there
	// is a riff already at bar barpos.
	riff_node* add_riff(string const* riffname, short barpos);
	riff_node* add_riff(riffdata*, short barpos);
	bool delete_riff_node(riff_node* rn);
	riff_node* goto_first() { return (riff_node*)riffnodelist->goto_first()->get_data(); }
	riff_node* goto_last() { return (riff_node*)riffnodelist->goto_last()->get_data(); }
	riff_node* goto_prev() { return (riff_node*)riffnodelist->goto_prev()->get_data(); }
	riff_node* goto_next() { return (riff_node*)riffnodelist->goto_next()->get_data(); }
	const STATUS* get_output_note_on_trig() { return &out_note_on_trig; }
	const STATUS* get_output_note_slide_trig() { return &out_note_slide_trig; }
	const STATUS* get_output_note_off_trig() { return &out_note_off_trig; }
	const STATUS* get_output_riff_start_trig() { return &out_riff_start_trig; }
	const STATUS* get_output_riff_end_trig() { return &out_riff_end_trig; }
	const STATUS* get_output_start_trig() { return &out_start_trig; }
	const STATUS* get_output_end_trig() { return &out_end_trig; }
	const double* get_output_freq() { return &out_freq; }
	const double* get_output_velocity() { return &out_velocity; }
	const unsigned long* get_total_sample_pos() { return &out_total_sample_pos; }
	const STATUS* get_play_state() { return &play_state; }
	const STATUS* get_riff_play_state() { return &riff_play_state; }
	const STATUS* get_note_play_state() { return &note_play_state; }
	// params
	void set_velocity_response_time(double ms){vel_response = (ms >= 0) ? ms : 0;}
	double get_velocity_response_time(){return vel_response;}
	// all sequencers want access to rifflist:
	static void register_rifflist(riff_list* rl){ rifflist = rl;}
	static riff_list* get_rifflist(){ return rifflist;}
	// virtual funcs
	void run();
	void init();
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*){ return false;}
	bool set_param(paramnames::PAR_TYPE, void const*);
		
 private:
	/*outputs*/
	STATUS out_note_on_trig;
	STATUS out_note_slide_trig;
	STATUS out_note_off_trig;
	STATUS out_riff_start_trig;
	STATUS out_riff_end_trig;
	STATUS out_start_trig;
	STATUS out_end_trig;
	double out_freq;
	double out_velocity;
	unsigned long out_total_sample_pos;
	STATUS play_state;
	STATUS riff_play_state;
	STATUS note_play_state;
	/* the list */
	linkedlist* riffnodelist;
	/* working data */
	riff_node* riff_node_ptr;
	riffdata* riff_ptr;
	ll_item* riffnodeitem;
	note_data* note_ptr;
	ll_item* noteitem;
	unsigned long note_on_sample_pos;
	unsigned long note_off_sample_pos;
	unsigned long next_note_sample_pos;
	unsigned long riff_sample_pos;
	unsigned long riff_start_sample;
	unsigned long riff_end_sample;
	unsigned long next_riff_start_sample;
	static int sequencer_count;
	double vel_response;
	unsigned long velrsp_max_samps;
	unsigned long velrsp_samp;
	double vel_stpsize;
	/* helper functions */
	riff_node* remove(riff_node * rn);
	void calc_riff_sample_positions();
	void calc_note_sample_positions();
	static riff_list* rifflist;
	static void create_params();
	static bool done_params;
};

// splaffwuck says:  I can't be bothered!  
// splaffwuck is dead, long live splaffwuck!

#endif
