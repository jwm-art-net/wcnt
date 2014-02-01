#include "../include/wavfileout.h"
#include "../include/globals.h"
#include "../include/conversions.h"

#include <iostream>         // <-- for messages while running
#include <sys/timeb.h>      // <-- filename timestamp

wavfileout::wavfileout(const char* uname) :
 synthmod::base(synthmod::WAVFILEOUT, uname, SM_DEFAULT),
 in_l(0), in_r(0), in_bar(0),
 out_write_start_trig(OFF), out_write_end_trig(OFF),
 write_status(OFF),
 snapshot_mode(OFF), data_format(FMT_FLT32),
 start_bar(0), end_bar(0),
 filename(0), _filename(0),
 fileout(0), //header(0),
 status(WAV_STATUS_INIT), st_buffer(0), sample_total(0), buff_pos(0),
 state(0)
{
    register_output(output::OUT_WRITE_START_TRIG);
    register_output(output::OUT_WRITE_END_TRIG);
    register_output(output::OUT_WRITE_STATE);

    st_buffer = new st_data[wcnt::wav_buffer_size];
    for(int i = 0; i < wcnt::wav_buffer_size; i++){
        st_buffer[i].left = 0;
        st_buffer[i].right = 0;
    }
    status = WAV_STATUS_OK;
}

void wavfileout::register_ui()
{
    register_param(param::FILENAME);
    register_param(param::DATA_FMT,"pcm16|pcm24|pcm32|float32|float64");
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT)->set_connect_as(input::IN_LEFT)
                                   ->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_BAR);
    register_param(param::START_BAR);
    register_param(param::END_BAR);
    register_param(param::SNAPSHOT_MODE)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* wavfileout::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
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

const void* wavfileout::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_WRITE_START_TRIG:
            return &out_write_start_trig;
        case output::OUT_WRITE_END_TRIG:
            return &out_write_end_trig;
        case output::OUT_WRITE_STATE:
            return &write_status;
        default:
            return 0;
    }
}

const void* wavfileout::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_LEFT:       return in_l = (double*)o;
        case input::IN_RIGHT:      return in_r = (double*)o;
        case input::IN_BAR:        return in_bar = (wcint_t*)o;
        default: return 0;
    }
}

const void* wavfileout::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:       return in_l;
        case input::IN_RIGHT:      return in_r;
        case input::IN_BAR:        return in_bar;
        default: return 0;
    }
}

bool wavfileout::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::SNAPSHOT_MODE:
            snapshot_mode = *(STATUS*)data;
            return true;
        case param::DATA_FMT:
            data_format = *(DATA_FMT*)data;
            return true;
        case param::FILENAME:
            set_wav_filename((char*)data);
            return true;
        case param::START_BAR:
            start_bar = *(wcint_t*)data;
            return true;
        case param::END_BAR:
            end_bar = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* wavfileout::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::SNAPSHOT_MODE: return &snapshot_mode;
        case param::DATA_FMT:      return &data_format;
        case param::FILENAME:      return filename;
        case param::START_BAR:     return &start_bar;
        case param::END_BAR:       return &end_bar;
        default: return 0;
    }
}

errors::TYPE wavfileout::validate()
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
    return errors::NO_ERROR;
}

void wavfileout::set_wav_filename(char* fname)
{
    const char* path = wcnt::jwm.path();
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
    size_t len = strlen(timestr);
    snprintf(timestr + len, 4, "%03d", now.millitm);
    len = strlen(timestr) + strlen(_filename);
    char* newname = new char[len + 1];
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

    if (!wcnt::jwm.is_dont_run()) {
        if (status == WAV_STATUS_OPEN) {
            #ifdef DEBUG
            std::cout << "why well i never..." << std::endl;
            #endif
            close_wav();
        }
        if ((fileout = open_wav_write(filename, data_format)) == 0)
            return status = WAV_STATUS_OPENERR;
    }
    return status = WAV_STATUS_OPEN;
}


void wavfileout::close_wav()
{
    if (status == WAV_STATUS_OPEN) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer, sample_total-buff_pos, buff_pos);
        if (!wcnt::jwm.is_dont_run()) {
            std::cout << "\nFinished writing to " << filename;
            sf_close(fileout);
        }
        status = WAV_STATUS_OK;
        write_status = OFF;
    }
}

void wavfileout::run()
{
    switch(state) {
    case 0: // waiting to start writing
        if (*in_bar == start_bar) {
            if (open_wav() != WAV_STATUS_OPEN) {
                std::cout << "\nCould not open WAV file " << filename
                          << " for writing. Abort forced...";
                synthmod::base::force_abort();
                invalidate();
                return;
            }
            buff_pos = 0;
            sample_total = 0;
            write_status = ON;
            out_write_start_trig = ON;
            state = 1;
        }
        break;
    case 1: // writing
        if(out_write_start_trig == ON)
            out_write_start_trig = OFF;
        sample_total++;
        st_buffer[buff_pos].left = *in_l;
        st_buffer[buff_pos].right = *in_r;
        buff_pos++;
        if (buff_pos == wcnt::wav_buffer_size) {
            write_wav_at(st_buffer, sample_total - wcnt::wav_buffer_size);
            buff_pos = 0;
        }
        if (*in_bar == end_bar) {
            if (buff_pos > 0)
                write_wav_chunk(st_buffer, sample_total - buff_pos,
                                                            buff_pos);
            close_wav();
            write_status = OFF;
            out_write_end_trig = ON;
            state = 2;
        }
        break;
    case 2:
        out_write_end_trig = OFF;
        state = 3;
    case 3:
        break;
    }
}

void wavfileout::write_wav_at(st_data* buf, samp_t smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, wcnt::wav_buffer_size);
    }
}

void wavfileout::write_wav_chunk(st_data* buf, samp_t smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(fileout, smp, SEEK_SET);
        sf_writef_double(fileout, (double*)buf, bsize);
    }
}
