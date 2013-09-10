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
