#ifndef WAVFILEOUT_H
#include "../include/wavfileout.h"
#include "../include/jwm_globals.h"
#include "../include/jwm_init.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"
#include "../include/dobjparamlist.h"
#include "../include/fxsparamlist.h"

#include <iostream>

wavfileout::wavfileout(char const* uname) :
 synthmod(synthmodnames::WAVFILEOUT, uname),
 in_l(0), in_r(0), in_bar(0), in_bar_trig(0),
 out_write_start_trig(OFF), out_write_end_trig(OFF), write_status(OFF),
 data_format(FMT_FLT32), start_bar(0), end_bar(0), filename(0),
 fileout(0), //header(0),
 status(WAV_STATUS_INIT), st_buffer(0), sample_total(0), buff_pos(0)
{
    jwm.get_inputlist().add_input(this, inputnames::IN_LEFT);
    jwm.get_inputlist().add_input(this, inputnames::IN_RIGHT);
    jwm.get_inputlist().add_input(this, inputnames::IN_BAR);
    jwm.get_inputlist().add_input(this, inputnames::IN_BAR_TRIG);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_WRITE_START_TRIG);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_WRITE_END_TRIG);
    jwm.get_outputlist().add_output(this,
                                    outputnames::OUT_WRITE_STATE);
    st_buffer = new st_data[jwm_init::wav_buffer_size];
    for(short i = 0; i < jwm_init::wav_buffer_size; i++){
        st_buffer[i].left = 0;
        st_buffer[i].right = 0;
    }
    status = WAV_STATUS_OK;
    create_params();
}

wavfileout::~wavfileout()
{
    if (write_status == ON) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer, sample_total-buff_pos, buff_pos);
    }
    close_wav();
    if (filename) delete [] filename;
    delete [] st_buffer;
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* wavfileout::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_WRITE_START_TRIG:
            return &out_write_start_trig;
        case outputnames::OUT_WRITE_END_TRIG:
            return &out_write_end_trig;
        case outputnames::OUT_WRITE_STATE:
            return &write_status;
        default:
            return 0;
    }
}

void const* wavfileout::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:       return in_l = (double*)o;
        case inputnames::IN_RIGHT:      return in_r = (double*)o;
        case inputnames::IN_BAR:        return in_bar = (short*)o;
        case inputnames::IN_BAR_TRIG:   return in_bar_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* wavfileout::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:       return in_l;
        case inputnames::IN_RIGHT:      return in_r;
        case inputnames::IN_BAR:        return in_bar;
        case inputnames::IN_BAR_TRIG:   return in_bar_trig;
        default: return 0;
    }
}

bool wavfileout::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DATA_FMT:
            data_format = *(DATA_FMT*)data;
            return true;
        case paramnames::FILENAME:
            set_wav_filename((char*)data);
            return true;
        case paramnames::START_BAR:
            start_bar = *(short*)data;
            return true;
        case paramnames::END_BAR:
            end_bar = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* wavfileout::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DATA_FMT:  return &data_format;
        case paramnames::FILENAME:  return filename;
        case paramnames::START_BAR: return &start_bar;
        case paramnames::END_BAR:   return &end_bar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE wavfileout::validate()
{
    if (open_wav() != WAV_STATUS_OPEN) {
        *err_msg = "could not open ";
        *err_msg += filename;
        *err_msg += " for output.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist().validate(this, paramnames::START_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames().get_name(paramnames::START_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (end_bar <= start_bar) {
        *err_msg += jwm.get_paramnames().get_name(paramnames::END_BAR);
        *err_msg += " should be after ";
        *err_msg += jwm.get_paramnames().get_name(paramnames::START_BAR);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wavfileout::set_wav_filename(char* fname)
{
    char const* path = jwm.path();
    if (filename)
        delete [] filename;
    if (*fname == '/' || path == NULL) {
        filename = new char[strlen(fname)+1];
        strcpy(filename, fname);
    }
    else {
        char* ptr;
        filename = new char[strlen(fname) + strlen(path) + 1];
        strcpy(filename, path);
        ptr = filename + strlen(path);
        strcpy(ptr, fname);
    }
}

WAV_STATUS wavfileout::open_wav()
{
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
    if (!jwm.is_dont_run()) {
        if ((fileout = sf_open(filename, SFM_WRITE, &sfinfo)) == NULL)
            return status = WAV_STATUS_OPENERR;
    }
    return status = WAV_STATUS_OPEN;
}


void wavfileout::close_wav()
{
    if (status == WAV_STATUS_OPEN) {
        if (!jwm.is_dont_run()) {
            std::cout << "\nFinished writing to " << filename;
            sf_close(fileout);
        }
        status = WAV_STATUS_OK;
    }
}

void wavfileout::run()
{
    if (write_status == OFF) {
        if (*in_bar == start_bar) {
            write_status = ON;
            out_write_start_trig = ON;
        }
    }
    else if(out_write_start_trig == ON)
        out_write_start_trig = OFF;
    if (write_status == ON) {
        sample_total++;
        if (*in_bar == end_bar) {
            if (buff_pos > 0)
                write_wav_chunk(st_buffer, sample_total - buff_pos - 1,
                 buff_pos);
            close_wav();
            write_status = OFF;
            out_write_end_trig = ON;
        }
        else {
            st_buffer[buff_pos].left = *in_l;
            st_buffer[buff_pos].right = *in_r;
            buff_pos++;
            if (buff_pos == jwm_init::wav_buffer_size) {
                write_wav_at(st_buffer,
                             sample_total - jwm_init::wav_buffer_size);
                buff_pos = 0;
            }
        }
    }
    else if (out_write_end_trig == ON)
        out_write_end_trig = OFF;
}

void wavfileout::write_wav_at(st_data* buf, unsigned long smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf,
                            jwm_init::wav_buffer_size);
    }
}

void wavfileout::write_wav_chunk(
    st_data* buf, unsigned long smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, bsize);
    }
}

bool wavfileout::done_params = false;

void wavfileout::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
        synthmodnames::WAVFILEOUT, paramnames::DATA_FMT);
    jwm.get_fxsparamlist().add_param(
        "pcm16/pcm24/pcm32/float32/float64", paramnames::DATA_FMT);
    jwm.get_paramlist().add_param(
        synthmodnames::WAVFILEOUT, paramnames::FILENAME);
    jwm.get_paramlist().add_param(
        synthmodnames::WAVFILEOUT, paramnames::START_BAR);
    jwm.get_paramlist().add_param(
        synthmodnames::WAVFILEOUT, paramnames::END_BAR);

    done_params = true;
}

#endif
