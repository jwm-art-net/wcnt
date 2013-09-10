#ifndef JWMSYNTH_H
#include "../include/jwmsynth.h"

#ifndef BARE_MODULES
jwmsynth::jwmsynth(int const argc, char **const argv) : 
	version(0), iocatnames(0), modnames(0), innames(0), outnames(0),
	parnames(0), synthmodslist(0), inputlist(0), outputlist(0), paramlist(0),
	connectlist(0), synthfile(0), mdobjlist(0), dobj_names(0), dobj_list(0),
	dobjparam_list(0), dpar_names(0), exit_bar(0), in_bar_trig(0), in_bar(0),
	options_count(argc), options(argv)
{
	version = new char[5];
	strcpy(version, "1.1z");
	wcnt_id = "wcnt-";
	wcnt_id += version;
	wcnt_id += "/jwmsynth";
	synthmod::register_iocatnames(iocatnames = new iocat_names);
	synthmod::register_modnames(modnames = new synthmodnames);
	synthmod::register_inputnames(innames = new inputnames);
	synthmod::register_outputnames(outnames  = new outputnames);
	synthmod::register_paramnames(parnames = new paramnames);
	synthmod::register_inputlist(inputlist = new modinputlist);
	synthmod::register_outputlist(outputlist = new modoutputlist);
	synthmod::register_paramlist(paramlist = new modparamlist);
	dobj::register_iocatnames(iocatnames); // usefull.
	dobj::register_dobjnames(dobj_names = new dobjnames);
	dobj::register_dparlist(dobjparam_list = new dobjparamlist);
	dobj::register_dparnames(dpar_names = new dparnames);
	dobj::register_dobjlist(dobj_list = new dobjlist);
	dobj::register_dobjdobjlist(dobj_dobjlist = new dobjdobjlist);
	synthmod::register_moddobjlist(mdobjlist = new moddobjlist);
	// must register the various things above before creating 
	// synthmodlist as it creates a nonezero module which
	// must have access to the lists - or it will cause seg fault.
	synthmod::register_modlist(synthmodslist = new synthmodlist);
	synthmod::register_connectlist(connectlist = new connectorlist);
	synthfile = new synthfilereader;	
	valid = true;
}

jwmsynth::~jwmsynth()
{
	delete [] version;
	delete modnames;
	delete innames;
	delete outnames;
	delete parnames;
    delete synthmodslist;
    delete inputlist;
    delete outputlist;
	delete paramlist;
	delete connectlist;
	delete synthfile;
	delete mdobjlist;
	delete dobj_names;
	delete dobj_list;
	delete dobjparam_list;
	delete dpar_names;
	delete dobj_dobjlist;
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
        cout << "\n\nProccessing wcnt/jwmsynth: "<<options[wcntfile_option_no];
	}
    string const *com = synthfile->read_command();
    while (*com != wcnt_id) 
	{
        if (!com) {
            err_msg = synthfile->get_error_msg();
            return false;
        }
		if (dobj_names->get_type(com) != dobjnames::DOBJ_FIRST) {
			if (!read_and_create_dobj(com))
				return false;
		}
		else { // create synth module
			if (!read_and_create_synthmod(com))
				return false;
		}
        com = synthfile->read_command();
    }
	if (synthmodslist->get_first_of_type(synthmodnames::MOD_WCNT) == 0) {
		err_msg = "\nfile ";
		err_msg += options[wcntfile_option_no];
		err_msg += "does not contain wcnt_exit module.";
		err_msg += "\nWithout this module I don't know when to stop.";
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
		err_msg = "\nin module " + *mod->get_username();
		err_msg += " " + *mod->get_error_msg();
		return false;
	}
	if (!synthmodslist->add_module(mod)) {
		err_msg = "\ncould not add module " + *mod->get_username();
		err_msg += " to list.";
		return false;
	}
	if (synthfile->is_verbose())
		cout << "\nend " << *mod->get_username();
	return true;
}

