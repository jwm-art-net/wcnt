#ifndef JWMSYNTH_H
#include "../include/jwmsynth.h"

#ifndef BARE_MODULES
jwmsynth::jwmsynth(int const argc, char **const argv) : 
	modnames(0), innames(0), outnames(0), parnames(0), synthmodslist(0), 
  	inputlist(0), outputlist(0), paramlist(0), connectlist(0), synthfile(0),
	rifflist(0), exit_bar(0), options_count(argc), options(argv), 
	option_filename_no(0), valid(false)
{
	synthmod::register_modnames(modnames = new synthmodnames);
	synthmod::register_inputnames(innames = new inputnames);
	synthmod::register_outputnames(outnames  = new outputnames);
	synthmod::register_paramnames(parnames = new paramnames);
	synthmod::register_inputlist(inputlist = new modinputlist);
	synthmod::register_outputlist(outputlist = new modoutputlist);
	synthmod::register_paramlist(paramlist = new modparamlist);
	// must register the various things above before creating 
	// synthmodlist as it creates a nonezero module which
	// must have access to the lists - or cause a seg fault.
	synthmod::register_modlist(synthmodslist = new synthmodlist);
	synthmod::register_connectlist(connectlist = new connectorlist);
	sequencer::register_rifflist(rifflist = new riff_list);
	sampler::register_wavfilein_list(wavfilelist = new wavfilein_list);
	synthfile = new synthfilereader;	
	valid = true;
}

jwmsynth::~jwmsynth()
{
	delete modnames;
	delete innames;
	delete outnames;
	delete parnames;
    delete synthmodslist;
    delete inputlist;
    delete outputlist;
	delete connectlist;
	delete synthfile;
    delete rifflist;
}

bool jwmsynth::generate_synth()
{
	if (!open_synthfile())
		return false;
    if (!synthfile->read_header(&audio_samplerate, &sm_beats_per_minute, 
			&sm_beats_per_measure, &sm_beat_value)) {
        err_msg = synthfile->get_error_msg();
        return false;
    }
    if (synthfile->is_verbose()) {
        cout << "\n\nProccessing wcnt/jwmsynth: "<<options[option_filename_no];
	}
    string const *com = synthfile->read_command();
    while (*com != "wcnt-1.1001/jwmsynth") 
	{
        if (!com) {
            err_msg = synthfile->get_error_msg();
            return false;
        }
        if (*com == "riff"){
			if (!read_and_create_riff(com))
				return false;
		} 
		else if (*com == "wavfilein"){
			if (!read_and_create_wavfilein(com))
				return false;
        } 
		else { // create synth module
			if (!read_and_create_synthmod(com))
				return false;
		}
        com = synthfile->read_command();
    }
	if (synthmodslist->get_first_of_type(synthmodnames::MOD_WCNT) == 0) {
		// wankest error message ever:)
		err_msg = "\nAfter processing the file '";
		err_msg += options[option_filename_no];
		err_msg += "'\nI notice you have not created a wcnt module.";
		err_msg += "\nWithout this module I don't know when to stop, therefore,";
		err_msg += "\nand, only wishing to keep your best interests at heart, I ";
		err_msg += "\nhave decided to cease operati....kerplunkshshshzzzdom.";
		return false;
	}
	return true;
}

bool jwmsynth::read_and_create_synthmod(string const* com)
{
	synthmod *mod = synthfile->read_synthmodule(com);
	if (!mod) {
		err_msg = synthfile->get_error_msg();
		return false;
	}
	if (!mod->is_valid()) {
		err_msg = "\nunable to properly create synthmodule: ";
		err_msg += (*mod->get_username() + " probably lacking memory ");
		return false;
	}
	if (!(synthmodslist->add_module(mod))) {
		err_msg = "\nproblems adding synthmodule: " + *mod->get_username();
		err_msg += " to list";
		return false; 
	}
	if (synthfile->is_verbose())
		cout << "\nend " << *mod->get_username();
	return true;
}

