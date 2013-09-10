#ifndef WAVFILEIN_H
#include "../include/wavfilein.h"

wavfilein::wavfilein()
:fname(NULL), filein(NULL), header(NULL), status(WAV_STATUS_INIT)
{
	if ((header = new wavheader) == NULL)
		status = WAV_STATUS_MEMERR;
}

wavfilein::~wavfilein() 
{
	if (header) 
		delete header;
	if (fname)
		delete fname;
	if (status == WAV_STATUS_OPEN) 
		fclose(filein);
}

WAV_CHANNELS wavfilein::get_channel_status() 
{
	if (status == WAV_STATUS_OPEN)
		if (header->GetChannels() == 1) 
			return WAV_CH_MONO;
		else 
			return WAV_CH_STEREO;
	else
		return WAV_CH_UNKNOWN;
}

WAV_STATUS wavfilein::open_wav(const char * filename)
{
	if (status == WAV_STATUS_MEMERR) 
		return status;
	if (status == WAV_STATUS_OPEN)
		fclose(filein);
	if (fname) 
		delete fname;
	if ((fname = new char[strlen(filename)+1]) == NULL) 
		return status = WAV_STATUS_MEMERR;
	strncpy(fname, filename, strlen(filename));
	fname[strlen(filename)] = '\0';
	if ((filein = fopen(filename, "rb")) == NULL)
	{
		return status = WAV_STATUS_NOT_FOUND;
	}
	fseek(filein, 0, SEEK_SET);
	fread(header, sizeof(wavheader), 1, filein);
	/* I won't bother doing any header checks - sample & bit rates are ignored!*/
	return status = WAV_STATUS_OPEN;
}

void wavfilein::read_wav_at(void * buf, unsigned long smp) 
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(filein, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
		if (header->GetChannels() == 1)
			fread((short*)buf, sizeof(short), WAV_BUFFER_SIZE, filein);
		else 
			fread((stereodata*) buf, sizeof(stereodata), WAV_BUFFER_SIZE, filein);
	}
}

void wavfilein::read_wav_chunk(void * buf, unsigned long smp, int bsize) 
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(filein, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
		if (header->GetChannels() == 1)
			fread((short*)buf, sizeof(short), bsize, filein);
		else 
			fread((stereodata*)buf, sizeof(stereodata), bsize, filein);
		}
}
#endif