bool jwmsynth::read_and_create_dobj(string const* com)
{
	dobj* dbj = synthfile->read_dobj(com);
	if (!dbj) {
		err_msg = synthfile->get_error_msg();
		return false;
	}
	if (!dbj->is_valid()) {
		err_msg = *dbj->get_error_msg();
		return false;
	}
	if (!dobj_list->add_dobj(dbj)) {
		err_msg = "\ncould not add data object to list.";
		return false;
	}
	if (synthfile->is_verbose())
		cout << "\nend " << *dbj->get_username();
	return true;
}

bool jwmsynth::connect_synth()
{
    if (synthfile->is_verbose()) {
        cout << "\n\nEnd wcnt/jwmsynth: " << options[wcntfile_option_no];
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
	cout << "\n\nProcessing " << options[wcntfile_option_no] << "\tone '";
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
	string author = "\nWritten By James W. Morris\ne:jameswmorris@users.soureforge.net\n";
	string wcnt = "\nwcnt ";
	string filename = " filename.wc ";
	string verbal = "--verbose";
		string v = "-v";
	string header = "--header";
	string modhelp = "--module-help"; 
		string mh = "-mh";
	string inputhelp = "--input-help";
		string ih = "-ih";
	string dobjhelp = "--data-object-help";
		string dh = "-dh";
	string sampleinfo = "--sample-info";
		string si = "-si";
	string note = "--note-info";
		string ni = "-ni";
	string freq = "--freq-info";
		string fi = "-fi";
	string or = " or ";
	string commandoptions = author + wcnt + filename;
	commandoptions += wcnt + verbal + or + v + filename;
	commandoptions += wcnt + header;
	commandoptions += wcnt + modhelp + or + mh;
	commandoptions += wcnt + modhelp + or + mh + " modulename";
	commandoptions += wcnt + dobjhelp + or + dh;
	commandoptions += wcnt + dobjhelp + or + dh + " dataobjectname";
	commandoptions += wcnt + inputhelp + or + ih + " inputname"; 
	commandoptions += wcnt + filename + inputhelp + or + ih + " inputname";
	commandoptions += wcnt + sampleinfo + or + si + " audiosample.wav";
	commandoptions += wcnt + note + or + ni + " note name";
	commandoptions += wcnt + freq + or + fi + " frequency  samplerate";
    if (options_count == 1) {
        err_msg = commandoptions + "\n";
        return false;
    } 
	else if (options_count == 2) {
		if (modhelp == options[1] || mh == options[1]) {
			module_help();
			return false;
		} else if (dobjhelp == options[1] || dh == options[1]) {
			dobj_help();
			return false;
		} else if (header == options[1]) {
			err_msg = "\nwcnt file header is " + wcnt_id;
			return false;
		}
        wcntfile_option_no = 1;
		return true; // should be a filename then
    }
	else if (options_count == 3) {
		if (inputhelp == options[1] || ih == options[1]) {
			input_help();
			return false;
		} else if (dobjhelp == options[1] || dh == options[1]) {
			dobj_help();
			return false;
		} else if (modhelp == options[1] || mh == options[1]) {
			module_help();
			return false;
		} else if (sampleinfo == options[1] || si == options[1]) {
			sample_info();
			return false;
		} else if (verbal == options[1] || v == options[1]) {
            synthfile->set_verbose();
            wcntfile_option_no = 2;
			return true;
        } else if (verbal == options[2] || v == options[2]) {
            synthfile->set_verbose();
            wcntfile_option_no = 1;
			return true;
		} else if (note == options[1] || ni == options[1]) {
			note_help();
			return false;
		}
	}
	else if (options_count == 4) {
		if (inputhelp == options[2] || ih == options[2]) {
			wcntfile_option_no = 1;
			input_help();
			return false;
        } else if (freq == options[1] || fi == options[1]) {
			freq_help();
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
    string tmp(options[wcntfile_option_no]);
    bool rtval = false;
    switch (synthfile->open_file(options[wcntfile_option_no], wcnt_id)) {
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

void jwmsynth::module_help()
{
	string smname = (options_count == 3) ? options[2] : "";
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
				string modname = modnames->
					get_name((synthmodnames::SYNTH_MOD_TYPE)i);
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
		return;
	}
	synthmod* sm = synthmodslist->create_module(smt, "username");
	err_msg = "\n" + modnames->get_name(smt);
	if (sm == 0) {
		err_msg += " module has not been fully\nincorporated into the ";
		err_msg += "wcnt user interface.  Oops! \n Send an email to ";
		err_msg += "james@jwm-art.net to complain.\n";
		return;
	}
	err_msg += ("\n" + *sm->get_username());
	dobj_help(smt);
	modinputlist* inlist = inputlist->get_inputlist_for_module(sm);
	modinput* input = inlist->goto_first();
	if (!input)
		err_msg +="\n// this module has no inputs";
	else {
		err_msg += ("\n// inputs for " + modnames->get_name(smt));
		while(input) {
			string in = innames->get_name(input->get_inputtype());
			err_msg += "\n\t" + in;
			if (in.length() < 8) err_msg += "\t\t\t";
			else if (in.length() < 16) err_msg += "\t\t";
			else err_msg += "\t";
			err_msg += "modulename\toutputname";
			input = inlist->goto_next();
		}
	}
	modparamlist* parlist = 
		synthmod::get_paramlist()->get_paramlist_for_moduletype(smt);
	modparam* param = parlist->goto_first();
	if (!param)
		err_msg +="\n// this module has no parameters";
	else {
		paramnames* parnames = synthmod::get_paramnames();
		err_msg += ("\n// parameters for " + modnames->get_name(smt));
		while(param) {
			paramnames::PAR_TYPE pt = param->get_paramtype();
			string pn = parnames->get_name(pt);
			err_msg += "\n\t" + pn;
			if (pn.length() < 8) err_msg += "\t\t\t";
			else if (pn.length() < 16) err_msg += "\t\t";
			else err_msg += "\t";
			err_msg += iocatnames->get_name(parnames->get_category(pt));
			param = parlist->goto_next();
		}
	}
	modoutputlist* outlist = 
		synthmod::get_outputlist()->get_outputlist_for_module(sm);
	modoutput* output = outlist->goto_first();
	if (!output)
		err_msg +="\n// this module has no outputs";
	else {
		outputnames* outnames = synthmod::get_outputnames();
		err_msg += ("\n// outputs for " + modnames->get_name(smt));
		while(output) {
			err_msg += ("\n// " + outnames->get_name(output->get_outputtype()));
			output = outlist->goto_next();
		}
	}
	err_msg += "\n" + *sm->get_username() + "\n";
	return;
}

void jwmsynth::dobj_help(synthmodnames::SYNTH_MOD_TYPE smt)
{
	moddobjlist* mdlist = mdobjlist->get_moddobjlist_for_moduletype(smt);
	moddobj* mdbj = mdlist->goto_first();
	if (!mdbj) {
		err_msg +="\n// this module has no data objects";
		delete mdlist;
		return;
	}
	err_msg += ("\n// data objects for module " + modnames->get_name(smt));
	while(mdbj) {
		dobjnames::DOBJ_TYPE dt = mdbj->get_dobj_type();
		err_msg += "\n\t" + dobj_names->get_name(dt);
		dobjdobjlist* ddlist = dobj_dobjlist->get_dobjdobjlist_for_dobjtype(dt);
		dobjdobj* dd = ddlist->goto_first();
		while(dd){
			dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
			err_msg += "\n\t\t" + dobj_names->get_name(sprogtype);
			delete dobj_list->create_dobj(sprogtype);
			dobj_help_params(sprogtype);			
			dd = ddlist->goto_next();
		}
		err_msg += "\n\t" + dobj_names->get_name(dt);
		delete ddlist;
		mdbj = mdlist->goto_next();
	}
	err_msg += "\n";
	delete mdlist;
	return;	
}

void jwmsynth::dobj_help()
{
	string dname = (options_count == 3) ? options[2] : "";
	dobjnames::DOBJ_TYPE dt = dobj_names->get_type(&dname);
	if (dt == dobjnames::DOBJ_FIRST) {
		// incorrect dobj name or no name specified
		if (options_count == 3) 
			err_msg = "\nno data object available named " + dname;
		err_msg += "\navailable data object types are\n\n";
		int i, ip = 0; // display data objects not defined within modules
		for (i = dobjnames::DOBJ_DEFLISTS + 1;i < dobjnames::DOBJ_SYNTHMOD;i++)
		{
			if (dobj_names->check_type((dobjnames::DOBJ_TYPE)i) == i) {
				dname = dobj_names->get_name((dobjnames::DOBJ_TYPE)i);
				int sl = dname.length();
				err_msg += dname;
				ip++;
				if (ip % 2 == 0) err_msg += "\n";
				else {
					if (sl < 8) err_msg += "\t\t";
					else err_msg += "\t";
				}
			}
		}
		err_msg += "\n";
		return;
	}
	delete dobj_list->create_dobj(dt);
	dobjdobjlist* ddlist = dobj_dobjlist->get_dobjdobjlist_for_dobjtype(dt);
	dobjdobj* dd = ddlist->goto_first();
	err_msg += "\n" + dobj_names->get_name(dt) + "\nusername";
	dobj_help_params(dt);
	while(dd) { 
		dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
		err_msg += "\n\t" + dobj_names->get_name(sprogtype);
		delete dobj_list->create_dobj(sprogtype);
		dobj_help_params(sprogtype);			
		dd = ddlist->goto_next();
	}
	err_msg += "\nusername";
	delete ddlist;
	return;
}

void jwmsynth::dobj_help_params(dobjnames::DOBJ_TYPE dt)
{
	dobjparamlist*dparlist=dobjparam_list->get_dobjparamlist_for_dobj_type(dt);
	dobjparam* dparam = dparlist->goto_first();
	while(dparam) {
		IOCAT iocat = dpar_names->get_category(dparam->get_dpartype());
		err_msg += "\n\t\t\t" + dpar_names->get_name(dparam->get_dpartype());
		dparam = dparlist->goto_next();
		err_msg += "\t" + iocatnames->get_name(iocat);
	}
	delete dparlist;
	return;
}

void jwmsynth::input_help()
{ // decide by the number of options passed on command line whether to load a 
  // wcnt file or to create all possible modules in order to access outputs.
	string inpname = (options_count == 3) ? options[2] : options[3];
	inputnames::IN_TYPE intype = innames->get_type(&inpname);
	if (intype == inputnames::IN_FIRST) {
		err_msg =
			"\nUnknown input type :" + inpname 
			+ "\navailable input types are:\n";
		int i;
		for (i = inputnames::IN_FIRST + 1; i < inputnames::IN_LAST; i++) {
			err_msg += (innames->get_name((inputnames::IN_TYPE)i) + "\n");
		}
		return;
	}
	err_msg = "";
	IOCAT incat = innames->get_category(intype);
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
					return;
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
		if (output->get_moduletype() != synthmodnames::MOD_NONEZERO)
			err_msg += (*output->get_synthmodule()->get_username() + " ");
		err_msg += (outnames->get_name(output->get_outputtype()) + "\n");
		output = outputs->goto_next();
	}
	delete outputs; // delete list we created
	return;
}

void jwmsynth::sample_info()
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
		return;
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
	if (wavfile.get_bitrate()!= WAV_BIT_16)	return;
	ostringstream conv;
	conv << "\t" << wavfile.get_sample_rate() << " hz";
	conv << "\n\tlength: " << wavfile.get_length() << " samples (";
	conv << (double)wavfile.get_length() / audio_samplerate << " seconds)\n";
	err_msg += conv.str();
	return;
}

void jwmsynth::note_help()
{
	if (!check_notename(options[2])) {
		err_msg = "\nInvalid note name";
		return;
	}
	ostringstream conv;
	conv << "\n\tfrequency " << note_to_freq(options[2]);
	err_msg = conv.str();
	return;
}

void jwmsynth::freq_help()
{
	float frequency = atof(options[2]);
	audio_samplerate = atoi(options[3]);
	if (frequency == 0) {
		err_msg = "\nInvalid frequency";
		return;
	}
	if (audio_samplerate == 0) {
		err_msg = "\nInvalid sample rate";
	}
	ostringstream conv;
	conv << "\n\tdeg_size " << freq_to_step(frequency);
	conv << "\n\tsamples  " << freq_to_samples(frequency);
	err_msg = conv.str();
	return;
}

#endif // ifndef BARE_MODULES
#endif