bool jwmsynth::read_and_create_wavfilein(string const* com)
{
	
	wavfilein* wavfile = synthfile->read_wavfilein(com);
	if (!wavfile) {
		err_msg = synthfile->get_error_msg();
		return false;
	}
	if (!wavfilelist->add_wavfile(wavfile)) {
		err_msg = "\nUnable to add ";
		err_msg += wavfile->get_sample_name();
		err_msg += " to list";
		return false;
	}
	if (synthfile->is_verbose())
		cout << "\nwavfilein:" << wavfile->get_sample_name();
	return true;
}

bool jwmsynth::read_and_create_riff(string const* com)
{
	riffdata* riff = synthfile->read_riffdata(com);
	if (!riff) {
		err_msg = synthfile->get_error_msg();
		return false;
	}
	if (!rifflist->add_riff(riff)) {
		err_msg = "\nUnable to add " + *riff->get_username() + " to list";
		return false;
	}
	if (synthfile->is_verbose())
		cout << "\nend " << *riff->get_username();
	return true;
}

bool jwmsynth::connect_synth()
{
    if (synthfile->is_verbose()) {
        cout << "\n\nEnd wcnt/jwmsynth: " << options[option_filename_no];
		cout << "\n\nWill now attempt to connect inputs and outputs";
		cout << ", hold your horses:\n";
		connectlist->set_verbose();
	}
	if (!connectlist->make_connections()) {
		err_msg = *connector::get_connect_err_msg();
		return false;
	}
	// might aswell sneak this in here
	synthmod* sm = synthmodslist->goto_first();
	while(sm) {
		sm->init();
		sm = synthmodslist->goto_next();
	}
    return true;
}

bool jwmsynth::execute_synth()
{
	if (synthfile->is_verbose())
		cout << "\n\nValidating synth...";
	synthmod* sm = synthmodslist->goto_first();
	while(sm) {
		if (!sm->is_valid()) {
			err_msg = "\nSynth module: " + *sm->get_username();
			err_msg += " is not valid to run.";
			return false;
		}
		sm = synthmodslist->goto_next();
	}
	// only use first wcnt module created, don't bother with any others
	// although user should not have been allowed to create > 1
	wcnt_module* wcnt = (wcnt_module*) 
		synthmodslist->get_first_of_type(synthmodnames::MOD_WCNT);
	const short* bar = wcnt->get_input_bar();
	short exit_bar = wcnt->get_exit_bar();
	unsigned long sample = 0;
	char bigcount = '@';
	char littlecount = '~';
	int samplesperbig = audio_samplerate;
	int divisions = 10;
	int samplespersmall = samplesperbig / divisions;
	int counter = 0;
	int divcounter = 0;
	cout << "\n\nProcessing " << options[option_filename_no] << "\tone '";
	cout << bigcount << "' per second done\n";
	while (*bar < exit_bar)
	{
		sm = synthmodslist->goto_first();
		while(sm) {
			sm->run();
			sm = synthmodslist->goto_next();
		}
		sample++;
		counter++;
		if (counter == samplespersmall)	{
			divcounter++;
			if (divcounter == divisions) {
				cout << bigcount;
				divcounter = 0;
			}
			else cout << littlecount;
			cout.flush();
			counter = 0;
		}
	}
	return true;
}

bool
jwmsynth::scan_cl_options()
{
	string wcnt = "\nwcnt ";
	string filename = " filename.wc ";
	string riffhelp = "--riff-help";
	string inputhelp = "--input-help";
	string modhelp = "--module-help";
	string sampleinfo = "--sample-info";
	string commandoptions = wcnt + filename;
	commandoptions += wcnt + " -v" + filename;
	commandoptions += wcnt + riffhelp;
	commandoptions += wcnt + modhelp;
	commandoptions += wcnt + modhelp + " modulename";
	commandoptions += wcnt + inputhelp + " inputname"; 
	commandoptions += wcnt + filename + inputhelp + " inputname";
	commandoptions += wcnt + sampleinfo + " samplename.wav";
    if (options_count == 1) {
        err_msg = commandoptions + "\n";
        return false;
    } 
	else if (options_count == 2) {
		if (modhelp == options[1]) {
			module_help();
			return false;
		} else if (riffhelp == options[1]) {
			riff_help();
			return false;
		}
        option_filename_no = 1;
		return true; // should be a filename then
    }
	else if (options_count == 3) {
		if (inputhelp == options[1]) {
			input_help();
			return false;
		} else if (modhelp == options[1]) {
			module_help();
			return false;
		} else if (sampleinfo == options[1]) {
			sample_info();
			return false;
		} else if (options[1][0] == '-' && options[1][1] == 'v') {
            synthfile->set_verbose();
            option_filename_no = 2;
			return true;
        } else if (options[2][0] == '-' && options[2][1] == 'v') {
            synthfile->set_verbose();
            option_filename_no = 1;
			return true;
        } 
	}
	else if (options_count == 4) {
		if (inputhelp == options[2]) {
			option_filename_no = 1;
			input_help();
			return false;
		} 
	}
	err_msg = "\nInvalid Options:";
	for (int i = 1; i < options_count; i++) {
		err_msg += " ";
		err_msg += options[i];
	}
	err_msg += commandoptions;
	return false;
}

