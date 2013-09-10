#ifndef SAMPLER_H
#define SAMPLER_H

#include "wavfileinlist.h"
#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

#define MAX_ANTI_CLIP_SIZE 4096
#define MAX_ANTI_CLIP_SAMPLES 256

class sampler : public synthmod 
{
 public:
	enum LOOP_MODE{ LOOP_OFF = 0, LOOP_ON, LOOP_BI };
	sampler(string uname);
	~sampler();
	void set_input_play_trig(const STATUS* pt ){ in_play_trig = pt;}
	void set_input_deg_size(const double* ds){ in_deg_size = ds;}
	void set_input_start_pos_mod(const double* sp){ in_start_pos_mod = sp;}
	const short* get_output_left(){ return &out_left;}
	const short* get_output_right(){ return &out_right;}
	const double* get_output_l(){ return &out_l;}
	const double* get_output_r(){ return &out_r;}
	const STATUS* get_play_state(){ return &play_state;}
	void set_wavfilein(wavfilein* wi);
	void set_start_pos_min(unsigned long samps){ min_start_pos = samps;}
	void set_start_pos_max(unsigned long samps){ max_start_pos = samps;}
	void set_loop_begin(unsigned long samps){ loop_begin = samps;}
	void set_loop_end(unsigned long samps){ loop_end = samps;}
	void set_loop_mode(LOOP_MODE lm){ loop_mode = lm;}
	void set_anti_clip_samples(short acs);
	void set_root_note(char * rn);
	void set_root_freq(double rf){ root_deg_size = freq_to_step( rf, 0 );}
	void set_root_degsize(double rds){ root_deg_size = rds;}
	void set_degsize_amount(double css){ deg_size_amount = css;} // 0 ignore 1 western  !=1 macrotonal
	static void register_wavfilein_list(wavfilein_list* w) { wavfilelist = w;}
	static wavfilein_list* get_wavfilein_list(){ return wavfilelist;}
	// virtual funcs
	void run();
	void init();
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);

 private:
	short out_left;
	short out_right;
	double out_l;
	double out_r;
	STATUS play_state;
	const STATUS* in_play_trig;
	const double* in_start_pos_mod;
	const double* in_deg_size;
	wavfilein* wavfile;
	short* mono_buffer;
	short* ac_m_buf;
	stereodata* st_buffer;
	stereodata* ac_st_buf;
	unsigned long buffer_start_pos;
	unsigned int buff_pos;
	unsigned long wavlength;
	double bp_midpoint;
	short anti_clip_size;
	double ac_step;
	double ac_size;
	double ac_out_left;
	double ac_out_right;
	WAV_CHANNELS ch;
	unsigned long	start_pos;
	unsigned long 	min_start_pos;
	unsigned long 	max_start_pos;
	unsigned long mid_sp;
	unsigned long spmod_rad;
	double cur_pos;
	double cp_step;
	double cp_ratio;
	double ac_cur_pos;
	double ac_midpoint;
	unsigned int ac_buf_pos;
	unsigned long ac_buf_start_pos;
	unsigned long 	loop_begin;
	unsigned long 	loop_end;
	LOOP_MODE loop_mode;
	double root_deg_size;
	double deg_size_amount;
	double sr_ratio; //ratio between input and output samplerates
	static int sampler_count;
	/* helper functions - in attempt to make sampler::run() code clearer */
	inline void fill_buffer(unsigned long);
	inline void anti_clip_init_pson(unsigned long); 	//when playstate ON
	inline void anti_clip_init_psoff();					//when playstate OFF
	inline short calc_midpoint(short, short, double);
	static void create_params();
	static bool done_params;
	static wavfilein_list* wavfilelist;
};

#endif
