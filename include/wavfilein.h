#ifndef WAVFILEIN_H
#define WAVFILEIN_H

#include <sndfile.h>

#include "dobj.h"
#include "wavfileheader.h"

/*
//  filename   == name (and path) of the file to be opened
//  samplename == name wcnt will use to refer to above
//  instead of:(old style)
//  wavfilein samples/percuss/alsshat01.wav
//  sampler::wavfilein samples/percuss/alsshat01.wav
//  
//  use this:  (new style)
//  wavfilein
//  hat
//   file samples/percuss/alsshat01.wav
//  hat
//  sampler::wavfilein hat
*/

class wavfilein : public dobj
{
public:
    wavfilein();
    ~wavfilein();
    void set_wav_filename(const char* filename);
    WAV_STATUS open_wav();
    void set_root_note(const char * rn);
    WAV_STATUS get_status(){ return status; }
    WAV_CHANNELS get_channel_status();
    WAV_BITRATE get_bitrate();
    unsigned long get_length();
    unsigned long get_sample_rate();
    void read_wav_at(void* buf, unsigned long smp);
    void read_wav_chunk(void* buf, unsigned long smp, int bsize);
    const char* get_filename() { return fname; }
    const char* get_root_note() { return rootnote;}
    double get_root_phase_step();
    // virtuals from dobj
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE) const;
    virtual stockerrs::ERR_TYPE validate();

private:
    char* fname;
    char* rootnote;
    SNDFILE* filein;
    SF_INFO sfinfo;
    WAV_STATUS status;
    void init_first();
    
};

#endif
