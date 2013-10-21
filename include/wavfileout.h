#ifndef WAVFILEOUT_H
#define WAVFILEOUT_H

#include <sndfile.h>

#include "synthmod.h"
#include "wavfileheader.h"

class wavfileout: public synthmod::base
{
public:
    wavfileout(const char*);
    ~wavfileout();

    // copies filenames & processes path:
    void set_wav_filename(char* fname);
    WAV_STATUS open_wav();
    WAV_STATUS get_status() const { return status; }
    void close_wav();

    // virtual funcs
    void run();
    errors::TYPE validate(); // opens the wav
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const double* in_l;
    const double* in_r;
    const wcint_t* in_bar;

    // outputs
    STATUS out_write_start_trig;
    STATUS out_write_end_trig;
    STATUS write_status;

    // params
    STATUS snapshot_mode;
    DATA_FMT data_format;
    wcint_t start_bar;
    wcint_t end_bar;
    char* filename;

    // other/working
    char* _filename;
    SNDFILE * fileout;
    WAV_STATUS status;
    st_data * st_buffer;
    samp_t sample_total;
    wcint_t buff_pos;
    void write_wav_at(st_data* buf, samp_t smp);
    void write_wav_chunk(st_data* buf, samp_t smp, int bsize);
    void timestamp_filename();
    int state;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif // WAVFILEOUT