bool jwmsynth::open_synthfile()
{
    string tmp(options[option_filename_no]);
    bool rtval = false;
    switch (synthfile->open_file(options[option_filename_no])) {
    case synthfilereader::NOT_FOUND:
        err_msg = "\nfile " + tmp + " not found.\n";
        break;
    case synthfilereader::INVALID_HEADER:
        err_msg = "\nfile " + tmp + " does not contain a valid header.\n";
        break;
    case synthfilereader::FILE_OPEN:
        rtval = true;
        break;
    case synthfilereader::FILE_READY:
        err_msg = "\nfile " + tmp + " inspires premature optimism.\n";
        break;
    }
	return rtval;
}

bool jwmsynth::module_help()
{
	string smname = (options_count == 3) ? options[2] : "";
	synthmodnames* modnames = synthmod::get_modnames();
	synthmodnames::SYNTH_MOD_TYPE smt = modnames->get_type(&smname);
	if (smt == synthmodnames::MOD_FIRST) {
		if (options_count == 3) 
			err_msg = "\nUnknown synth module: " + smname + "\n";
		else
			err_msg = "";
		err_msg += "\navailable module types are:\n\n";
		int i, ip = 0;
		for (i = synthmodnames::MOD_FIRST + 1;i < synthmodnames::MOD_LAST; i++) 
		{
			if (i != synthmodnames::MOD_NONEZERO) {
				ip++;
				string modname = modnames->get_name
					((synthmodnames::SYNTH_MOD_TYPE)i);
				int strlen = modname.length();
				err_msg+=modnames->get_name((synthmodnames::SYNTH_MOD_TYPE)i);
				if (ip % 4 == 0) err_msg += "\n"; 
				else {
					if (strlen < 8)	err_msg += "\t\t";
						else err_msg += "\t";
				}
			}
		}
		err_msg += "\n";
		return false;
	}
	synthmod* sm = synthmod::get_modlist()->create_module(smt, "username");
	err_msg = "\n" + modnames->get_name(smt);
	if (sm == 0) {
		err_msg += " module has not been fully\nincorporated into the ";
		err_msg += "wcnt user interface.  Oops! \n Send an email to ";
		err_msg += "james@jwm-art.net to complain.\n";
		return false;
	}
	err_msg += ("\n" + *sm->get_username());
	switch(smt)
	{
		case synthmodnames::MOD_ADSR:
			err_msg += "\n\tenvelope\n\t\tattack  uptime uplevel lotime ";
			err_msg += "lolevel\n\t\tdecay   uptime uplevel lotime lolevel";
			err_msg += "\n\t\trelease uptime uplevel lotime lolevel";
			err_msg += "\n\t\t// nb there can be any number of each section";
			err_msg += "\n\tenvelope";
			break;
		case synthmodnames::MOD_SEQUENCER:
			err_msg += "\n\ttrack\n\t\triff username barposition ";
			err_msg += "// bar 0 is first bar\n\t\t// add as many riffs as";
			err_msg += " you like.\n\ttrack";
			break;
		case synthmodnames::MOD_STEREOMIXER:
			err_msg += "\n\tmixdesk\n\t\tmix_chanusername\n\tmixdesk";
			err_msg += "\n// add as many mix_chan(nels) as you like!";
			break;
		case synthmodnames::MOD_USERWAVE:
			err_msg += "\n\tenvelope\n\t\tvertex updegree uplevel lodegree";
			err_msg += "lolevel\n\tenvelope\n// you will generally want at ";
			err_msg += "least three sections but more is possible";
			break;
		case synthmodnames::MOD_SWITCHER:
			err_msg += "\n\tsignals\n\t\twcnt_signalusername\n\tsignals\n//";
			err_msg += " you'll need atleast two signals to switch between";
			break;
		case synthmodnames::MOD_COMBINER:
			err_msg += "\n\tsignals\n\t\twcnt_signalusername\n\tsignals";
			break;
		case synthmodnames::MOD_TIMEMAP:
			err_msg += "\n\ttime\n\t\tbpm bpmfloat barposinteger";
			err_msg += "\n\t\tsignature 4/4 barposinteger // (or 3/4 etc)";
			err_msg += "\n\ttime";
			break;
		default:
			break;
	}
	modinputlist* inlist =
		synthmod::get_inputlist()->get_inputlist_for_module(sm);
	modinput* input = inlist->goto_first();
	inputnames* innames = synthmod::get_inputnames();
	if (!input)
		err_msg +="\n//this module has no inputs.";
	else {
		err_msg += ("\n//inputs for module: " + modnames->get_name(smt));
		while(input) {
			err_msg += ("\n\t" + innames->getname(input->getinputtype()));
			err_msg += "\toutputmodulename\toutputname";
			input = inlist->goto_next();
		}
	}
	modparamlist* parlist = 
		synthmod::get_paramlist()->get_paramlist_for_moduletype(smt);
	modparam* param = parlist->goto_first();
	paramnames* parnames = synthmod::get_paramnames();
	if (!param)
		err_msg +="\n//this module has no parameters.";
	else {
		err_msg += ("\n// parameters for module: " + modnames->get_name(smt));
		while(param) {
			err_msg += ("\n\t" + parnames->getname(param->get_paramtype()));
			switch(parnames->getcategory(param->get_paramtype())) 
			{
			case CAT_DOUBLE:
				err_msg += "\tfloatingpointvalue";
				break;
			case CAT_SHORT:
			case CAT_ULONG:
				err_msg += "\tintegervalue";
				break;
			case CAT_TRIG:
			case CAT_STATE:
				err_msg += "\ton/off";
				break;
			case CAT_FILENAME:
				err_msg += "\tfilename";
				break;
			case CAT_NOTENAME:
				err_msg += "\tnotename";
				break;
			case CAT_MOD_FUNC:
				err_msg += "\tadd/sub/mul/div/mod/sin/cos/tan/and/or/xor";
				break;
			case CAT_WAVFILEIN:
				err_msg += "\twavfileinfilename";
				break;
			case CAT_LOGIC:
				err_msg += "\tand/or/xor";
				break;
			case CAT_LOOP_MODE:
				err_msg += "\toff/fwd/rev/bi";
				break;
			case CAT_PLAY_DIR:
				err_msg += "\tfwd/rev";
				break;
			case CAT_PLAY_MODE:
				err_msg += "\tstop/wrap/bounce/jump";
				break;
			case CAT_JUMP_DIR:
				err_msg += "\tplay/loop";
				break;
			default:
				err_msg +="\t ***program error***";
			}
			param = parlist->goto_next();
		}
	}
	modoutputlist* outlist = 
		synthmod::get_outputlist()->get_outputlist_for_module(sm);
	modoutput* output = outlist->goto_first();
	outputnames* outnames = synthmod::get_outputnames();
	if (!output)
		err_msg +="\n//this module has no outputs.";
	else {
		err_msg += ("\n// outputs for module: " + modnames->get_name(smt));
		while(output) {
			err_msg += ("\n// " + outnames->getname(output->getoutputtype()));
			output = outlist->goto_next();
		}
	}
	err_msg += ("\n" + *sm->get_username() + "\n");
	return false;
}

