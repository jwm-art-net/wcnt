#ifndef SAMPLER_H
#include "../include/sampler.h"


sampler::sampler(string uname) //  ;->
: synthmod(synthmodnames::MOD_SAMPLER, sampler_count, uname),
  out_left(0), out_right(0), out_l(0.00), out_r(0.00), play_state(OFF), in_play_trig(NULL), in_start_pos_mod(NULL),  
  in_deg_size(NULL), wavfile(NULL), mono_buffer(NULL), ac_m_buf(NULL), st_buffer(NULL), ac_st_buf(NULL), 
  buffer_start_pos(0), buff_pos(0), wavlength(0), bp_midpoint(0.00), anti_clip_size(25),  ac_step(0.00), ac_size(0.00),
  ac_out_left(0.00), ac_out_right(0.00), ch(WAV_CH_UNKNOWN), start_pos(0), min_start_pos(0), max_start_pos(0), 
  mid_sp(0), spmod_rad(0), cur_pos(0.00), cp_step(0.00), cp_ratio(0), loop_begin(0), loop_end(0),
  loop_mode(LOOP_OFF), root_deg_size(1), deg_size_amount(0), sr_ratio(1.00)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_LEFT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_RIGHT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_L)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_R)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PLAY_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_START_POS_MOD)){
		invalidate();
		return;
	}
	set_root_note("c0");
	sampler_count++;
	validate();
	create_params();
}

