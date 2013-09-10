#ifndef STEREOMIXER_H
#define STEREOMIXER_H

#include "stereochannel.h"
#include "linkedlist.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class stereomixer: public synthmod
{
 public:
	stereomixer(string uname);
	~stereomixer();
	stereo_channel* add_channel(stereo_channel* ch);
	stereo_channel* remove_channel(stereo_channel* ch);
	stereo_channel* goto_first_left_channel()
 		{ return (stereo_channel*)chlist.goto_first()->get_data();}
	stereo_channel* goto_last_left_channel()
		{ return (stereo_channel*)chlist.goto_last()->get_data();}
	stereo_channel* goto_prev_left_channel()
		{ return (stereo_channel*)chlist.goto_prev()->get_data();}
	stereo_channel* goto_next_left_channel()
		{ return (stereo_channel*)chlist.goto_next()->get_data();}
	const short* get_output_left(){ return &out_left;}
	const short* get_output_right(){ return &out_right;}
	void set_master_level(double ml) { master_level = ml;}
	double get_master_level(){ return master_level;}
	// virtual funcs
	void run();
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	short out_left;
	short out_right;
	double master_level;
	double o_l;
	double o_r;
	linkedlist chlist;
	ll_item* chitem;
	stereo_channel* chan;
	static int stereomixer_count;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
