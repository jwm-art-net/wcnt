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

class serialwavfileout: public synthmod::base
{
public:
    serialwavfileout(const char*);
    ~serialwavfileout();
    void set_wav_basename(const char* fname);
    // virtual funcs
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    // inputs
    const double* in_l;
    const double* in_r;
    const wcint_t* in_bar;
    const STATUS* in_write_trig;
    const STATUS* in_stop_trig;
    // outputs
    STATUS write_status;
    // params
    DATA_FMT data_format;
    wcint_t start_bar;
    wcint_t end_bar;
    char* wav_basename;
    wcint_t count;
    // other/working
    char* filepath;    /* path + wav_basename */
    char* wavfilename;  /* _basename + nnnn + .wav */
    SNDFILE* fileout;
    wcint_t wavcount;
    STATUS in_write_region; // no it's not an input
    WAV_STATUS status;
    st_data* st_buffer;
    samp_t sample_total;
    int state;
    wcint_t buff_pos;
    void write_wav_at(st_data* buf, samp_t smp);
    void write_wav_chunk(st_data* buf, samp_t smp, int bsize);
    // other
    WAV_STATUS open_wav(const char* fname);
    void close_wav();
    // synthmod stuff
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
