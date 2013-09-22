#include "../include/serialwavfileout.h"
#include "../include/jwm_globals.h"
#include "../include/jwm_init.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

#include <iostream>

serialwavfileout::serialwavfileout(char const* uname) :
 synthmod(synthmodnames::SERIALWAVFILEOUT, uname, SM_DEFAULT),
 in_left_channel(0), in_right_channel(0), in_bar(0), in_bar_trig(0),
 in_write_trig(0), in_stop_trig(0), write_status(OFF),
 data_format(FMT_FLT32), start_bar(0),
 end_bar(0), wav_basename(0), wavfilename(0), fileout(0), wavcount(0),
 in_write_region(OFF), status(WAV_STATUS_INIT), 
 st_buffer(NULL), sample_total(0), buff_pos(0)
{
    register_input(inputnames::IN_LEFT);
    register_input(inputnames::IN_RIGHT);
    register_input(inputnames::IN_BAR);
    register_input(inputnames::IN_BAR_TRIG);
    register_input(inputnames::IN_WRITE_TRIG);
    register_input(inputnames::IN_STOP_TRIG);
    register_output(outputnames::OUT_WRITE_STATE);
    st_buffer = new stereodata[jwm_init::wav_buffer_size];
    for(short i = 0; i < jwm_init::wav_buffer_size; i++){
        st_buffer[i].left = 0;
        st_buffer[i].right = 0;
    }
    init_first();
}

serialwavfileout::~serialwavfileout()
{
    if (in_write_region == ON && write_status == ON) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
        close_wav();
    }
    if (status == WAV_STATUS_OPEN)
        sf_close(fileout);
    delete [] st_buffer;
    if (wav_basename) delete [] wav_basename;
}

void const* serialwavfileout::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_WRITE_STATE: return &write_status;
        default: return 0;
    }
}

void const*
serialwavfileout::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:
            return in_left_channel = (short*)o;
        case inputnames::IN_RIGHT:
            return in_right_channel = (short*)o;
        case inputnames::IN_BAR:
            return in_bar = (short*)o;
        case inputnames::IN_BAR_TRIG:
            return in_bar_trig = (STATUS*)o;
        case inputnames::IN_WRITE_TRIG:
            return in_write_trig = (STATUS*)o;
        case inputnames::IN_STOP_TRIG:
            return in_stop_trig = (STATUS*)o;
        default:
            return 0;
    }
}

void const* serialwavfileout::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:
            return in_left_channel;
        case inputnames::IN_RIGHT:
            return in_right_channel;
        case inputnames::IN_BAR:
            return in_bar;
        case inputnames::IN_BAR_TRIG:
            return in_bar_trig;
        case inputnames::IN_WRITE_TRIG:
            return in_write_trig;
        case inputnames::IN_STOP_TRIG:
            return in_stop_trig;
        default:
            return 0;
    }
}

bool
serialwavfileout::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DATA_FMT:
            data_format = *(DATA_FMT*)data;
            return true;
        case paramnames::WAV_BASENAME:
            set_wav_basename((char*)data);
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

void const* serialwavfileout::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DATA_FMT:      return &data_format;
        case paramnames::WAV_BASENAME:  return wav_basename;
        case paramnames::START_BAR:     return &start_bar;
        case paramnames::END_BAR:       return &end_bar;
        default: return 0;
    }
}

stockerrs::ERR_TYPE serialwavfileout::validate()
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

void serialwavfileout::set_wav_basename(char * fname)
{
    if (wav_basename)
        delete [] wav_basename;
    wav_basename = new char[strlen(fname) + 1];
    strcpy(wav_basename, fname);
}

WAV_STATUS serialwavfileout::open_wav(char * fname)
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
    if ((fileout = sf_open(fname, SFM_WRITE, &sfinfo)) == NULL)
        return status = WAV_STATUS_OPENERR;
    return status = WAV_STATUS_OPEN;


}

void serialwavfileout::close_wav()
{
    if (status == WAV_STATUS_OPEN)
        sf_close(fileout);
    status = WAV_STATUS_OK;
    delete [] wavfilename;
    wavfilename = 0;
}

void serialwavfileout::run()
{
    if (in_write_region == OFF) {
        if (*in_bar == start_bar && *in_bar_trig == ON)
            in_write_region = ON;
    }
    if (in_write_region == ON) {
        if (*in_bar == end_bar) {
            if (write_status == ON && buff_pos > 0) {
                write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
                close_wav();
                write_status = OFF;
            }
            in_write_region = OFF;
        }
        else if (*in_write_trig == ON) {
            if (write_status == ON) { // close current file
                if (buff_pos > 0) {
                    write_wav_chunk(
                     st_buffer,sample_total-buff_pos,buff_pos);
                }
                close_wav();
            }
            buff_pos = 0;
            sample_total = 0;
            write_status = ON;
            wavcount++;
            wavfilename = new char[strlen(wav_basename) + 8];
            sprintf(wavfilename, "%s%04d.wav", wav_basename, wavcount);
            if (open_wav(wavfilename) != WAV_STATUS_OPEN) {
                std::cout << "\n!!!Unable to create '" << wavfilename;
                std::cout << "to write from module " << get_username();
                std::cout << "!!!\n";
                delete [] wavfilename;
                write_status = OFF;
                in_write_region = OFF;
            }
        }
        if (write_status == ON) {
            sample_total++;
            if (*in_stop_trig == ON) {
                if (buff_pos > 0) {
                    write_wav_chunk(
                     st_buffer,sample_total-buff_pos,buff_pos);
                }
                close_wav();
                write_status = OFF;
            }
            else {
                st_buffer[buff_pos].left = *in_left_channel;
                st_buffer[buff_pos].right = *in_right_channel;
                buff_pos++;
                if (buff_pos == jwm_init::wav_buffer_size) {
                    write_wav_at(
                     st_buffer, sample_total - jwm_init::wav_buffer_size);
                    buff_pos = 0;
                }
            }
        } // endif (write_status == ON)
    } // endif (write_region == ON)
}

void serialwavfileout::write_wav_at(stereodata* buf, unsigned long smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf,
                            jwm_init::wav_buffer_size);
    }
}

void serialwavfileout::write_wav_chunk(
 stereodata * buf, unsigned long smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, bsize);
    }
}

void serialwavfileout::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::DATA_FMT, "pcm16/pcm24/pcm32/float32/float64");     register_param(paramnames::WAV_BASENAME);
    register_param(paramnames::START_BAR);
    register_param(paramnames::END_BAR);
}

