#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <sndfile.h>
#include "types.h"

enum DATA_FMT
{
    FMT_PCM16,
    FMT_PCM24,
    FMT_PCM32,
    FMT_FLT32,
    FMT_FLT64
};

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
    WAV_BIT_OTHER = 0,
    WAV_BIT_8 = 8,
    WAV_BIT_16 = 16
};

struct st_data
{
    double left;
    double right;
};

SNDFILE*    open_wav_write(const char*, DATA_FMT);

#endif // WAVHEADER_H
