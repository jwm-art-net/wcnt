#ifndef SINEWAVE_H
#define SINEWAVE_H

#include "dtr.h"
#include <stdlib.h>
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

/*
	sine_wave
	---------------
	
	input_phase_trig 	- 	trigger  generation of cycles
	input_deg_size 	- 	how many degree's to step through at each sample
						(to generate sine of frequency specified by clock)
	recycle_mode		-	ON: to never stop generating
						OFF: stop after 360 * cycles
	on_trig_zero_deg	-	ON:  reset phase on phase_trig  (imperfect), 
						OFF:  perfection in normal circumstances
	cycles			-	number of cycles to generate on phase trig
	
	notes
	-------
		for oscilators:
			for normal expected behaviour, phase_trig & deg_size should be
			from the same oscclock.  recycle mode = OFF, on_trig_zero_degs = OFF
			cycles = 1.00
			for other behaviour for example to generate one cycle at octave 1 
			triggered	from oscclock at octave -1, set recycle mode = OFF, cycles = 1.
			to generate half a cycle at octave -1 triggered from oscclock at octave 0
			set on_trig_zero_degs = ON
		for LFO's
			set phase trig and degsize from same lfoclock.  
			See LFO documentation.
*/

class sine_wave : public synthmod 
{
 public:
	sine_wave(string uname);
	~sine_wave();
	void set_input_phase_trig(const STATUS* pt){ in_phase_trig = pt;}
	void set_input_deg_size(const double* ds){ in_deg_size = ds; }
	const STATUS* get_input_phase_trig(){ return in_phase_trig; }
	const double* get_input_deg_size(){ return in_deg_size; }
	const double* get_output(){ return &output; }
	const STATUS* get_play_state(){ return &play_state; }
	void set_recycle_mode(STATUS rm){ recycle = rm; }
	void set_on_trig_zero_deg(STATUS zd) { zero_deg = zd; }
	void set_cycles(double c){ cycles = c; }
	STATUS get_recycle_mode(){ return recycle; }
	STATUS get_on_trig_zero_deg(STATUS zd) { return zero_deg;}
	double get_cycles(){ return cycles; }
	// virtual funcs
	void run();
	void init();
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	
 private:
	double output;
	STATUS play_state;
	const STATUS * in_phase_trig;
	const double * in_deg_size;
	double degs;
	STATUS recycle;
	STATUS zero_deg;
	double cycles;
	double maxdegs;
	static int sine_wave_count;
	static void create_params();
	static bool done_params;
};

#endif
