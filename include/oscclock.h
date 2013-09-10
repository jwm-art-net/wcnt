#ifndef OSCCLOCK_H
#define OSCCLOCK_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class osc_clock : public synthmod 
{
 public:
	osc_clock(string uname);
	~osc_clock();
	void set_input_note_on_trig(const STATUS* t){ in_note_on_trig = t; }
	void set_input_note_slide_trig(const STATUS* t){ in_note_slide_trig = t; }
	void set_input_freq(const double* f){ in_freq = f; }
	void set_input_freq_mod1(const double* fm){ in_freq_mod1 = fm; }
	void set_input_freq_mod2(const double* fm){ in_freq_mod2 = fm; }
	const STATUS* get_input_note_on_trig(){ return in_note_on_trig; }
	const STATUS* get_input_note_slide_trig(){ return in_note_slide_trig; }
	const double* get_input_freq(){ return in_freq; }
	const double* get_input_freq_mod1(){ return in_freq_mod1; }
	const double* get_input_freq_mod2(){ return in_freq_mod2; }
	const STATUS* get_output_phase_trig(){ return &out_phase_trig; }
	const double* get_output_deg_size(){ return &out_deg_size; }
	const double* get_output_premod_deg_size(){ return &out_premod_deg_size; }
	void set_freq_mod1size(double fms){ freq_mod1size = (fms > 1) ? fms : 1; }
	void set_freq_mod2size(double fms){ freq_mod2size = (fms > 1) ? fms : 1; }
	void set_octave(char o){ octave_offset = o; }
	void set_tuning_semitones(double s);
	void set_portamento(double f){ portamento = f; }
	double get_freq_mod1size(){ return freq_mod1size; }
	double get_freq_mod2size(){ return freq_mod2size; }
	char get_octave(){ return octave_offset; }
	double get_tuning_semitones(){ return semitones;}
	double get_portamento()	{ return portamento; }
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
	const double* in_freq;
	const STATUS* in_note_on_trig;
	const STATUS* in_note_slide_trig;
	const double* in_freq_mod1;
	const double* in_freq_mod2;
	char octave_offset;
	double semitones;
	double freq_mod1size;
	double freq_mod2size;
	double mod1size;
	double mod2size;
	double degs;
	double degsize1;
	double degsize2;
	double portamento;
	double slide_size;
	double target_deg_size;
	unsigned long slidesamples;
	static int osc_clock_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
	#endif
};

#endif
