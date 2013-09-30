#ifndef SERIALWAVFILEOUT_H
#define SERIALWAVFILEOUT_H

#include <sndfile.h>

#include "synthmod.h"
#include "wavfileheader.h"

/*  writes multiple wav files.

starts writing in between start bar and end bar.
each time write_trig is ON it starts writing a new wav file.
each time stop_trig is ON it stops writing the wav file.
if a stop_trig is not specified it will keep writing until it
recieves another write_trig, or the program exits.
the name of the output files will be basenamennn where nnn is
a number between 1 and 9999.  No, surely not, you want to 
automatically write more wavs than that? are you mad?  Go change 
the code yourself then!                             */

class serialwavfileout: public synthmod
{
public:
    serialwavfileout(const char*);
    ~serialwavfileout();
    void set_wav_basename(const char* fname);
    // virtual funcs
    void run();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const short* in_left_channel;
    const short* in_right_channel;
    const short* in_bar;
    const STATUS* in_bar_trig;
    const STATUS* in_write_trig;
    const STATUS* in_stop_trig;
    // outputs
    STATUS write_status;
    // params
    DATA_FMT data_format;
    short start_bar;
    short end_bar;
    // other/working
    char* wav_basename;
    char* wavfilename;
    SNDFILE* fileout;
    SF_INFO sfinfo;
    short wavcount;
    STATUS in_write_region; // no it's not an input
    WAV_STATUS status;
    stereodata* st_buffer;
    unsigned long sample_total;
    unsigned short buff_pos;
    void write_wav_at(stereodata* buf, unsigned long smp);
    void write_wav_chunk(stereodata* buf, unsigned long smp, int bsize);
    // other
    WAV_STATUS open_wav(const char* fname);
    void close_wav();
    // synthmod stuff
    void init_first();
    
};

#endif
