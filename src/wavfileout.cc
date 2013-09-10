#ifndef WAVFILEOUT_H
#include "../include/wavfileout.h"

wavfileout::wavfileout(string uname)
: synthmod(synthmodnames::MOD_WAVFILEOUT, wavfileout_count, uname), 
write_status(OFF), write_start_trigger_out(OFF), write_end_trigger_out(OFF), filename(NULL), fileout(0),
status(WAV_STATUS_INIT), left_channel(NULL), right_channel(NULL), st_buffer(NULL), 
start_bar(0), end_bar(0), sample(0), start_sample(0), end_sample(0), buff_pos(0) 
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_WRITE_STATE)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_WRITE_START_TRIG)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_WRITE_END_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_LEFT)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_RIGHT)){
		invalidate();
		return;
	}
	if (!(st_buffer = new stereodata[WAV_BUFFER_SIZE])){
		status = WAV_STATUS_MEMERR;
		invalidate();
		return;
	}
	for(short i = 0; i < WAV_BUFFER_SIZE; i++){
		st_buffer[i].left = 0;
		st_buffer[i].right = 0;
	}
	status = WAV_STATUS_OK;
	wavfileout_count++;
	validate();
	create_params();
}

wavfileout::~wavfileout() 
{
	if (header) 
		delete header;
	if (st_buffer) 
		delete st_buffer;
	if (fileout) 
		fclose(fileout);
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* wavfileout::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_WRITE_STATE:
			o = &write_status;
			break;
		case outputnames::OUT_WRITE_START_TRIG:
			o = &write_start_trigger_out;
			break;
		case outputnames::OUT_WRITE_END_TRIG:
			o = &write_end_trigger_out;
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
			i = left_channel = (short*)o;
			break;
		case inputnames::IN_RIGHT:
			i = right_channel = (short*)o;
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
		case paramnames::PAR_WRITE_FROM_BAR:
			set_write_from_bar(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_WRITE_TO_BAR:
			set_write_to_bar(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_WRITE_FROM_SAMPLE:
			set_write_from_sample(*(unsigned long*)data);
			retv = true;
			break;
		case paramnames::PAR_WRITE_TO_SAMPLE:
			set_write_to_sample(*(unsigned long*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

WAV_STATUS wavfileout::open_wav(char * fname)
{
	if ((fileout = fopen(fname, "wb")) == NULL) 
		return status = WAV_STATUS_OPENERR;;
	if ((filename = new char[strlen(fname) + 1]) == NULL) 
	{
		invalidate();
		return status = WAV_STATUS_MEMERR;
	}
	strcpy(filename, fname);
	if ((header = new wavheader(audio_channels, audio_samplerate, audio_bitrate)) == NULL) 
	{
		invalidate();
		return status = WAV_STATUS_MEMERR;
	}
	return status = WAV_STATUS_OPEN;
}

void wavfileout::close_wav() 
{
	if (status == WAV_STATUS_OPEN)
		fclose(fileout);
	status = WAV_STATUS_OK;
}

void wavfileout::set_write_from_bar(short b)
{
	start_bar = b;
	start_sample =	convert_notelen_to_samples(start_bar * beats_per_measure * (256 / beat_value));
}

void wavfileout::set_write_to_bar(short b )
{
	end_bar = b;
	end_sample =	convert_notelen_to_samples(end_bar * beats_per_measure * (256 / beat_value));
}

void wavfileout::set_write_from_sample(unsigned long s)
{
	start_sample = s;
}

void wavfileout::set_write_to_sample(unsigned long s)
{
	end_sample = s;
}

void wavfileout::run()
{
	if (sample == start_sample)  
	{
		if (status == WAV_STATUS_OPEN)
		{
			write_status = ON;
			write_start_trigger_out = ON;
			write_wav_header(end_sample - start_sample);
			buff_pos = 0;
		}
	} 
	else if(write_start_trigger_out == ON)
		write_start_trigger_out = OFF;
	sample++;
	if (write_status == ON) 
	{
		if (sample == end_sample) 
		{
			if (buff_pos > 0)  // blah
				write_wav_chunk(st_buffer, sample - start_sample - buff_pos, buff_pos);
			close_wav();
			write_status = OFF;
			write_end_trigger_out = ON;
		} 
		else 
		{
			st_buffer[buff_pos].left = * left_channel;
			st_buffer[buff_pos].right = * right_channel;
			buff_pos++;
			if (buff_pos == WAV_BUFFER_SIZE) 
			{
				write_wav_at(st_buffer, sample - start_sample - WAV_BUFFER_SIZE);
				buff_pos = 0;
			}
		}
	} 
	else if (write_end_trigger_out == ON)
		write_end_trigger_out = OFF;
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
bool wavfileout::done_params = false;

void wavfileout::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_FILENAME);
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_WRITE_FROM_BAR);
	get_paramlist()->add_param(synthmodnames::MOD_WAVFILEOUT, paramnames::PAR_WRITE_TO_BAR);
	// have not implemented ability to choose between write_from_bar and write_from_sample,
	// if I added both here, the user would have to define both rather than choose.
	done_params = true;
}

#endif
