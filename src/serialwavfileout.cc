#include "../include/serialwavfileout.h"
#include "../include/globals.h"


// for wav open errors while run()ing:
#include <iostream>


serialwavfileout::serialwavfileout(const char* uname) :
 synthmod::base(synthmod::SERIALWAVFILEOUT, uname, SM_DEFAULT),
 in_l(0), in_r(0), in_bar(0),
 in_write_trig(0), in_stop_trig(0), write_status(OFF),
 data_format(FMT_FLT32), start_bar(0),
 end_bar(0), wav_basename(0), filepath(0), wavfilename(0), fileout(0),
 wavcount(0), in_write_region(OFF), status(WAV_STATUS_INIT),
 st_buffer(NULL), sample_total(0), state(0), buff_pos(0)
{
    register_output(output::OUT_WRITE_STATE);

    st_buffer = new st_data[wcnt::wav_buffer_size];
    for(int i = 0; i < wcnt::wav_buffer_size; i++)
        st_buffer[i].left = st_buffer[i].right = 0.0f;
}

void serialwavfileout::register_ui()
{
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT);
    register_input(input::IN_BAR);
    register_input(input::IN_WRITE_TRIG);
    register_input(input::IN_STOP_TRIG);
    register_param(param::DATA_FMT,
                    "pcm16/pcm24/pcm32/float32/float64");
    register_param(param::WAV_BASENAME);
    register_param(param::START_BAR);
    register_param(param::END_BAR);
    register_param(param::COUNT);
}

serialwavfileout::~serialwavfileout()
{
    if (in_write_region == ON && write_status == ON) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer, sample_total - buff_pos, buff_pos);
        close_wav();
    }
    if (status == WAV_STATUS_OPEN)
        sf_close(fileout);
    delete [] st_buffer;
    if (wav_basename)
        delete [] wav_basename;
    if (filepath)
        delete [] filepath;
    if (wavfilename)
        delete [] wavfilename;
}

const void* serialwavfileout::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_WRITE_STATE: return &write_status;
        default: return 0;
    }
}

const void*
serialwavfileout::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_LEFT:
            return in_l = (const double*)o;
        case input::IN_RIGHT:
            return in_r = (const double*)o;
        case input::IN_BAR:
            return in_bar = (const wcint_t*)o;
        case input::IN_WRITE_TRIG:
            return in_write_trig = (STATUS*)o;
        case input::IN_STOP_TRIG:
            return in_stop_trig = (STATUS*)o;
        default:
            return 0;
    }
}

const void* serialwavfileout::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:
            return in_l;
        case input::IN_RIGHT:
            return in_r;
        case input::IN_BAR:
            return in_bar;
        case input::IN_WRITE_TRIG:
            return in_write_trig;
        case input::IN_STOP_TRIG:
            return in_stop_trig;
        default:
            return 0;
    }
}

bool
serialwavfileout::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DATA_FMT:
            data_format = *(DATA_FMT*)data;
            return true;
        case param::WAV_BASENAME:
            set_wav_basename((const char*)data);
            return true;
        case param::START_BAR:
            start_bar = *(wcint_t*)data;
            return true;
        case param::END_BAR:
            end_bar = *(wcint_t*)data;
            return true;
        case param::COUNT:
            count = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* serialwavfileout::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DATA_FMT:      return &data_format;
        case param::WAV_BASENAME:  return wav_basename;
        case param::START_BAR:     return &start_bar;
        case param::END_BAR:       return &end_bar;
        case param::COUNT:         return &count;
        default: return 0;
    }
}

