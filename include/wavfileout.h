#ifndef WAVFILEOUT_H
#define WAVFILEOUT_H

#include <stdio.h>

#include "wavfileheader.h"
#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class wavfileout: public synthmod
{
public:
    wavfileout(char const*);
    ~wavfileout();
    // inputs
    void set_input_bar_trig(const STATUS* bt){ in_bar_trig = bt;}
    void set_input_bar(const short* b){ in_bar = b;}
    void set_input_left_channel(const short * lc) {
        in_left_channel = lc;
    }
    void set_input_right_channel(const short * rc) {
        in_right_channel = rc;
    }
    const STATUS* get_input_bar_trig(){ return in_bar_trig;}
    const short* get_input_bar(){ return in_bar;}
    const short* get_input_left_channel() { return in_left_channel; }
    const short* get_input_right_channel() { return in_right_channel; }
    // outputs
    const STATUS* get_output_write_status() {
        return &write_status; 
    }
    const STATUS* get_output_write_start_trig() {
        return &out_write_start_trig;
    }
    const STATUS* get_output_write_end_trig() {
        return &out_write_end_trig;
    }
    // params
    void set_start_bar(short sb) { start_bar = sb;}
    void set_end_bar(short eb) { end_bar = eb;}
    WAV_STATUS open_wav(char * fname);
    short get_start_bar() { return start_bar; }
    short get_end_bar() { return end_bar; }
    char * get_wav_filename() { return filename; }
    // other
    WAV_STATUS get_status() { return status; }
    void close_wav();
    void write_wav_header(unsigned long length);
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif
private:
    // inputs
    const short* in_left_channel;
    const short* in_right_channel;
    const short* in_bar;
    const STATUS* in_bar_trig;
    // outputs
    STATUS out_write_start_trig;
    STATUS out_write_end_trig;
    STATUS write_status;
    // params
    short start_bar;
    short end_bar;
    // other/working
    char * filename;
    FILE * fileout;
    wavheader * header;
    WAV_STATUS status;
    stereodata * st_buffer;
    unsigned long sample_total;
    unsigned int buff_pos;
    void write_wav_at(stereodata * buf, unsigned long smp);
    void write_wav_chunk(stereodata * buf, unsigned long smp, int bsize);
    // synthmod stuff
    static short wavfileout_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
