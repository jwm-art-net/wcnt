#ifndef SWITCHER_H
#define SWITCHER_H

#include "wcntsignal.h"
#include "conversions.h"
#include "linkedlist.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "dobjdobjlist.h"
#include "dobjmod.h"
#include "dobjlist.h"
#endif

class switcher: public synthmod 
{
 public:
	switcher(string uname);
	~switcher();
	wcnt_signal* add_signal(wcnt_signal* s){ return wcntsig = (wcnt_signal*)
		(wcntsig_item = wcntsiglist->add_at_tail(s))->get_data();}
	wcnt_signal* goto_first(){ return wcntsig = (wcnt_signal*)
		(wcntsig_item = wcntsiglist->goto_first())->get_data();}
	wcnt_signal* goto_last(){ return wcntsig = (wcnt_signal*)
		(wcntsig_item = wcntsiglist->goto_last())->get_data();}
	wcnt_signal* goto_prev(){ return wcntsig = (wcnt_signal*)
		(wcntsig_item = wcntsiglist->goto_prev())->get_data();}
	wcnt_signal* goto_next(){ return wcntsig = (wcnt_signal*)
		(wcntsig_item = wcntsiglist->goto_next())->get_data();}
	void set_input_trig(STATUS const* it){ in_trig = it; }
	STATUS const* get_input_trig(){ return in_trig;}
	double const* get_output(){ return &out_output;}
	void set_xfadetime(double xt){ xfadetime = xt;}
	double get_xfadetime(){ return xfadetime;}
	// virtual funcs
	void run();
	void init();
	bool validate();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	// wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class 
	// - dobjmod, is passed which contains a pointer to the wcnt_signal
	dobj* add_dobj(dobj*); 
	#endif
 private:
	STATUS const* in_trig;
	double out_output;
	double xfadetime;
	unsigned long xfade_samp;
	unsigned long xfade_max_samps;
	double xfade_stpsz;
	double xfade_size;
	linkedlist* wcntsiglist;
	ll_item*	wcntsig_item;
	wcnt_signal* wcntsig;
	double const* sig;
	double const* prevsig;
	double zero;// for initial stuff
	static int switcher_count;
 	#ifndef BARE_MODULES
	static bool done_params;
	void create_params();
	static bool done_moddobj;
	void create_moddobj();
 	#endif
};

#endif
