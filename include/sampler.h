#ifndef SAMPLER_H
#define SAMPLER_H

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

#include "samplercommon.h"

// I don't think this could get any sillier ;)
// but saying that......of course it could, I could
// have loop points which as well as being offsets
// from a modulatable start position could be modulatable
// aswell.....

// it just keeps growing and growing like I've opened
// pandoras box in an effort to not be satisfied with
// things that only half work, to get them working
// fully as I'd like them to. implimenting reverse
// and bi-directional looping with the existing
// modulatable startpoints opened up all sorts of
// problems, so I added to it by having relative loop
// points, which in turn created other problems....
// .. such as playback away from loop points and how
// to deal with that (see play_modes - not to be
// confused with play_dir)...... causing .... more
// changes to the loop code....

// .... now it's the turn of the anti clip code.
// anti clip - when jumping from end of loop to the
// begining, the waveform gets cut - causing a nasty
// popping sound.  Anti clip smooths the transission 
// by fading out the portion of sound just after the 
// loop, while simultaneously fading in the beginning of 
// the loop.  Unfortunately in wcnt-1.001c it had a 
// slight problem that if it was activated while already
// active, it lost what it was currently doing to do the 
// new ac clip - causing a nasty popping sound, so now 
// I'm trying to fix that by copying existing AC buf 
// into a tempory buffer while fading it and then mixing 
// it with the new AC clip buffer. it's not much fun to
// do and i don't like it but if it works it should be 
// worth it for the end results. 

// now the sampler module is probably the most complex of
// all of them, and before I started revamping it I'd intended
// to create a sampler with two loops, and inputs for handling
// sample positions and loop points etc, from next gen note data
// via next gen sequencer..... am I looking forward to that or
// what!?!?!

class sampler : public synthmod 
{
 public:
	sampler(string uname);
	~sampler();
	// inputs
	void set_input_play_trig(const STATUS* pt ){ in_play_trig = pt;}
	void set_input_stop_trig(const STATUS* st) { in_stop_trig = st;}
	void set_input_deg_size(const double* ds){ in_deg_size = ds;}
	void set_input_start_pos_mod(const double* sp){ in_start_pos_mod = sp;}
	// outputs
	const short* get_output_left(){ return &out_left;}
	const short* get_output_right(){ return &out_right;}
	const double* get_output_l(){ return &out_l;}
	const double* get_output_r(){ return &out_r;}
	const STATUS* get_output_loop_trig(){ return &out_loop_trig;}
	const STATUS* get_play_state(){ return &play_state;}
	// params
	void set_wavfilein(wavfilein* wi);
	void set_play_dir(PLAY_DIR pd)   { play_dir = pd;}
	void set_play_mode(PLAY_MODE pm) { play_mode = pm;}
	void set_jump_mode(JUMP_DIR jm) { jump_mode = jm;}
	void set_start_pos_min(unsigned long samps){ min_start_pos = samps;}
	void set_start_pos_max(unsigned long samps){ max_start_pos = samps;}
	void set_loop_begin(unsigned long samps){ loop_begin = samps;}
	void set_loop_end(unsigned long samps){ loop_end = samps;}
	void set_loop_mode(LOOP_MODE lm){ loop_mode = lm;}
	void set_loop_is_offset(STATUS o){ loop_is_offset = o;}
	void set_loop_bi_offset(short lbo){ loop_bi_offset = lbo;}
	void set_anti_clip_samples(short acs);
	void set_anti_clip_each_end(STATUS acee){ ac_each_end = acee;}
	void set_zero_search_range(short zsr){ search_range = (zsr != 1) ? zsr : 2;}
	void set_degsize_amount(double css){ deg_size_amount = css;} // 0 ignore 1 western  !=1 macrotonal
	// virtual funcs
	void run();
	void init();
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	// inputs
	const STATUS* in_play_trig;
 	const STATUS* in_stop_trig;
	const double* in_start_pos_mod;
	const double* in_deg_size;
	// outputs
	short out_left;
	short out_right;
	double out_l;
	double out_r;
	STATUS out_loop_trig;
	STATUS play_state;
	// params
	wavfilein* wavfile;
 	PLAY_DIR play_dir;
 	PLAY_MODE play_mode;
	JUMP_DIR jump_mode;
	unsigned long min_start_pos;
	unsigned long max_start_pos;
	unsigned long loop_begin;
	unsigned long loop_end;
	STATUS loop_is_offset;
	LOOP_MODE loop_mode;
	short loop_bi_offset;
	short anti_clip_size;
	STATUS ac_each_end;
	short search_range;
	double deg_size_amount;
	// working
	double root_deg_size;
 	PLAY_DIR playdir;
	PLAY_DIR acplaydir;
	bool loop_yet;
	short* mono_buffer;
	short* ac_m_buf;
	stereodata* st_buffer;
	stereodata* ac_st_buf;
	unsigned long buffer_start_pos;
	int buff_pos;
	unsigned long wavstart;
	unsigned long wavlength;
	unsigned long wavstbi;
	unsigned long wavlenbi;
	double cur_pos;
	double ac_cpstep;
	double oldcpstp;
	double cp_step;
	double cp_ratio;
	double sr_ratio; //ratio between input and output samplerates
	double bp_midpoint;
	unsigned long start_pos;
	unsigned long startpos_span;
	unsigned long loopstart;
	unsigned long loopfinish;
	bool loop_fits_in_buffer;
	bool loop_loaded;
	STATUS do_ac;
	double ac_cur_pos;
	double ac_midpoint;
	int ac_buf_pos;
	unsigned long ac_buf_start_pos;
	double ac_step;
	double ac_size;
	double ac_out_left;
	double ac_out_right;
	WAV_CHANNELS ch;
	unsigned long sampletot;
	// private helper functions:
	inline void fill_buffer(unsigned long);
	inline short calc_midpoint(short, short, double);
	unsigned long zero_search(unsigned long, short range);
	void trigger_playback();
	void pos_wavlen();
	void pos_loopend();
	void pos_wavstart();
	void pos_loopbegin();
	void anti_clip_fwd();
	void anti_clip_rev();
	void ac_copy_fwd_mono(short*);
	void ac_copy_fwd_stereo(stereodata*);
	void ac_copy_rev_mono(short*);
	void ac_copy_rev_stereo(stereodata*);
	void ac_mix_fwd_mono(short*);
	void ac_mix_fwd_stereo(stereodata*);
	void ac_mix_rev_mono(short*);
	void ac_mix_rev_stereo(stereodata*);
	// synthmod stuff
	static int sampler_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
	#endif
};

#endif
