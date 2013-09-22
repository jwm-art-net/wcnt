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
#include <sstream>
#include <sys/timeb.h>

wavfileout::wavfileout(char const* uname) :
 synthmod(synthmodnames::WAVFILEOUT, uname, SM_DEFAULT),
 in_l(0), in_r(0), in_bar(0), in_bar_trig(0),
 out_write_start_trig(OFF), out_write_end_trig(OFF),
 write_status(OFF),
 snapshot_mode(OFF), data_format(FMT_FLT32),
 start_bar(0), end_bar(0),
 _filename(0), filename(0),
 fileout(0), //header(0),
 status(WAV_STATUS_INIT), st_buffer(0), sample_total(0), buff_pos(0)
{
    register_input(inputnames::IN_LEFT);
    register_input(inputnames::IN_RIGHT);
    register_input(inputnames::IN_BAR);
    register_input(inputnames::IN_BAR_TRIG);
    register_output(outputnames::OUT_WRITE_START_TRIG);
    register_output(outputnames::OUT_WRITE_END_TRIG);
    register_output(outputnames::OUT_WRITE_STATE);
    st_buffer = new st_data[jwm_init::wav_buffer_size];
    for(short i = 0; i < jwm_init::wav_buffer_size; i++){
        st_buffer[i].left = 0;
        st_buffer[i].right = 0;
    }
    status = WAV_STATUS_OK;
    init_first();
}

wavfileout::~wavfileout()
{
    close_wav();
    if (_filename && _filename != filename)
        delete [] _filename;
    if (filename)
        delete [] filename;
    delete [] st_buffer;
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
        case paramnames::SNAPSHOT_MODE:
            snapshot_mode = *(STATUS*)data;
            return true;
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
        case paramnames::SNAPSHOT_MODE: return &snapshot_mode;
        case paramnames::DATA_FMT:      return &data_format;
        case paramnames::FILENAME:      return filename;
        case paramnames::START_BAR:     return &start_bar;
        case paramnames::END_BAR:       return &end_bar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE wavfileout::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::START_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", paramnames::get_name(paramnames::START_BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (end_bar <= start_bar) {
        sm_err("%s should be after %s.",
                    paramnames::get_name(paramnames::END_BAR),
                    paramnames::get_name(paramnames::START_BAR));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wavfileout::set_wav_filename(char* fname)
{
    char const* path = jwm.path();
    if (_filename)
        delete [] _filename;
    if (*fname == '/' || path == NULL) {
        _filename = new char[strlen(fname)+1];
        strcpy(_filename, fname);
    }
    else {
        char* ptr;
        _filename = new char[strlen(fname) + strlen(path) + 1];
        strcpy(_filename, path);
        ptr = _filename + strlen(path);
        strcpy(ptr, fname);
    }
    filename = _filename;
}

void wavfileout::timestamp_filename()
{
    if (filename && filename != _filename)
        delete [] filename;
    char timestr[30];
    timeb now;
    ftime(&now);
    tm* l_time = localtime(&now.time);
    strftime(timestr, 30, "-%Y%m%d-%H%M%S-", l_time);
    std::ostringstream conv;
    conv.width(3);
    conv.fill('0');
    conv << now.millitm;
    strcat(timestr, conv.str().c_str());
    int newlen = strlen(timestr) + strlen(_filename);
    char* newname = new char[newlen + 1];
    char* dp = strrchr(_filename, '.');
    *dp = '\0';
    strcpy(newname, _filename);
    strcat(newname, timestr);
    *dp = '.';
    strcat(newname, dp);
    filename = newname;
}

WAV_STATUS wavfileout::open_wav()
{
    if (snapshot_mode == ON)
        timestamp_filename();
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
        if (status == WAV_STATUS_OPEN)
            close_wav();
        if ((fileout = sf_open(filename, SFM_WRITE, &sfinfo)) == NULL)
            return status = WAV_STATUS_OPENERR;
    }
    return status = WAV_STATUS_OPEN;
}


void wavfileout::close_wav()
{
    if (status == WAV_STATUS_OPEN) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer, sample_total-buff_pos, buff_pos);
        if (!jwm.is_dont_run()) {
            std::cout << "\nFinished writing to " << filename;
            sf_close(fileout);
        }
        status = WAV_STATUS_OK;
        write_status = OFF;
    }
}

void wavfileout::run()
{
    if (*in_bar_trig == ON) {
        if (*in_bar == start_bar) {
            if (open_wav() != WAV_STATUS_OPEN) {
                std::cout << "\nCould not open WAV file " << filename
                    << " for writing. Abort forced...";
                synthmod::force_abort();
                invalidate();
                return;
            }
            buff_pos = 0;
            sample_total = 0;
            write_status = ON;
            out_write_start_trig = ON;
        }
    }
    else if(out_write_start_trig == ON)
        out_write_start_trig = OFF;
    if (write_status == ON) {
        sample_total++;
        st_buffer[buff_pos].left = *in_l;
        st_buffer[buff_pos].right = *in_r;
        buff_pos++;
        if (buff_pos == jwm_init::wav_buffer_size) {
            write_wav_at(
                st_buffer, sample_total - jwm_init::wav_buffer_size);
            buff_pos = 0;
        }
        if (*in_bar == end_bar) {
            if (buff_pos > 0)
                write_wav_chunk(
                    st_buffer, sample_total - buff_pos - 1, buff_pos);
            close_wav();
            write_status = OFF;
            out_write_end_trig = ON;
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

void wavfileout::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::SNAPSHOT_MODE);
    register_param(paramnames::DATA_FMT, "pcm16/pcm24/pcm32/float32/float64");
    register_param(paramnames::FILENAME);
    register_param(paramnames::START_BAR);
    register_param(paramnames::END_BAR);
}

