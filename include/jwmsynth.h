#ifndef JWMSYNTH
#define JWMSYNTH

#ifndef BARE_MODULES

#include <iostream>

#include "synthmodulelist.h"
#include "modinputslist.h"
#include "modoutputslist.h"
#include "connectorlist.h"
#include "synthfilereader.h"
#include "dobjlist.h"

// jwmsynth - the class to tie everything together.
// processes command line, initiates synth generation and file reading
// then connects, and executes.  also creates lists and names.
// knows when to stop!


// in the something_help() methods you'll see code like this:-
// 		delete something_list->create_something(something_type);
// this is because in order to find out what parts make up that
// something, that something has to be created so that it creates
// the parts of the ui which correspond to them.  it's ok. alright?

class jwmsynth
{
  public:
    jwmsynth(int const argc, char **const argv);
     ~jwmsynth();
	const char* get_version(){ return version;}
	// must call these functions  in order stated
	bool is_valid(){ return valid;}	// 1
    bool scan_cl_options(); 		// 2
    bool generate_synth(); 			// 3
	bool connect_synth(); 			// 4
    bool execute_synth(); 			// 5
    string get_error_msg(){ return err_msg;}
  private:
	// data for creating modules, and making connections etc
	char* version;
	string wcnt_id;
 	iocat_names* iocatnames;
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
	// dobj data object stuff
	moddobjlist* mdobjlist;
	dobjnames* dobj_names;
	dobjlist* dobj_list;
	dobjparamlist* dobjparam_list;
	dparnames* dpar_names;
	dobjdobjlist* dobj_dobjlist;
	// validation and error message
	bool valid;
    string err_msg;
	// these are used to figure out when to stop
	short exit_bar;
 	const STATUS* in_bar_trig;  // taken from the only wcnt_exit module
 	const short* in_bar;		// the user is allowed to create
	// command line processing
    int const options_count;
    char** const options;
    int wcntfile_option_no;
	// file reading
	bool open_synthfile();
	bool read_and_create_synthmod(string const* com);
	bool read_and_create_dobj(string const* com);
	// command line help
	void module_help();
	void dobj_help();
	void dobj_help(synthmodnames::SYNTH_MOD_TYPE);
	void input_help();
	void input_help_with_wcfile();
	void sample_info();
	void dobj_help_params(dobjnames::DOBJ_TYPE);
	void note_help();
	void freq_help();
};

#endif
#endif
