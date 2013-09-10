#ifndef LFOCLOCK_H
#define LFOCLOCK_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class lfo_clock : public synthmod 
{
 public:
	lfo_clock(string uname);
	~lfo_clock();
	void set_input_phase_trig(const STATUS* pti){ in_phase_trig = pti; }
	void set_input_freq_mod1(const double* fm){ in_freq_mod1 = fm; }
	void set_input_freq_mod2(const double* fm){ in_freq_mod2 = fm; }
	const STATUS* get_input_phase_trig(){ return in_phase_trig; }
	const double* get_input_freq_mod1(){ return in_freq_mod1; }
	const double* get_input_freq_mod2(){ return in_freq_mod2; }
	const STATUS* get_output_phase_trig(){ return &out_phase_trig; }
	const double* get_output_deg_size(){ return &out_deg_size; }
	const double* get_output_premod_deg_size(){ return &out_premod_deg_size; }
	void set_freq_mod1size(double fms){ freq_mod1size = (fms > 1) ? fms : 1; }
	void set_freq_mod2size(double fms){ freq_mod2size = (fms > 1) ? fms : 1; }
	void set_notelength_frequency(int nl);
	void set_frequency(double f);
	double get_freq_mod1size(){ return freq_mod1size; }
	double get_freq_mod2size(){ return freq_mod2size; }
	int get_notelength_frequency(){ return note_length_freq; }
	double get_frequency(){ return hrtz_freq; }
	// virtual funcs
	void run();
	void init();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	STATUS out_phase_trig;
	double out_deg_size;
	double out_premod_deg_size;
	int note_length_freq;
	double hrtz_freq;
	const STATUS* in_phase_trig;
	const double* in_freq_mod1;
	const double* in_freq_mod2;
	double freq_mod1size;
	double freq_mod2size;
	double mod1size;
	double mod2size;
	double degs;
	double degsize1;
	double degsize2;
	static int lfo_clock_count;
 	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
 	#endif
};

#endif
