#ifndef WAVFILEOUT_H
#define WAVFILEOUT_H

#include <stdio.h>

#include "wavfileheader.h"
#include "conversions.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class wavfileout: public synthmod 
{
 public:
	wavfileout(string uname);
	~wavfileout();
	WAV_STATUS open_wav(char * fname);
	WAV_STATUS get_status() { return status; }
	void close_wav();
	void set_write_from_bar(short);
	void set_write_to_bar(short);
	void set_write_from_sample(unsigned long);
	void set_write_to_sample(unsigned long);
	void set_left_channel(const short * lc) { left_channel = lc; }
	void set_right_channel(const short * rc) { right_channel = rc; }
	char * get_name() { return filename; }
	short get_start_bar() { return start_bar; }
	short get_end_bar() { return end_bar; }
	const short* get_left_channel() { return left_channel; }
	const short* get_right_channel() { return right_channel; }
	void write_wav_header(unsigned long length);
	const STATUS* get_output_write_status() { return &write_status; }
	const STATUS* get_output_write_start_trig() { return &write_start_trigger_out; }
	const STATUS* get_output_write_end_trig() { return &write_end_trigger_out; }
	// virtual funcs
	void run();
	void init(){};
	void const* get_out(outputnames::OUT_TYPE);
	void const* set_in(inputnames::IN_TYPE, void const*);
	bool set_param(paramnames::PAR_TYPE, void const*);
	
 private:
	STATUS write_status;
	STATUS write_start_trigger_out;
	STATUS write_end_trigger_out;
	char * filename;
	FILE * fileout;
	wavheader * header;
	WAV_STATUS status;
	const short * left_channel;
	const short * right_channel;
	stereodata * st_buffer;
	short start_bar;
	short end_bar;
	unsigned long sample;
	unsigned long start_sample;
	unsigned long end_sample;
	unsigned int buff_pos;
	void write_wav_at(stereodata * buf, unsigned long smp);
	void write_wav_chunk(stereodata * buf, unsigned long smp, int bsize);
	static short wavfileout_count;
	static void create_params();
	static bool done_params;
};

#endif
