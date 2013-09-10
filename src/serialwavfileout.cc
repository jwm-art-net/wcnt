#ifndef SERIALWAVFILEOUT_H
#include "../include/serialwavfileout.h"

serialwavfileout::serialwavfileout(string uname) :
	synthmod(synthmodnames::MOD_SERIALWAVFILEOUT, serialwavfileout_count, uname), 
	in_left_channel(NULL), in_right_channel(NULL), in_bar(0), in_bar_trig(0), 
	write_status(OFF), start_bar(0), end_bar(0), wav_basename(NULL), 
	fileout(0), wavcount(0), header(0), status(WAV_STATUS_INIT), 
	st_buffer(NULL), sample_total(0), buff_pos(0) 
{
	#ifndef BARE_MODULES
	get_inputlist()->add_input(this, inputnames::IN_LEFT);
	get_inputlist()->add_input(this, inputnames::IN_RIGHT);
	get_inputlist()->add_input(this, inputnames::IN_BAR);
	get_inputlist()->add_input(this, inputnames::IN_BAR_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_WRITE_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_STOP_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_WRITE_STATE);
	#endif
	st_buffer = new stereodata[WAV_BUFFER_SIZE];
	for(short i = 0; i < WAV_BUFFER_SIZE; i++){
		st_buffer[i].left = 0;
		st_buffer[i].right = 0;
	}
	header = new wavheader(audio_channels, audio_samplerate, audio_bitrate);
	serialwavfileout_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

serialwavfileout::~serialwavfileout() 
{
	if (in_write_region == ON && write_status == ON) {
		if (buff_pos > 0)
			write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
		close_wav();
	}
	if (status == WAV_STATUS_OPEN)
		fclose(fileout);
	delete [] st_buffer;
	delete header;
	if (wav_basename) delete [] wav_basename;
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}


#ifndef BARE_MODULES
void const* serialwavfileout::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_WRITE_STATE:
			o = &write_status;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* serialwavfileout::set_in(inputnames::IN_TYPE it, void const* o)
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
		case inputnames::IN_WRITE_TRIG:
			i = in_write_trig = (STATUS*)o;
			break;
		case inputnames::IN_STOP_TRIG:
			i = in_stop_trig = (STATUS*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool serialwavfileout::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_WAV_BASENAME:
			set_wav_basename((char*)data);
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

bool serialwavfileout::validate()
{
	if (start_bar < 0) {
		*err_msg = "\n" + get_paramnames()->get_name(paramnames::PAR_START_BAR);
		*err_msg += " should be non-negative value";
		invalidate();
	}
	if (end_bar <= start_bar) {
		*err_msg += "\n" + get_paramnames()->get_name(paramnames::PAR_END_BAR);
		*err_msg += " should be greater than value of " +
			get_paramnames()->get_name(paramnames::PAR_START_BAR);
		invalidate();
	}
	// won't bother validating basename, if blank, 
	// output names will be 0001.wav etc
	return is_valid();
}

#endif // BARE_MODULES

void serialwavfileout::set_wav_basename(char * fname)
{
	wav_basename = new char[strlen(fname) + 1];
	strcpy(wav_basename, fname);
}

WAV_STATUS serialwavfileout::open_wav(char * fname)
{
	if ((fileout = fopen(fname, "wb")) == NULL) 
		return status = WAV_STATUS_OPENERR;
	return status = WAV_STATUS_OPEN;
}

void serialwavfileout::close_wav() 
{
	write_wav_header(sample_total);
	if (status == WAV_STATUS_OPEN)
		fclose(fileout);
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
				if (buff_pos > 0)
					write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
				close_wav();
			}
			buff_pos = 0;
			sample_total = 0;
			write_status = ON;
			wavcount++;
			wavfilename = new char[strlen(wav_basename) + 8];
			sprintf(wavfilename, "%s%04d.wav", wav_basename, wavcount);
			if (open_wav(wavfilename) != WAV_STATUS_OPEN) {
				cout << "\n!!!Unable to create '" << wavfilename;
				cout << "to write from module " << get_username() << "!!!\n";
				delete [] wavfilename;
				write_status = OFF;
				in_write_region = OFF;
			}
		}	
		if (write_status == ON) {
			sample_total++;
			if (*in_stop_trig == ON) {
				if (buff_pos > 0)
					write_wav_chunk(st_buffer,sample_total-buff_pos,buff_pos);
				close_wav();
				write_status = OFF;
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
		} // endif (write_status == ON)
	} // endif (write_region == ON)
}

void serialwavfileout::write_wav_at(stereodata* buf, unsigned long smp) 
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
		fwrite((stereodata*) buf, sizeof(stereodata), WAV_BUFFER_SIZE, fileout);
	}
}

void serialwavfileout::write_wav_chunk(stereodata * buf, unsigned long smp, int bsize)
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(fileout, sizeof(wavheader) + smp * header->GetBlockAlign(), SEEK_SET);
		fwrite((stereodata*) buf, sizeof(stereodata), bsize, fileout);
	}
}

void serialwavfileout::write_wav_header(unsigned long length) 
{
	if (status == WAV_STATUS_OPEN)
	{
		fseek(fileout, 0, SEEK_SET);
		header->SetReady(length);
		fwrite((wavheader*) header, sizeof(wavheader), 1, fileout);
	}
}

short serialwavfileout::serialwavfileout_count = 0;

#ifndef BARE_MODULES
bool serialwavfileout::done_params = false;

void serialwavfileout::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SERIALWAVFILEOUT, paramnames::PAR_WAV_BASENAME);
	get_paramlist()->add_param(synthmodnames::MOD_SERIALWAVFILEOUT, paramnames::PAR_START_BAR);
	get_paramlist()->add_param(synthmodnames::MOD_SERIALWAVFILEOUT, paramnames::PAR_END_BAR);
	done_params = true;
}
#endif
#endif