sampler::~sampler() 
{
	if (mono_buffer)
		delete mono_buffer; 
	if (st_buffer) 
		delete st_buffer;
	if (ac_m_buf)
		delete ac_m_buf;
	if (ac_st_buf)
		delete ac_st_buf;
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* sampler::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_LEFT:
			o = &out_left;
			break;
		case outputnames::OUT_RIGHT:
			o = &out_right;
			break;
		case outputnames::OUT_L:
			o = &out_l;
			break;
		case outputnames::OUT_R:
			o = &out_r;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* sampler::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_PLAY_TRIG:
			i = in_play_trig = (STATUS*)o;
			break;
		case inputnames::IN_DEG_SIZE:
			i = in_deg_size = (double*)o;
			break;
		case inputnames::IN_START_POS_MOD:
			i = in_start_pos_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool sampler::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_WAVFILEIN:
			set_wavfilein((wavfilein*)data); //note: pass pointer
			retv = true;
			break;
		case paramnames::PAR_START_POS_MIN:
			set_start_pos_min(*(unsigned long*)data);
			retv = true;
			break;
		case paramnames::PAR_START_POS_MAX:
			set_start_pos_max(*(unsigned long*)data);
			retv = true;
			break;
		case paramnames::PAR_LOOP_BEGIN:
			set_loop_begin(*(unsigned long*)data);
			retv = true;
			break;
		case paramnames::PAR_LOOP_END:
			set_loop_end(*(unsigned long*)data);
			retv = true;
			break;
		case paramnames::PAR_LOOP_MODE:
			set_loop_mode(*(LOOP_MODE*)data);
			retv = true;
			break;
		case paramnames::PAR_ANTI_CLIP:
			set_anti_clip_samples(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_ROOT_NOTE:
			set_root_note((char*)data); //note: pass pointer
			retv = true;
			break;
		case paramnames::PAR_ROOT_FREQ:
			set_root_freq(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_ROOT_DEGSIZE:
			set_root_degsize(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_DEGSIZE_AMOUNT:
			set_degsize_amount(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void sampler::set_wavfilein(wavfilein * wi) 
{
	if (wi->get_status() == WAV_STATUS_OPEN) 
	{
		wavfile = wi; 
		// next two statements save overhead of function calls in run() as they are checked on every sample
		ch = wavfile->get_channel_status();
		wavlength = wavfile->get_length();
		sr_ratio = (double)wavfile->get_sample_rate() / audio_samplerate;
		if (ch == WAV_CH_MONO)
		{
			if ((mono_buffer = new short[WAV_BUFFER_SIZE]) == NULL) 
			{
				invalidate();
				return;
			}
			for (int i = 0; i < WAV_BUFFER_SIZE; i++) 
				mono_buffer[i] = 0;
			if ((ac_m_buf = new short[MAX_ANTI_CLIP_SIZE]) == NULL)
			{
				invalidate();
				return;
			}
			for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++)
				ac_m_buf[i] = 0;
		}
		else {
			if ((st_buffer = new stereodata[WAV_BUFFER_SIZE]) == NULL) 
			{
				invalidate();
				return;
			}
			for (int i = 0; i < WAV_BUFFER_SIZE; i++) 
			{
				st_buffer[i].left = 0;
				st_buffer[i].right = 0;
			}
			if ((ac_st_buf = new stereodata[MAX_ANTI_CLIP_SIZE]) == NULL)
			{
				invalidate();
				return;
			}
			for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++)
			{
				ac_st_buf[i].left = 0;
				ac_st_buf[i].right = 0;
			}
		}
		max_start_pos = (unsigned long)(wavlength * 0.25);
		loop_end = wavlength;
	}
}

void sampler::set_root_note(char* rn)
{
	double rds = note_to_step(rn, 0);
	if (rds != 0.00)
		root_deg_size = rds;
}

void sampler::set_anti_clip_samples(short acs)
{ 
	if (acs < 0)
		return;
	anti_clip_size = (acs < MAX_ANTI_CLIP_SAMPLES) ? acs : MAX_ANTI_CLIP_SAMPLES;
	if (anti_clip_size == 0) 
		ac_size = 1; // prevents execution of some anticlip code
}

//initialise startpos radius and mid point from min and max 
void sampler::init()
{
	spmod_rad = (max_start_pos - min_start_pos) / 2;
	mid_sp = min_start_pos + spmod_rad;
	start_pos = 4294967295ul;
}

// read data from wav file into buffer
inline void sampler::fill_buffer(unsigned long pos)
{
	if (ch == WAV_CH_MONO) 
		wavfile->read_wav_at(mono_buffer, pos); 
	else 
		wavfile->read_wav_at(st_buffer, pos);
}

// as we step through the sample, it is very likly we land in between sample a and sample b,
// so we calculate an inbetween value - this function improves the code readability
inline short sampler::calc_midpoint(short a, short b, double r)
{
	return (short)(a + (b - a) * r);
}

// to initialise anti clip buffer and associated variables when play state is OFF.
inline void sampler::anti_clip_init_psoff()
{
	if (anti_clip_size > 0)
	{
		if (ch == WAV_CH_MONO)	
			for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++) ac_m_buf[i] = 0;
		else 
			for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++) 
			{
				ac_st_buf[i].left = 0; 
				ac_st_buf[i].right = 0; 
			}
		ac_out_left = ac_out_right = 0;
		ac_step = 1.00 / (double)anti_clip_size;
		ac_size = 0;
		out_l = out_r = 0;
	}
}

// to initialise anti clip buffer and associated variables when play state is ON.
inline void sampler::anti_clip_init_pson(unsigned long pos)
{
	if (anti_clip_size > 0) 
	{  // do we need to read from file or can we use the wav buffer?
		if (pos - buffer_start_pos + MAX_ANTI_CLIP_SIZE > WAV_BUFFER_SIZE - 1) 
		{ 
			if (ch == WAV_CH_MONO)	
				wavfile->read_wav_chunk(ac_m_buf, pos, MAX_ANTI_CLIP_SIZE);
			else 
				wavfile->read_wav_chunk(ac_st_buf, pos, MAX_ANTI_CLIP_SIZE);
		}
		else
		{   // use the wav buffer
			if (ch == WAV_CH_MONO)
				for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++)
					ac_m_buf[i] = mono_buffer[pos - buffer_start_pos + i];
			else
				for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++) 
				{
					ac_st_buf[i].left = st_buffer[pos - buffer_start_pos + i].left;
					ac_st_buf[i].right = st_buffer[pos - buffer_start_pos + i].right;
				}
		}
		if (ac_size < 1) cout << "\nanticlip activating while already active!";
		ac_out_left = ac_out_right = 0;
		ac_step = 1.00 / (double)anti_clip_size;
		ac_size = 0;
		out_l = out_r = 0;
		ac_cur_pos = ac_buf_start_pos = pos;
	}
}

