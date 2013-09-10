#ifndef STEREOAMP_H
#define STEREOAMP_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class stereo_amp : public synthmod 
{
 public:
	enum CLIP_MODE { CLIP = 0, INVERT_CLIP = 1 };
	stereo_amp(string uname);
	~stereo_amp();
	void set_input_signal(const double* in){ in_signal = in; }
	void set_input_amp_eg(const double* aeg){ in_amp_eg = aeg; }
	void set_input_amp_mod(const double* am){ in_amp_mod = am; }
	void set_input_pan_mod(const double* pm){ in_pan_mod = pm; }
	const double* get_input_signal(){ return in_signal; }
	const double* get_input_amp_eg(){ return in_amp_eg; }
	const double* get_input_amp_mod(){ return in_amp_mod; }
	const double* get_input_pan_mod(){ return in_pan_mod; }
	const short* get_output_left(){ return &out_left; }
	const short* get_output_right(){ return &out_right; }
	void set_amplitude(short a){ amplitude = a; }
	void set_amp_modsize(double ams){ amp_modsize = ams; }
	void set_pan(double p){ pan = p; }
	void set_pan_modsize(double pms){ pan_modsize = pms; }
	void set_clip_mode(CLIP_MODE cm){ clip_mode = cm; }
	void set_clip_level(short cl){ clip_level = cl; }
	void set_dc_level(short dc){ dc_level = dc; } 
	int get_amplitude(){ return amplitude; }
	double get_amp_modsize(){ return amp_modsize; }
	double get_pan(){ return pan; }
	double get_pan_modsize(){ return pan_modsize; }
	CLIP_MODE get_clip_mode(){ return clip_mode; }
	short get_clip_level(){ return clip_level; }
	short get_dc_level(){ return dc_level; }
	// virtual funcs
	void run();
	void init(){};
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
		
 private:
	short out_left;
	short out_right;
	const double* in_signal;
	const double* in_amp_eg;
	const double* in_amp_mod;
	const double* in_pan_mod;
	short amplitude;
	double amp_modsize;
	double pan;
	double pan_modsize;
	double _amp_level;
	double ampsig;
	double _pan_pos;
	double _left;
	double _right;
	CLIP_MODE clip_mode;
	short clip_level;
	short dc_level;
	static int stereoamp_count;
	static void create_params();
	static bool done_params;
};

#endif
