#ifndef COMBINER_H
#define COMBINER_H

#include "wcntsignal.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "conversions.h"

class combiner: public synthmod 
{
 public:
	combiner(string uname);
	~combiner();
	wcnt_signal* add_signal(wcnt_signal* s){ sigcount++;return wcntsig = (wcnt_signal*)(wcntsig_item = wcntsiglist->add_at_tail(s))->get_data();}
	wcnt_signal* goto_first(){ return wcntsig = (wcnt_signal*)(wcntsig_item = wcntsiglist->goto_first())->get_data();}
	wcnt_signal* goto_last(){ return wcntsig = (wcnt_signal*)(wcntsig_item = wcntsiglist->goto_last())->get_data();}
	wcnt_signal* goto_prev(){ return wcntsig = (wcnt_signal*)(wcntsig_item = wcntsiglist->goto_prev())->get_data();}
	wcnt_signal* goto_next(){ return wcntsig = (wcnt_signal*)(wcntsig_item = wcntsiglist->goto_next())->get_data();}
	void set_mean_total(STATUS s){ meantotal = s;}
	double const* get_output(){ return &out_output;}
	// virtual funcs
	void run();
	void init();
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
		
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
	static void create_params();
	static bool done_params;
};

#endif