bool jwmsynth::riff_help()
{
	err_msg += "\nriff username\n\tquarter_value int (see wcnt documentation";
	err_msg += "in quarter.txt)\n\tnote notename noteposition notelength";
	err_msg += "notevelocity\nusername\n";
	return false;
}

bool jwmsynth::input_help()
{ // decide by the number of options passed on command line whether to load a 
  // wcnt file or to create all possible modules in order to access outputs.
	string inpname = (options_count == 3) ? options[2] : options[3];
	inputnames::IN_TYPE intype = innames->get_type(&inpname);
	if (intype == inputnames::IN_FIRST) {
		err_msg =
			"\nUnknown input type :" + inpname 
			+ "\n available input types are:\n";
		int i;
		for (i = inputnames::IN_FIRST + 1; i < inputnames::IN_LAST; i++) {
			err_msg += (innames->getname((inputnames::IN_TYPE)i) + "\n");
		}
		return false;
	}
	err_msg = "";
	IOCAT incat = innames->getcategory(intype);
	if (options_count == 3) { // must create the modules 
		synthmod* sm;
		int i;
		for (i = synthmodnames::MOD_FIRST + 1; 
			i < synthmodnames::MOD_LAST; i++) 
		{
			if (i != synthmodnames::MOD_NONEZERO) {
				sm = synthmodslist->create_module(
					(synthmodnames::SYNTH_MOD_TYPE) i, 
					modnames->get_name((synthmodnames::SYNTH_MOD_TYPE)i));
				if (!sm) { 
					err_msg += "\nnot enough memory to process request.";
					return false;
				}
			}
		}
	} else { // read modules from file
		if (!generate_synth()) {
			err_msg = 
				"\nproblems reading synthfile:\n" + 
				err_msg + "\nproceeding anyway";
		}
	}
	err_msg += "\navailable outputs are:\n";
	// now get the outputs matching category of input
	modoutputlist* outputs = outputlist->list_of_category(incat);
	modoutput* output = outputs->goto_first();
	while(output)
	{ // this if statement prevents from displaying off off
	  // now it only displays one off - less confusing.
		if (output->getmoduletype() != synthmodnames::MOD_NONEZERO)
			err_msg += (*output->getsynthmodule()->get_username() + " ");
		err_msg += (outnames->getname(output->getoutputtype()) + "\n");
		output = outputs->goto_next();
	}
	delete outputs; // delete list we created
	return false;
}

