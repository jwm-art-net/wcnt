#ifndef WAVFILEOUT_H
#include "../include/wavfileout.h"

wavfileout::wavfileout(char const* uname) :
 synthmod(synthmodnames::MOD_WAVFILEOUT, wavfileout_count, uname),
 in_left_channel(0), in_right_channel(0), in_bar(0), in_bar_trig(0),
 out_write_start_trig(OFF), out_write_end_trig(OFF), write_status(OFF),
 start_bar(0), end_bar(0), filename(0), fileout(0), header(0),
 status(WAV_STATUS_INIT), st_buffer(0), sample_total(0), buff_pos(0)
{
    get_inputlist()->add_input(this, inputnames::IN_LEFT);
    get_inputlist()->add_input(this, inputnames::IN_RIGHT);
    get_inputlist()->add_input(this, inputnames::IN_BAR);
    get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_WRITE_START_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_WRITE_END_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_WRITE_STATE);
    st_buffer = new stereodata[WAV_BUFFER_SIZE];
    for(short i = 0; i < WAV_BUFFER_SIZE; i++){
        st_buffer[i].left = 0;
        st_buffer[i].right = 0;
    }
    header =
     new wavheader(audio_channels, audio_samplerate, audio_bitrate);
    status = WAV_STATUS_OK;
    wavfileout_count++;
    create_params();
}

wavfileout::~wavfileout()
{
    if (write_status == ON) {
        if (buff_pos > 0)
            write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
    }
    close_wav();
    if (filename) delete [] filename;
    delete [] st_buffer;
    if (header)
        delete header;
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* wavfileout::get_out(outputnames::OUT_TYPE ot)
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
    case inputnames::IN_LEFT:
        return in_left_channel = (short*)o;
    case inputnames::IN_RIGHT:
        return in_right_channel = (short*)o;
    case inputnames::IN_BAR:
        return in_bar = (short*)o;
    case inputnames::IN_BAR_TRIG:
        return in_bar_trig = (STATUS*)o;
    default:
        return 0;
    }
}

void const* wavfileout::get_in(inputnames::IN_TYPE it)
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
    default:
        return 0;
    }
}

bool wavfileout::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_FILENAME:
        set_wav_filename((char*)data);
        return true;
    case paramnames::PAR_START_BAR:
        set_start_bar(*(short*)data);
        return true;
    case paramnames::PAR_END_BAR:
        set_end_bar(*(short*)data);
        return true;
    default:
        return false;
    }
}

void const* wavfileout::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_FILENAME:
        return filename;
    case paramnames::PAR_START_BAR:
        return &start_bar;
    case paramnames::PAR_END_BAR:
        return &end_bar;
    default:
        return 0;
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
    if (!get_paramlist()->validate(this, paramnames::PAR_START_BAR,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_START_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (end_bar <= start_bar) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_END_BAR);
        *err_msg += " should be after ";
        *err_msg += get_paramnames()->get_name(paramnames::PAR_START_BAR);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void wavfileout::set_wav_filename(char* fname)
{
    char const* path = synthmod::get_path();
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
    if ((fileout = fopen(filename, "wb")) == NULL)
        return status = WAV_STATUS_OPENERR;
    return status = WAV_STATUS_OPEN;
}


void wavfileout::close_wav()
{
    write_wav_header(sample_total - 1);
    if (status == WAV_STATUS_OPEN) {
        cout << "\nFinished writing to " << filename;
        fclose(fileout);
    }
    status = WAV_STATUS_OK;
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
            st_buffer[buff_pos].left = *in_left_channel;
            st_buffer[buff_pos].right = *in_right_channel;
            buff_pos++;
            if (buff_pos == WAV_BUFFER_SIZE) {
                write_wav_at(st_buffer, sample_total - WAV_BUFFER_SIZE);
                buff_pos = 0;
            }
        }
    }
    else if (out_write_end_trig == ON)
        out_write_end_trig = OFF;
}

void wavfileout::write_wav_at(stereodata* buf, unsigned long smp)
{
    if (status == WAV_STATUS_OPEN)
    {
        fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(),
         SEEK_SET);
        fwrite((stereodata*) buf, sizeof(stereodata), WAV_BUFFER_SIZE,
         fileout);
    }
}

void wavfileout::write_wav_chunk(
 stereodata * buf, unsigned long smp, int bsize)
{
    if (status == WAV_STATUS_OPEN)
    {
        fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(),
         SEEK_SET);
        fwrite((stereodata*) buf, sizeof(stereodata), bsize, fileout);
    }
}

void wavfileout::write_wav_header(unsigned long length)
{
    if (status == WAV_STATUS_OPEN)
    {
        fseek(fileout, 0, SEEK_SET);
        header->SetReady(length);
        fwrite((wavheader*) header, sizeof(wavheader), 1, fileout);
    }
}

short wavfileout::wavfileout_count = 0;

bool wavfileout::done_params = false;

void wavfileout::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_FILENAME);
    get_paramlist()->add_param(
     synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_START_BAR);
    get_paramlist()->add_param(
     synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_END_BAR);
// have not implemented ability to choose between write_from_bar and
// write_from_sample,  if I added both here, the user would have to
// define both rather than choose.
    done_params = true;
}

#endif
