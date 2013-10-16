#include "../include/wavfilein.h"
#include "../include/globals.h"
#include "../include/conversions.h"


wavfilein::wavfilein() :
 dobj::base(dobj::DEF_WAVFILEIN),
 fname(0), rootnote(0),
 filein(0), status(WAV_STATUS_INIT)
{
}

void wavfilein::register_ui()
{
    register_param(param::FILENAME);
    register_param(param::ROOT_NOTE);
}

wavfilein::~wavfilein()
{
    if (fname)
        delete [] fname;
    if (status == WAV_STATUS_OPEN)
        sf_close(filein);
    if (rootnote)
        delete [] rootnote;
}

WAV_CHANNELS wavfilein::get_channel_status() const
{
    if (status == WAV_STATUS_OPEN) {
        if (sfinfo.channels == 1)
            return WAV_CH_MONO;
        else if (sfinfo.channels == 2)
            return WAV_CH_STEREO;
    }
    return WAV_CH_UNKNOWN;
}

#ifdef UNUSED
WAV_BITRATE wavfilein::get_bitrate() const
{
    /*
    if (header->GetResolution() == 8)
        return WAV_BIT_8;
    else if (header->GetResolution() == 16)
        return WAV_BIT_16;
    else
    */
    return WAV_BIT_OTHER;
}
#endif

samp_t wavfilein::get_length() const
{
    if (status == WAV_STATUS_OPEN)
        return sfinfo.frames;
    else
        return 0;
}


samp_t wavfilein::get_sample_rate() const
{
    if (status == WAV_STATUS_OPEN)
        return sfinfo.samplerate;
    else
        return 0;
}

void wavfilein::set_wav_filename(const char* filename)
{
    if (fname)
        delete [] fname;
    const char* path = wcnt::jwm.path();
    if (*filename == '/' || path == NULL) {
        fname = new char[strlen(filename)+1];
        strcpy(fname, filename);
    }
    else {
        char* ptr;
        fname = new char[strlen(filename) + strlen(path) + 1];
        strcpy(fname, path);
        ptr = fname + strlen(path);
        strcpy(ptr, filename);
    }
}


WAV_STATUS wavfilein::open_wav()
{
    if (status == WAV_STATUS_MEMERR)
        return status;
    if (status == WAV_STATUS_OPEN)
        return status;
    if ((filein = sf_open(fname, SFM_READ, &sfinfo)) == NULL)
        return status = WAV_STATUS_NOT_FOUND;
    return status = WAV_STATUS_OPEN;
}

void wavfilein::set_root_note(const char* rn)
{
    if (rootnote) delete [] rootnote;
    rootnote = new char[wcnt::note_array_size];
    strncpy(rootnote, rn, wcnt::note_name_len);
    rootnote[wcnt::note_name_len] = '\0';
}

double wavfilein::get_root_phase_step() const
{
    return note_to_step(rootnote, 0);
}

void wavfilein::read_wav_at(void * buf, samp_t smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(filein, smp, SEEK_SET);
        sf_read_double(filein, (double*)buf,
                       wcnt::wav_buffer_size * sfinfo.channels);
    }
}

void wavfilein::read_wav_chunk(void * buf, samp_t smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        sf_seek(filein, smp, SEEK_SET);
        sf_read_double(filein, (double*)buf, bsize * sfinfo.channels);
    }
}

bool wavfilein::set_param(param::TYPE dt, const void* data)
{
    switch(dt)
    {
        case param::FILENAME:
            set_wav_filename((const char*)data);
            return true;
        case param::ROOT_NOTE:
            set_root_note((const char*)data);
            return true;
        default:
            return false;
    }
}

const void* wavfilein::get_param(param::TYPE dt) const
{
    switch(dt)
    {
        case param::FILENAME:  return fname;
        case param::ROOT_NOTE: return rootnote;
        default: return 0;
    }
}

errors::TYPE wavfilein::validate()
{
    if (!check_notename(rootnote)) {
        dobjerr("%s, %s", param::names::get(
                                    param::ROOT_NOTE), rootnote);
        invalidate();
        return errors::NOTENAME;
    }
    return errors::NO_ERROR;
}

