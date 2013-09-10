#ifndef WAVFILEIN_H
#define WAVFILEIN_H

#include <stdio.h>/*FILE*/
#include <string.h> /*strncpy*/
#include "../include/wavfileheader.h"

class wavfilein 
{
 public:
	wavfilein();
	~wavfilein();
	WAV_STATUS open_wav(const char* filename);
	WAV_STATUS get_status(){ return status; }
	WAV_CHANNELS get_channel_status();
	unsigned long get_length(){ return header->GetSampleLength();}
	unsigned long get_sample_rate(){ return header->GetSampleRate();}
	void read_wav_at(void* buf, unsigned long smp);
	void read_wav_chunk(void* buf, unsigned long smp, int bsize);
	const char* get_name() { return fname; }
 private:
	char* fname;
	FILE* filein;
	wavheader* header;
	WAV_STATUS 	status;
};

#endif
