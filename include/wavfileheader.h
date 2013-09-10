#ifndef WAVHEADER_H
#define WAVHEADER_H

#define WAV_BUFFER_SIZE 2048

enum WAV_WRITEHEAD
{
	WAV_WH_OK = 0,
	WAV_WH_CHERR = -1,
	WAV_WH_RESERR = -2
};

enum WAV_MODES	 
{ 
	WAV_MODE_ERR = -1, 
	WAV_MODE_IN = 1, 
	WAV_MODE_OUT = 2	
};

enum WAV_STATUS 
{
	WAV_STATUS_MEMERR = -6,
	WAV_STATUS_OPENERR = -5,
	WAV_STATUS_WAVERR = -4,
	WAV_STATUS_MODE_ERR = -3,
	WAV_STATUS_NOT_FOUND = -2,
	WAV_STATUS_INIT = -1,
	WAV_STATUS_OK = 0,
	WAV_STATUS_OPEN = 1
};

enum WAV_CHANNELS	
{ 	
	WAV_CH_UNKNOWN = 0,
	WAV_CH_MONO = 1,
	WAV_CH_STEREO = 2	
};

enum WAV_BITRATE
{	
	WAV_BIT_8 = 8,		
	WAV_BIT_16 = 16	
};

struct stereodata
{
	short left;
	short right;
};

class wavheader
{
 public:
	wavheader(){};
	wavheader(short channels, unsigned long samplerate, short resolution);
	~wavheader(){}
	void SetChannels(short ch) { itsChannels = ch; }
	void SetSampleRate(unsigned long sr) { itsSampleRate = sr; }
	void SetResolution(short res) { itsResolution = res; }
	WAV_WRITEHEAD SetReady(unsigned long &length);
	unsigned long	SecondsToSamples(double &secs) { return (unsigned long) (itsSampleRate * secs); }
	unsigned long	GetSampleRate() { return itsSampleRate; }
	unsigned long	GetSampleLength() { return itsSampleLength / itsBlockAlign; }
	short GetChannels() { return itsChannels; }
	short GetResolution() { return itsResolution; }
	char GetBlockAlign() { return itsBlockAlign; }
 private:
	unsigned char itsText1[4]; //riff
	unsigned long itsLength;	  //36 (noof bytes before itsText3[4] aparently)
	unsigned char itsText2[4]; //type name
	unsigned char itsData1[4]; // format name
	unsigned long its_length;	  // sixteen what?
	unsigned short format_tag; //1 is pcm
	short itsChannels;
	unsigned long	itsSampleRate;
	unsigned long	itsBlockSampleRate;
	unsigned short	itsBlockAlign;
	short itsResolution;
	unsigned char	itsText3[4];
	unsigned long	itsSampleLength;
};

#endif // WAVHEADER_H
