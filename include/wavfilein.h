#ifndef WAVFILEIN_H
#define WAVFILEIN_H

#include <stdio.h>/*FILE*/
#include <string.h> /*strncpy*/
#include <string> /* string blah == "dit dit dit"  (char*)*/

#include "wavfileheader.h"
#include "conversions.h"
#include "dobjparamlist.h"

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

class wavfilein : public dobj
{
 public:
	wavfilein();
	~wavfilein();
	WAV_STATUS open_wav(const char* filename);
	void set_root_note(char * rn);
	WAV_STATUS get_status(){ return status; }
	WAV_CHANNELS get_channel_status();
	WAV_BITRATE get_bitrate();
	unsigned long get_length(){ return header->GetSampleLength();}
	unsigned long get_sample_rate(){ return header->GetSampleRate();}
	void read_wav_at(void* buf, unsigned long smp);
	void read_wav_chunk(void* buf, unsigned long smp, int bsize);
	const char* get_filename() { return fname; }
	const char* get_root_note() { return rootnote;}
	double get_root_deg_size();
	#ifndef BARE_MODULES
	// virtuals from dobj
	bool set_dparam(dparnames::DPAR_TYPE, void*);
	void* get_dparam(dparnames::DPAR_TYPE);
	bool validate();
	#endif
 private:
	char* fname;
 	char* rootnote;
	FILE* filein;
	wavheader* header;
	WAV_STATUS 	status;
	#ifndef BARE_MODULES
	void create_dparams();
	static bool done_dparams;
	#endif
};

#endif
