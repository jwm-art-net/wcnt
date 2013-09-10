#ifndef COMBINER_H
#define COMBINER_H

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


class combiner: public synthmod 
{
 public:
	combiner(string uname);
	~combiner();
	wcnt_signal* add_signal(wcnt_signal* s){ 
		sigcount++;
		return wcntsig = (wcnt_signal*)
			(wcntsig_item = wcntsiglist->add_at_tail(s))->get_data();}
	wcnt_signal* goto_first(){ 
		return wcntsig = (wcnt_signal*)(wcntsig_item = 
			wcntsiglist->goto_first())->get_data();}
	wcnt_signal* goto_last(){ 
		return wcntsig = (wcnt_signal*)(wcntsig_item = 
			wcntsiglist->goto_last())->get_data();}
	wcnt_signal* goto_prev(){ 
		return wcntsig = (wcnt_signal*)(wcntsig_item = 
			wcntsiglist->goto_prev())->get_data();}
	wcnt_signal* goto_next(){ 
		return wcntsig = (wcnt_signal*)(wcntsig_item = 
			wcntsiglist->goto_next())->get_data();}
	void set_mean_total(STATUS s){ meantotal = s;}
	double const* get_output(){ return &out_output;}
	// virtual funcs
	void run();
	void init();
	bool validate();
	// wcnt_signal is not a dobj, but a synthmod, so a dobj wrapper class 
	// - dobjmod, is passed which contains a pointer to the wcnt_signal
	dobj* add_dobj(dobj*); 
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	// combiner does not use add_dobj() because wcnt_signal is a 
	// synthmod not a dobj - but still uses create_moddobj()...
	#endif
		
 private:
	double out_output;
	double total;
	int sigcount;
	STATUS meantotal;
	linkedlist* wcntsiglist;
	ll_item*	wcntsig_item;
	wcnt_signal* wcntsig;
	double const* sig;
	double const* prevsig;
	double zero;// for initial stuff
	static int combiner_count;
 	#ifndef BARE_MODULES
	static bool done_params;
	void create_params();
	static bool done_moddobj;
	void create_moddobj();
 	#endif
};

#endif
