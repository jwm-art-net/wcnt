#ifndef WAVFILEIN_H
#include "../include/wavfilein.h"

wavfilein::wavfilein() :
 dobj(dobjnames::DEF_WAVFILEIN), fname(0), rootnote(0), filein(0),
 header(0), status(WAV_STATUS_INIT)
{
    header = new wavheader;
    create_params();
}

wavfilein::~wavfilein()
{
    delete header;
    if (fname)
        delete [] fname;
    if (status == WAV_STATUS_OPEN)
        fclose(filein);
    if (rootnote)
        delete [] rootnote;
}

WAV_CHANNELS wavfilein::get_channel_status()
{
    if (status == WAV_STATUS_OPEN)
        if (header->GetChannels() == 1)
            return WAV_CH_MONO;
        else
            return WAV_CH_STEREO;
    else
        return WAV_CH_UNKNOWN;
}

WAV_BITRATE wavfilein::get_bitrate()
{
    if (header->GetResolution() == 8)
        return WAV_BIT_8;
    else if (header->GetResolution() == 16)
        return WAV_BIT_16;
    else
        return WAV_BIT_OTHER;
}


void wavfilein::set_wav_filename(const char* filename)
{
    if (fname)
        delete [] fname;
    const char* path = synthmod::get_path();
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
        fclose(filein);
    if ((filein = fopen(fname, "rb")) == NULL)
        return status = WAV_STATUS_NOT_FOUND;
    fseek(filein, 0, SEEK_SET);
    fread(header, sizeof(wavheader), 1, filein);
    if (!header->valid_format_name()) {
        fclose(filein);
        return status = WAV_STATUS_WAVERR;
    }
    if (!header->valid_type_name()) {
        fclose(filein);
        return status = WAV_STATUS_WAVERR;
    }
    if (!header->valid_format_name()){
        fclose(filein);
        return status = WAV_STATUS_WAVERR;
    }
    return status = WAV_STATUS_OPEN;
}

void wavfilein::set_root_note(char* rn)
{
    if (rootnote) delete [] rootnote;
    rootnote = new char[NOTE_ARRAY_SIZE];
    strncpy(rootnote, rn, NOTE_NAME_LEN);
    rootnote[NOTE_NAME_LEN] = '\0';
}

double wavfilein::get_root_deg_size()
{
    return note_to_step(rootnote, 0);
}

void wavfilein::read_wav_at(void * buf, unsigned long smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        fseek(filein, sizeof(wavheader) + smp * header->GetBlockAlign(), 
         SEEK_SET);
        if (header->GetChannels() == 1)
            fread((short*)buf, sizeof(short), WAV_BUFFER_SIZE, filein);
        else
            fread((stereodata*) buf, sizeof(stereodata), WAV_BUFFER_SIZE,
             filein);
    }
}

void wavfilein::read_wav_chunk(void * buf, unsigned long smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        fseek(filein, sizeof(wavheader) + smp * header->GetBlockAlign(),
         SEEK_SET);
        if (header->GetChannels() == 1)
            fread((short*)buf, sizeof(short), bsize, filein);
        else
            fread((stereodata*)buf, sizeof(stereodata), bsize, filein);
    }
}

bool wavfilein::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_FILENAME:
        set_wav_filename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_ROOT_NOTE:
        set_root_note((char*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* wavfilein::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_FILENAME:
        retv = fname;
        break;
    case paramnames::PAR_ROOT_NOTE:
        retv = rootnote;
        break;
    default:
        retv = 0;
    }
    return retv;
}

stockerrs::ERR_TYPE wavfilein::validate()
{
    open_wav();
    if (status == WAV_STATUS_NOT_FOUND) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_FILENAME);
        *err_msg += ", ";
        *err_msg += fname;
        *err_msg += " was not found.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (status == WAV_STATUS_WAVERR) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_FILENAME);
        *err_msg += ", ";
        *err_msg += fname;
        *err_msg += " is not a wav file.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (status != WAV_STATUS_OPEN) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_FILENAME);
        *err_msg = ", an unspecified error occurred trying to open ";
        *err_msg += fname;
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!check_notename(rootnote)) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_ROOT_NOTE);
        *err_msg += ", ";
        *err_msg += rootnote;
        invalidate();
        return stockerrs::ERR_NOTENAME;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wavfilein::create_params()
{
    if (done_params == true)	return;
    get_dparlist()->add_dobjparam(
     dobjnames::DEF_WAVFILEIN, paramnames::PAR_FILENAME);
    get_dparlist()->add_dobjparam(
     dobjnames::DEF_WAVFILEIN, paramnames::PAR_ROOT_NOTE);
    done_params = true;
}

bool wavfilein::done_params = false;

#endif
