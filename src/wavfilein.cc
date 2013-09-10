#ifndef WAVFILEIN_H
#include "../include/wavfilein.h"

wavfilein::wavfilein() :
	dobj(dobjnames::SDEF_WAVFILEIN), fname(0), rootnote(0), filein(0), 
	header(0), status(WAV_STATUS_INIT)
{
	header = new wavheader;
	#ifndef BARE_MODULES
	create_dparams();
	#endif
}

wavfilein::~wavfilein() 
{
	delete header;
	if (fname)
		delete fname;
	if (status == WAV_STATUS_OPEN) 
		fclose(filein);
	if (rootnote)
		delete [] rootnote;
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

void wavfilein::set_root_note(char* rn)
{
	if (rootnote) delete rootnote;
	rootnote = new char[NOTE_NAME_SIZE];
	strncpy(rootnote, rn, NOTE_NAME_SIZE - 1);
	rootnote[NOTE_NAME_SIZE - 1] = '\0';
}

double wavfilein::get_root_deg_size()
{
	return note_to_step(rootnote, 0);
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

#ifndef BARE_MODULES

bool wavfilein::set_dparam(dparnames::DPAR_TYPE dt, void* data)
{
	bool retv = false;
	switch(dt)
	{
		case dparnames::DPAR_FILENAME:
			open_wav((char*)data); // pass pointer
			retv = true;
			break;
		case dparnames::DPAR_ROOTNOTE:
			set_root_note((char*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void* wavfilein::get_dparam(dparnames::DPAR_TYPE dt)
{
	void* retv = 0;
	switch(dt)
	{
		case dparnames::DPAR_FILENAME:
			retv = new char[strlen(fname) + 1];
		    strncpy((char*)retv, fname, strlen(fname));
    		((char*)retv)[strlen(fname) + 1] = '\0';
			break;
		default:
			retv = 0;
	}
	return retv;
}

bool wavfilein::validate()
{
	if (status != WAV_STATUS_OPEN) {
		*err_msg = "\ncould not open ";
		*err_msg += fname;
		*err_msg += " for reading";
		invalidate();
	}
	if (!check_notename(rootnote)) {
		*err_msg += "\ninvalid note name set for " +
			dobj::get_dparnames()->get_name(dparnames::DPAR_ROOTNOTE);
		invalidate();
	}
	return is_valid();
}

void wavfilein::create_dparams()
{
	if (done_dparams == true)	return;
	get_dobjparamlist()->add_dobjparam(dobjnames::SDEF_WAVFILEIN, dparnames::DPAR_FILENAME);
	get_dobjparamlist()->add_dobjparam(dobjnames::SDEF_WAVFILEIN, dparnames::DPAR_ROOTNOTE);
	done_dparams = true;
}

bool wavfilein::done_dparams = false;

#endif

#endif
