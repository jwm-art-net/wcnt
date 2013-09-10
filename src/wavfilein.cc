#ifndef WAVFILEIN_H
#include "../include/wavfilein.h"

wavfilein::wavfilein()
:fname(NULL), sname(NULL), filein(NULL), header(NULL), status(WAV_STATUS_INIT)
{
	header = new wavheader;
}

wavfilein::~wavfilein() 
{
	delete header;
	if (fname)
		delete fname;
	if (sname)
		delete sname;
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

WAV_BITRATE wavfilein::get_bitrate()
{
	if (header->GetResolution() == 8)
		return WAV_BIT_8;
	else if (header->GetResolution() == 16)
		return WAV_BIT_16;
	else 
		return WAV_BIT_OTHER;
}

WAV_STATUS wavfilein::open_wav(const char * filename)
{
	if (status == WAV_STATUS_MEMERR) 
		return status;
	if (status == WAV_STATUS_OPEN)
		fclose(filein);
	if (fname) 
		delete fname;
	fname = new char[strlen(filename)+1];
	strncpy(fname, filename, strlen(filename));
	fname[strlen(filename)] = '\0';
	if ((filein = fopen(filename, "rb")) == NULL)
	{
		return status = WAV_STATUS_NOT_FOUND;
	}
	fseek(filein, 0, SEEK_SET);
	fread(header, sizeof(wavheader), 1, filein);
	char* chp = 0;
	chp = header->get_riff_name();
	string riffname = "RIFF";
	if (riffname != chp) {
		fclose(filein);
		delete chp;
		return status = WAV_STATUS_WAVERR;
	}
	delete chp;
	chp = header->get_type_name();
	string wavtypename = "WAVE";
	if (wavtypename != chp) {
		fclose(filein);
		delete chp;
		return status = WAV_STATUS_WAVERR;
	}
	delete chp;
	chp = header->get_format_name();
	string formatname = "fmt ";
	if (formatname != chp){
		fclose(filein);
		delete chp;
		return status = WAV_STATUS_WAVERR;
	}
	delete chp;
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

WAV_STATUS wavfilein::set_sample_name(const char* sn)
{
	if (sname) 
		delete sname;
	if ((sname = new char[strlen(sn)+1]) == NULL) 
		return status = WAV_STATUS_MEMERR;
	strncpy(sname, sn, strlen(sn));
	sname[strlen(sn)] = '\0';
	return WAV_STATUS_OK;
}
#endif
