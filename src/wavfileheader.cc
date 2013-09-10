#ifndef WAVHEADER_H
#include "../include/wavfileheader.h"

wavheader::wavheader(short channels, unsigned long samplerate, short resolution)
: itsLength(36), its_length(16), format_tag(1), itsChannels(channels), itsSampleRate(samplerate), itsResolution(resolution),	
  itsSampleLength(0)
{
	itsText1[0] = 'R';
	itsText1[1] = 'I';
	itsText1[2] = 'F';
	itsText1[3] = 'F';
	// itsLength
	itsText2[0] = 'W';
	itsText2[1] = 'A';
	itsText2[2] = 'V';
	itsText2[3] = 'E';
	itsData1[0] = 'f';
	itsData1[1] = 'm';
	itsData1[2] = 't';
	itsData1[3] = ' ';
	//its_length
	//format_tag
	//itsChannels
	//itsSampleRate
	itsBlockSampleRate = itsBlockAlign * itsSampleRate;
	itsBlockAlign = (itsResolution / 8) * itsChannels;
	//itsResolution
	itsText3[0] = 'd';
	itsText3[1] = 'a';
	itsText3[2] = 't';
	itsText3[3] = 'a';
	//itsSampleLength
}

char* wavheader::get_format_name()
{
	char* fn = new char[5];
	strncpy(fn, itsData1, 4);
	fn[4] = 0;
	return fn;
}

char* wavheader::get_type_name()
{
	char* tn = new char[5];
	strncpy(tn, itsText2, 4);
	tn[4] = 0;
	return tn;
}

char* wavheader::get_riff_name()
{
	char* rn = new char[5];
	strncpy(rn, itsText1, 4);
	rn[4] = 0;
	return rn;
}

WAV_WRITEHEAD wavheader::SetReady(unsigned long &length)
{
	itsSampleLength = length * itsBlockAlign;
//	itsLength = itsSampleLength + sizeof(wavheader);
//	its_length = itsSampleLength;
	if (itsChannels < 1 || itsChannels > 2)
		return WAV_WH_CHERR;
	if (itsResolution % 8 != 0)
		return WAV_WH_RESERR;
	return WAV_WH_OK;
}

#endif