void sampler::run()
{
	if (play_state == OFF)
	{
		if (*in_play_trig == ON)
		{  // initialise playback 
			play_state = ON;
			start_pos = (unsigned long)(mid_sp + spmod_rad * (*in_start_pos_mod));
			buffer_start_pos = (unsigned long)(cur_pos = start_pos);
			fill_buffer(start_pos);
			anti_clip_init_psoff();
		}
	} 
	else 
	{
		if (*in_play_trig == ON) 
		{  // retrigger playback
			anti_clip_init_pson(cur_pos); 
			play_state = ON;
			start_pos = (unsigned long)(mid_sp + spmod_rad * (*in_start_pos_mod));
			buffer_start_pos = (unsigned long)(cur_pos = start_pos);
			fill_buffer(start_pos);
		}
		cp_ratio = *in_deg_size / root_deg_size;
		cp_step = (1 + (cp_ratio - 1) * deg_size_amount) * sr_ratio;  
		buff_pos = (unsigned int)cur_pos - buffer_start_pos;
		if (buff_pos >= WAV_BUFFER_SIZE - 2) 
		{  
			if (cur_pos < wavlength)
			{  // read more data from wav file
				fill_buffer(cur_pos);
				buffer_start_pos = (unsigned long)cur_pos;
				buff_pos = (unsigned int)(cur_pos - buffer_start_pos);
			}
			else 
			{
				cout << "cur_pos in sampler is beyond length of wav file";
			}
		}
		ac_buf_pos = (unsigned int)ac_cur_pos - ac_buf_start_pos;
		if (ac_buf_pos >= MAX_ANTI_CLIP_SIZE - 2)
		{  // hopefully unlikly
			cout << "\nreached end of anti clip buffer    ";
			ac_buf_pos = MAX_ANTI_CLIP_SIZE - 2;
		}
		bp_midpoint = (double)((cur_pos - buffer_start_pos) - buff_pos); 
		ac_midpoint = (double)((ac_cur_pos - ac_buf_start_pos) - ac_buf_pos);
		if (ch == WAV_CH_MONO) 
		{
			out_left = calc_midpoint(mono_buffer[buff_pos], mono_buffer[buff_pos + 1], bp_midpoint);
			if (ac_size < 1) 
			{
				ac_out_left = calc_midpoint(ac_m_buf[ac_buf_pos], ac_m_buf[ac_buf_pos + 1], ac_midpoint);
				out_left = (short)(out_left * ac_size + ac_out_left * (1 - ac_size));
			}
			out_right = out_left;
			out_l = out_r = (double)out_left / 0x7fff;
		} 
		else 
		{
			out_left = calc_midpoint(st_buffer[buff_pos].left, st_buffer[buff_pos + 1].left, bp_midpoint);
			out_right = calc_midpoint(st_buffer[buff_pos].right, st_buffer[buff_pos + 1].right, bp_midpoint);
			if (ac_size < 1) 
			{
				ac_out_left = calc_midpoint(ac_st_buf[ac_buf_pos].left, ac_st_buf[ac_buf_pos + 1].left, ac_midpoint);
				ac_out_right = calc_midpoint(ac_st_buf[ac_buf_pos].right, ac_st_buf[ac_buf_pos + 1].right, ac_midpoint);
				out_left = (short)(out_left * ac_size + ac_out_left * (1 - ac_size));
				out_right = (short)(out_right * ac_size + ac_out_right * (1 - ac_size));
			}
			out_l = (double)out_left / 0x7fff;
			out_r = (double)out_right / 0x7fff;
		}
		if (ac_size < 1) 
		{
			ac_size += ac_step;
			ac_cur_pos += cp_step;
		}
		cur_pos += cp_step;
		if (cur_pos > loop_end) // && start_pos < loop_end)
		{ // could not see any best way to go about this checking
			if (loop_mode == LOOP_ON)
			{
				anti_clip_init_pson(cur_pos);
				cur_pos = loop_begin;
				fill_buffer(cur_pos);
				buffer_start_pos = (unsigned long)cur_pos;
			}
			else if (cur_pos > wavlength)
			{
				out_left = out_right = 0;
				out_l = out_r = 0;
				play_state = OFF;
			}
		} 
	}
}

int sampler::sampler_count = 0;
bool sampler::done_params = false;
wavfilein_list* sampler::wavfilelist = 0;

void sampler::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_WAVFILEIN);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_START_POS_MIN);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_START_POS_MAX);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_BEGIN);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_END);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_ANTI_CLIP);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_ROOT_NOTE);
//	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_ROOT_FREQ);
//	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_ROOT_DEGSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_DEGSIZE_AMOUNT);
	done_params = true;
	// the two parameters commented out because I've not implemented the ability for
	// the user to choose between which to choose .
}


#endif
