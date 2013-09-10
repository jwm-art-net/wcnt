#ifndef WAVFILEIN_H
#define WAVFILEIN_H

#include <stdio.h>/*FILE*/
#include <string.h> /*strncpy*/
#include <string> /* string blah == "dit dit dit"  (char*)*/

#include "../include/wavfileheader.h"

/*
	filename == 	name (and path) of the file to be opened
	samplename == 	name wcnt will use to refer to above
	
	instead of:	(old style)
	
	wavfilein samples/percuss/alsshat01.wav
	
	sampler::wavfilein samples/percuss/alsshat01.wav
	
	use this:  (new style)
	
	wavfilein 
	hat 
		file samples/percuss/alsshat01.wav 
	hat
	
	sampler::wavfilein hat
*/

class wavfilein 
{
 public:
	wavfilein();
	~wavfilein();
	WAV_STATUS open_wav(const char* filename);
	WAV_STATUS get_status(){ return status; }
	WAV_CHANNELS get_channel_status();
	WAV_BITRATE get_bitrate();
	WAV_STATUS set_sample_name(const char* sn);
	unsigned long get_length(){ return header->GetSampleLength();}
	unsigned long get_sample_rate(){ return header->GetSampleRate();}
	void read_wav_at(void* buf, unsigned long smp);
	void read_wav_chunk(void* buf, unsigned long smp, int bsize);
	const char* get_name() { return fname; }
	const char* get_sample_name() { return sname; }
 private:
	char* fname;
 	char* sname;
	FILE* filein;
	wavheader* header;
	WAV_STATUS 	status;
};

#endif
