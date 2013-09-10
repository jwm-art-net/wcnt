#ifndef TRIGGER_H
#define TRIGGER_H

#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class trigger: public synthmod 
{
 public:
	trigger(string uname);
	~trigger();
	void set_input_signal(double const* is){ in_signal = is; }
	double const* get_input_signal(){ return in_signal;}
	STATUS const* get_output_trig(){ return &out_trig;}
	void set_delay_time(double ms){ delay_time = ms; }
	void set_trigger_level(double tl){ trig_level = tl; }
	// virtual funcs
	void run();
	void init(){};
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
		
 private:
	double const* in_signal;
	STATUS out_trig;
	double delay_time;
	double trig_level;
	unsigned long delay_samps;
	static int trigger_count;
	static void create_params();
	static bool done_params;
};

#endif
