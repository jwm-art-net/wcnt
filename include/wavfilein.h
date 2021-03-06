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

class wavfilein : public dobj::base
{
 public:
    wavfilein();
    ~wavfilein();
    void set_wav_filename(const char* filename);
    WAV_STATUS open_wav();
    void set_root_note(const char * rn);
    WAV_STATUS get_status() const{ return status; }
    WAV_CHANNELS get_channel_status() const;
    #ifdef UNUSED // usage of libsamplerate makes bitrate transparent
    WAV_BITRATE get_bitrate() const; /* FIXME: used?? */
    #endif
    samp_t get_length() const;
    samp_t get_sample_rate() const;
    void read_wav_at(void* buf, samp_t smp);
    void read_wav_chunk(void* buf, samp_t smp, int bsize);
    const char* get_filename() const { return fname; }
    const char* get_root_note() const { return rootnote;}
    double get_root_phase_step() const;
    // virtuals from dobj
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    virtual errors::TYPE validate();

 private:
    char* fname;
    char* rootnote;
    SNDFILE* filein;
    SF_INFO sfinfo;
    WAV_STATUS status;
    void register_ui();
    ui::dobjitem_list* get_ui_items();
};

#endif
