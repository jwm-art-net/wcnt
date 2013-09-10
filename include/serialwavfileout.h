#ifndef SERIALWAVFILEOUT_H
#define SERIALWAVFILEOUT_H

#include <stdio.h>

#include "wavfileheader.h"
#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif
/*
	writes multiple wav files.
	
	starts writing in between start bar and end bar.
	each time write_trig is ON it starts writing a new wav file.
	each time stop_trig is ON it stops writing the wav file.
	if a stop_trig is not specified it will keep writing until it
	recieves another write_trig, or the program exits.
	the name of the output files will be basenamennn where nnn is
	a number between 1 and 9999.  No, surely not, you want to 
	automatically write more wavs than that? are you mad?  Go change 
	the code yourself then!
*/
class serialwavfileout: public synthmod 
{
 public:
	serialwavfileout(string uname);
	~serialwavfileout();
	// inputs
	void set_input_bar_trig(const STATUS* bt){ in_bar_trig = bt;}
	void set_input_bar(const short* b){ in_bar = b;}
	void set_input_write_trig(const STATUS* wt){in_write_trig = wt;}
	void set_input_stop_trig(const STATUS* st){in_stop_trig = st;}
	void set_input_left_channel(const short * lc) { in_left_channel = lc; }
	void set_input_right_channel(const short * rc) { in_right_channel = rc; }
	const STATUS* get_input_bar_trig(){ return in_bar_trig;}
	const short* get_input_bar(){ return in_bar;}
	const STATUS* get_input_write_trig(){return in_write_trig;}
	const STATUS* get_input_stop_trig(){return in_stop_trig;}
	const short* get_input_left_channel() { return in_left_channel; }
	const short* get_input_right_channel() { return in_right_channel; }
	// outputs
	const STATUS* get_output_write_status() { return &write_status; }
	// params
	void set_start_bar(short sb) { start_bar = sb;}
	void set_end_bar(short eb) { end_bar = eb;}
	void set_wav_basename(char * fname);
	short get_start_bar() { return start_bar; }
	short get_end_bar() { return end_bar; }
	char * get_wav_basename() { return wav_basename; }
	// virtual funcs
	void run();
	void init(){};
	#ifndef BARE_MODULES
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	#endif
 private:
	 // inputs
	const short* in_left_channel;
	const short* in_right_channel;
 	const short* in_bar;
 	const STATUS* in_bar_trig;
 	const STATUS* in_write_trig;
 	const STATUS* in_stop_trig;
 	// outputs
	STATUS write_status;
 	// params
	short start_bar;
	short end_bar;
	// other/working
	char * wav_basename;
	char* wavfilename;
 	FILE * fileout;
 	short wavcount;
	STATUS in_write_region; // no it's not an input
	wavheader * header;
	WAV_STATUS status;
	stereodata * st_buffer;
	unsigned long sample_total;
	unsigned int buff_pos;
	void write_wav_at(stereodata * buf, unsigned long smp);
	void write_wav_chunk(stereodata * buf, unsigned long smp, int bsize);
	// other
	WAV_STATUS open_wav(char * fname);
	void close_wav();
	void write_wav_header(unsigned long length);
	// synthmod stuff
	static short serialwavfileout_count;
	#ifndef BARE_MODULES
	static void create_params();
	static bool done_params;
	#endif
};

#endif