bool jwmsynth::sample_info()
{
	wavfilein wavfile;
	string wavname = options[2];
	wavfile.open_wav(options[2]);
	switch(wavfile.get_status())
	{
		case WAV_STATUS_MEMERR:
			err_msg = "\nnot enough memory to open wav: " + wavname;
			break;
		case WAV_STATUS_OPENERR:
			err_msg = "\nerror occurred opening wav: " + wavname;
			break;
		case WAV_STATUS_WAVERR:
			err_msg = "\nwav: " + wavname + " is not a wav file";
			break;
		case WAV_STATUS_MODE_ERR:
			err_msg = "\nmode error with wav: " + wavname;
			break;
		case WAV_STATUS_NOT_FOUND:
			err_msg = "\nwav: " + wavname + " was not found";
			break;
		case WAV_STATUS_INIT:
			err_msg = "\nproblem initialising wav: " + wavname;
			break;
		case WAV_STATUS_OK:
			err_msg = "\nwav: " + wavname + " is only OK, not opened.";
			break;
		case WAV_STATUS_OPEN:
			err_msg = "\nheader info for wav: " + wavname + ":";
			break;
	}
	if (wavfile.get_status() != WAV_STATUS_OPEN) 
		return false;
	switch(wavfile.get_channel_status())
	{
		case WAV_CH_MONO:
			err_msg += "\n\tMono";
			break;
		case WAV_CH_STEREO:
			err_msg += "\n\tStereo";
			break;
		default:
			err_msg += "\n\tUnsupported number of channels\n";
	}
	switch(wavfile.get_bitrate())
	{
		case WAV_BIT_16:
			err_msg += "\t16 bit";
			break;
		case WAV_BIT_8:
			err_msg += "\t8 bit";
			// no break intended
		default:
			err_msg += "\nbitrate not supported.  While wcnt will let you use";
			err_msg += " this sample, the sampler output will be garbage!";
			break;
	}
	if (wavfile.get_bitrate()!= WAV_BIT_16)
		return false;
	ostringstream conv;
	conv << "\t" << wavfile.get_sample_rate() << " hz";
	conv << "\n\tlength: " << wavfile.get_length() << " samples (";
	conv << (double)wavfile.get_length() / audio_samplerate << " seconds)\n";
	err_msg += conv.str();
	return false;
}
#endif // ifndef BARE_MODULES
#endif