errors::TYPE serialwavfileout::validate()
{
    if (!validate_param(param::START_BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (end_bar <= start_bar) {
        sm_err("%s should be after %s.",
                param::names::get(param::END_BAR),
                param::names::get(param::START_BAR));
        invalidate();
        return errors::ERROR;
    }

    if (!validate_param(param::COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

void serialwavfileout::set_wav_basename(const char* fname)
{
    const char* path = wcnt::jwm.path();
    if (wav_basename)
        delete [] wav_basename;
    if (filepath)
        delete [] filepath;
    if (wavfilename)
        delete [] wavfilename;

    filepath = wavfilename = 0;
    wav_basename = new char[strlen(fname)+1];
    strcpy(wav_basename, fname);

    if (path && *fname != '/') {
        filepath = new char[strlen(fname) + strlen(path) + 1];
        strcpy(filepath, path);
        strcat(filepath, fname);
    }
}

WAV_STATUS serialwavfileout::open_wav(const char* fname)
{
    if (!wcnt::jwm.is_dont_run()) {
        if (status == WAV_STATUS_OPEN) {
            #ifdef DEBUG
            std::cout << "why well i never..." << std::endl;
            #endif
            close_wav();
        }
        if ((fileout = open_wav_write(fname, data_format)) == 0)
            return status = WAV_STATUS_OPENERR;
    }
    return status = WAV_STATUS_OPEN;
}

void serialwavfileout::close_wav()
{
    if (status == WAV_STATUS_OPEN) {
        std::cout << "\nFinished writing to " << wavfilename;
        sf_close(fileout);
        status = WAV_STATUS_OK;
    }
}

void serialwavfileout::run()
{
    switch(state) {
    case 0: // waiting for write region
        if (*in_bar == start_bar) {
            state = 1;
            goto write_region;
        }
        break;
    case 1:
    write_region:
        if (*in_bar == end_bar) {
            state = 2;
            goto stopping;
        }
        if (*in_write_trig == ON) {
            if (write_status == ON) { // close currently open file
                if (--count <= 0)
                    goto stopping;
                if (buff_pos > 0)
                    write_wav_chunk(st_buffer, sample_total - buff_pos,
                                                                buff_pos);
                close_wav();
            }
            buff_pos = 0;
            sample_total = 0;
            write_status = ON;
            wavcount++;
            size_t slen = strlen(filepath) + 8;
            if (wavfilename)
                delete [] wavfilename;
            wavfilename = new char[slen + 1];
            snprintf(wavfilename, slen + 1, "%s%04d.wav", filepath,
                                                            wavcount);
            wavfilename[slen] = '\0';
            if (open_wav(wavfilename) != WAV_STATUS_OPEN) {
                std::cout << "\n!!!Unable to create '" << wavfilename;
                std::cout << "to write from module " << get_username();
                std::cout << "!!! Abandoning further writes!\n";
                delete [] wavfilename;
                wavfilename = 0;
                write_status = OFF;
                in_write_region = OFF;
                state = 2;
                goto stopping;
            }
        }
        sample_total++;
        if (*in_stop_trig == ON) {
            if (--count <= 0)
                goto stopping;
            if (buff_pos > 0)
                write_wav_chunk(st_buffer, sample_total - buff_pos,
                                                            buff_pos);
            close_wav();
            write_status = OFF;
        }
        else {
            st_buffer[buff_pos].left = *in_l;
            st_buffer[buff_pos].right = *in_r;
            buff_pos++;
            if (buff_pos == wcnt::wav_buffer_size) {
                write_wav_at(st_buffer, sample_total -
                                            wcnt::wav_buffer_size);
                buff_pos = 0;
            }
        }
        break;
    case 2: // stopping
    stopping:
        if (write_status == ON) {
            if (buff_pos > 0)
                write_wav_chunk(st_buffer, sample_total - buff_pos,
                                                            buff_pos);
            close_wav();
            write_status = OFF;
        }
        state = 3;
    case 3:
        break;
    }
}

void serialwavfileout::write_wav_at(st_data* buf, samp_t smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, wcnt::wav_buffer_size);
    }
}

void serialwavfileout::write_wav_chunk(st_data* buf, samp_t smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, bsize);
    }
}


