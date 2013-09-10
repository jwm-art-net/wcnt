/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef SYNTHFILEREADER_H
#define SYNTHFILEREADER_H

#include <fstream>
#include <sstream>
#include <string>

#include "modules.h"
#include "connectorlist.h"

class synthfilereader
{
 public:
	enum FILE_STATUS
	{
		NOT_FOUND = -1,
		INVALID_HEADER = 0,		
		FILE_OPEN = 1,
		FILE_READY = 2
	};
	enum GEN_STATUS
	{
		MASSIVE_ERROR = -2,
		HALTED = -1,
		OK = 0,
		COMPLETE = 1
	};
	
	synthfilereader();
	~synthfilereader();
	FILE_STATUS open_file(char* synthfilename);
	bool read_header(unsigned long* samplerate, short* bpm, short* beatspermease, short* beatvalue, short* exitbar);
	string const* read_command();
	synthmod* const read_synthmodule(string const* command);
	riffdata* const read_riffdata(string const* command);
	wavfilein* const read_wavfilein(string const* command);
	char* get_filename(){ return filename;}
	GEN_STATUS get_generation_status(){ return genstatus;}
	string get_error_msg(){ return *err_msg;}
	void set_verbose(){ verbose = true;}
	bool is_verbose(){ return (verbose == true);}
	
 private:
	char* filename;
	FILE_STATUS filestatus;
	ifstream* synthfile;
	string* buff;
	string* synthheader;
	string* err_msg;
	ostringstream* conv;
	GEN_STATUS genstatus;
	bool verbose;
	note_data* const read_notedata();
	bool skip_remarks();
	bool read_inputs(synthmod*);
	bool read_params(synthmod*);
	void* read_param_value(paramnames::PAR_TYPE);
	void destroy_tmp_param_data(paramnames::PAR_TYPE pt, void* data);
	bool read_adsr_envelope(adsr*);
	bool read_sequencer_riffs(sequencer*);
	bool read_mixer_channels(stereomixer*);
	bool read_userwave_envelope(user_wave*);
	bool read_signals(synthmod*);
	wave_vertex* read_wave_vertex();
	adsr_coord* read_adsr_coord(adsr_coord::SECT);
};

#endif
