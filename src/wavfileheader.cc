#include "../include/wavfileheader.h"
#include "../include/jwm_init.h"
#include "../include/jwm_globals.h"


SNDFILE* open_wav_write(const char* filename, DATA_FMT data_format)
{
    SF_INFO sfinfo;
    sfinfo.frames = 0;
    sfinfo.samplerate = jwm.samplerate();
    sfinfo.channels = 2;
    int fmt = 0;
    switch(data_format){
        case FMT_PCM16: fmt = SF_FORMAT_PCM_16; break;
        case FMT_PCM24: fmt = SF_FORMAT_PCM_24; break;
        case FMT_PCM32: fmt = SF_FORMAT_PCM_32; break;
        case FMT_FLT32: fmt = SF_FORMAT_FLOAT;  break;
        case FMT_FLT64: fmt = SF_FORMAT_DOUBLE; break;
        default:
            fmt = SF_FORMAT_PCM_16;
    }
    sfinfo.format = SF_FORMAT_WAV | fmt;
    sfinfo.sections = 0;
    sfinfo.seekable = 0;

    return sf_open(filename, SFM_WRITE, &sfinfo);
}
