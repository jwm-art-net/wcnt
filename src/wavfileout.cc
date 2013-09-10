#ifndef WAVFILEOUT_H
#include "../include/wavfileout.h"

wavfileout::wavfileout(string uname) :
	synthmod(synthmodnames::MOD_WAVFILEOUT, wavfileout_count, uname), 
	in_left_channel(0), in_right_channel(0), in_bar(0), in_bar_trig(0), 
	out_write_start_trig(OFF), out_write_end_trig(OFF), write_status(OFF),
	start_bar(0), end_bar(0), filename(0), fileout(0), header(0),
	status(WAV_STATUS_INIT), st_buffer(0), sample_total(0), buff_pos(0) 
{
	#ifndef BARE_MODULES
	get_inputlist()->add_input(this, inputnames::IN_LEFT);
	get_inputlist()->add_input(this, inputnames::IN_RIGHT);
	get_inputlist()->add_input(this, inputnames::IN_BAR);
	get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_WRITE_START_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_WRITE_END_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_WRITE_STATE);
	#endif
	st_buffer = new stereodata[WAV_BUFFER_SIZE];
	for(short i = 0; i < WAV_BUFFER_SIZE; i++){
		st_buffer[i].left = 0;
		st_buffer[i].right = 0;
	}
	header = new wavheader(audio_channels, audio_samplerate, audio_bitrate);
	status = WAV_STATUS_OK;
	wavfileout_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

wavfileout::~wavfileout() 
{
	if (write_status == ON) {
		if (buff_pos > 0)
			write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
		close_wav();
	}
	if (fileout) 
		fclose(fileout);
	delete st_buffer;
	if (header)
		delete header;
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}


#ifndef BARE_MODULES
void const* wavfileout::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_WRITE_START_TRIG:
			o = &out_write_start_trig;
			break;
		case outputnames::OUT_WRITE_END_TRIG:
			o = &out_write_end_trig;
			break;
		case outputnames::OUT_WRITE_STATE:
			o = &write_status;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* wavfileout::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_LEFT:
			i = in_left_channel = (short*)o;
			break;
		case inputnames::IN_RIGHT:
			i = in_right_channel = (short*)o;
			break;
		case inputnames::IN_BAR:
			i = in_bar = (short*)o;
			break;
		case inputnames::IN_BAR_TRIG:
			i = in_bar_trig = (STATUS*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool wavfileout::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_FILENAME:
			open_wav((char*)data);
			retv = true;
			break;
		case paramnames::PAR_START_BAR:
			set_start_bar(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_END_BAR:
			set_end_bar(*(short*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

bool wavfileout::validate()
{
	if (start_bar < 0) {
		*err_msg = "\n" + get_paramnames()->get_name(paramnames::PAR_START_BAR);
		*err_msg += " must not be negative";
		invalidate();
	}
	if (end_bar <= start_bar) {
		*err_msg += "\n" + get_paramnames()->get_name(paramnames::PAR_END_BAR);
		*err_msg += " must be more than value of " +
			get_paramnames()->get_name(paramnames::PAR_START_BAR);
		invalidate();
	}
	if (status != WAV_STATUS_OPEN) {
		*err_msg += "\ncould not open ";
		*err_msg += filename;
		*err_msg += " for writing";
		invalidate();
	}
	return is_valid();
}

#endif // BARE_MODULES

WAV_STATUS wavfileout::open_wav(char * fname)
{
	if ((fileout = fopen(fname, "wb")) == NULL) 
		return status = WAV_STATUS_OPENERR;
	filename = new char[strlen(fname) + 1];
	strcpy(filename, fname);
	return status = WAV_STATUS_OPEN;
}

void wavfileout::close_wav() 
{
	write_wav_header(sample_total - 1);
	if (status == WAV_STATUS_OPEN)
		fclose(fileout);
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
				write_wav_chunk(st_buffer, sample_total - buff_pos - 1, buff_pos);
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
		fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
		fwrite((stereodata*) buf, sizeof(stereodata), WAV_BUFFER_SIZE, fileout);
	}
}

void wavfileout::write_wav_chunk(stereodata * buf, unsigned long smp, int bsize)
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
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

#ifndef BARE_MODULES
bool wavfileout::done_params = false;

void wavfileout::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_FILENAME);
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_START_BAR);
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_END_BAR);
	// have not implemented ability to choose between write_from_bar and write_from_sample,
	// if I added both here, the user would have to define both rather than choose.
	done_params = true;
}
#endif
#endif
