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
 
#ifndef JWMSYNTH
#define JWMSYNTH

#include "synthmodulelist.h"
#include "modinputslist.h"
#include "modoutputslist.h"
#include "connectorlist.h"
#include "synthfilereader.h"
#include "rifflist.h"
#include "wavfileinlist.h"

/*
	jwmsynth - the main class
*/

class jwmsynth
{
  public:
    jwmsynth(int const argc, char **const argv);
     ~jwmsynth();
	// must call these functions  in order stated
	bool is_valid(){ return valid;} // 1
    bool scan_cl_options(); // 2
    bool generate_synth(); // 3
	bool connect_synth(); // 4
    bool execute_synth(); // 5
	// call this on above funtions returning false
    string get_error_msg(){ return err_msg;}
  private:
	synthmodnames* modnames;
	inputnames* innames;
	outputnames* outnames;
	paramnames* parnames;
    synthmodlist* synthmodslist;
    modinputlist* inputlist;
    modoutputlist* outputlist;
	modparamlist* paramlist;
	connectorlist* connectlist;
	synthfilereader* synthfile;
    riff_list* rifflist;
	wavfilein_list* wavfilelist;
	short exit_bar;
    int const options_count;
    char** const options;
    int option_filename_no;
	bool open_synthfile();
	bool valid;
    string err_msg;
	bool read_and_create_riff(string const* com);
	bool read_and_create_wavfilein(string const* com);
	bool read_and_create_synthmod(string const* com);
	bool module_help();
	bool riff_help();
	bool input_help();
	bool input_help_with_wcfile();
	bool sample_info();
};

#endif
