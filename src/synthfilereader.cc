#ifndef SYNTHFILEREADER_H
#include "../include/synthfilereader.h"

synthfilereader::synthfilereader()
: filename(0), filestatus(NOT_FOUND), synthfile(0), buff(0), synthheader(0), 
  err_msg(0), conv(0), genstatus(HALTED), verbose(false)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    err_msg = new string;
    conv = new ostringstream;
}

synthfilereader::~synthfilereader()
{
    synthfile->close();
    delete filename;
    delete synthfile;
    delete buff;
    delete synthheader;
    delete err_msg;
    delete conv;
}

synthfilereader::FILE_STATUS synthfilereader::open_file(char *synthfilename)
{
    synthfile->open(synthfilename);
    if (!synthfile->is_open())
        filestatus = NOT_FOUND;
    else {
        *synthfile >> *synthheader;
        if (*synthheader == "wcnt/jwmsynth")
            filestatus = FILE_OPEN;
        else
            filestatus = INVALID_HEADER;
    }
    return filestatus;
}

bool
synthfilereader::read_header(unsigned long *samplerate, short *bpm,
                             short *beatspermeasure, short *beatvalue, short* exitbar)
{
    if (filestatus != FILE_OPEN) {
        genstatus = HALTED;
        *err_msg =
            "\nProgrammer Error - not coded it proper: contact the tw*t and give 'im some grief!!\n";
        return 0;
    }
    if (!skip_remarks()) {
        *err_msg = "Unexpected end of file.";
        genstatus = MASSIVE_ERROR;
        return false;
    }
    if (*buff == "samplerate") {
        if (!(*synthfile >> *samplerate)) {
            *err_msg = "Expected value for samplerate.";
            genstatus = HALTED;
            return false;
        }
        if (*samplerate > 4000 && *samplerate < 200000) {
            if (verbose)
                cout << "\nsamplerate set at " << *samplerate;
        } else {
            *conv << *samplerate;
            *err_msg =
                "Invalid samplerate: " + conv->str() +
                ". valid values between 4000 and 200000.";
            genstatus = HALTED;
            return false;
        }
    } else {
        *err_msg = "Expected 'samplerate' got " + *buff + " instead.";
        genstatus = HALTED;
        return false;
    }
    if (!skip_remarks()) {
        *err_msg = "Unexpected end of file.";
        genstatus = HALTED;
        return false;
    }
    if (*buff == "bpm") {
        if (!(*synthfile >> *bpm)) {
            *err_msg = "Expected value for bpm";
            genstatus = HALTED;
            return false;
        }
        if (*bpm >= 20 && *bpm <= 1000) {
            if (verbose)
                cout << "\nbpm set at " << *bpm;
        } else {
            *conv << *bpm;
            *err_msg =
                "Invalid bpm: " + conv->str() +
                ". valid values between 20 and 1000.";
            genstatus = HALTED;
            return false;
        }
    } else {
        *err_msg = "Expected 'bpm' got " + *buff + " instead.";
        genstatus = HALTED;
        return false;
    }
    if (!skip_remarks()) {
        *err_msg = "Unexpected end of file.";
        genstatus = MASSIVE_ERROR;
        return false;
    }
    if (*buff == "signature") {
        if (!(*synthfile >> *beatspermeasure)) {
            *err_msg =
                "Expected value for time signature - beats per measure.";
            genstatus = HALTED;
            return false;
        }
        if (*beatspermeasure < 1 || *beatspermeasure > 16) {
            *conv << *beatspermeasure;
            *err_msg =
                "Invalid time signature with beats per measure: "
                + conv->str() + ". valid value in range 1 to 16.";
            genstatus = HALTED;
            return false;
        }
        char ch;
        while (synthfile->get(ch)) {
            if (ch == '/')
                break;
        }
        if (synthfile->eof()) {
            *err_msg =
                "Unexpected end of file while scanning time signature.";
            genstatus = HALTED;
            return false;
        }
        if (!(*synthfile >> *beatvalue)) {
            *err_msg = "Expected value for time signature - beat value.";
            genstatus = HALTED;
            return false;
        }
        if (*beatvalue < 1 || *beatvalue > 128) {
            *conv << *beatvalue;
            *err_msg =
                "Invalid time signature with beat value: " +
                conv->str() + ". valid value in range 1 to 128.";
            genstatus = HALTED;
            return false;
        }
        if (verbose)
            cout << "\ntime signature set to " << *beatspermeasure
                << "/" << *beatvalue;
    } else {
        *err_msg = "Expected 'signature'.";
        genstatus = HALTED;
        return false;
    }
    if (!skip_remarks()) {
        *err_msg = "Unexpected end of file.";
        genstatus = MASSIVE_ERROR;
        return false;
    }
    if (*buff == "exit_bar") {
        if (!(*synthfile >> *exitbar)) {
            *err_msg = "Expected value for exit_bar.";
            genstatus = HALTED;
            return false;
        }
        if (*exitbar > 0 && *exitbar < 32766) {
            if (verbose)
                cout << "\nexit_bar set at " << *exitbar;
        } else {
            *conv << *exitbar;
            *err_msg =
                "Invalid exit_bar value: " + conv->str() +
                ". valid values between 1 and 32766.";
            genstatus = HALTED;
            return false;
        }
    } else {
        *err_msg = "Expected 'exit_bar' got " + *buff + " instead.";
        genstatus = HALTED;
        return false;
    }
    filestatus = FILE_READY;
    return true;
}

string const *
synthfilereader::read_command()
{
    if (filestatus != FILE_READY) {
        *err_msg = "\nFile not ready!\n";
        genstatus = HALTED;
        return 0;
    }
    if (!skip_remarks()) {
        *err_msg = "\nUnexpected end of file.\n";
        genstatus = MASSIVE_ERROR;
        return 0;
    }
    return buff;
}

synthmod *const
synthfilereader::read_synthmodule(string const *com)
{
	synthmodnames::SYNTH_MOD_TYPE smt = synthmod::get_modnames()->get_type(com);
	if (smt == synthmodnames::MOD_FIRST) {
		*err_msg = "\nUnrecognised wcnt/jwmsynth module: " + *com;
		return 0;
	}
	string modname;
	*synthfile >> modname;
	// interject here to check that a module with that name does not already exist!
	if (synthmod::get_modlist()->get_synthmod_by_name(&modname)){
		*err_msg = "\nsynth module already exists named: " + modname + " will not duplicate.";
		return 0;
	}
	synthmod* sm = 0;
	switch(smt)
	{
		case synthmodnames::MOD_ADSR:
			sm = new adsr(modname);
			break;
		case synthmodnames::MOD_STEREOAMP:
			sm = new stereo_amp(modname);
			break;
		case synthmodnames::MOD_CLOCK:
			sm = new clockclock(modname);
			break;
		case synthmodnames::MOD_CONSTMOD:
			sm = new constmod(modname);
			break;
		case synthmodnames::MOD_FREQGEN:
			sm = new freq_generator(modname);
			break;
		case synthmodnames::MOD_LFOCLOCK:
			sm = new lfo_clock(modname);
			break;
		case synthmodnames::MOD_LFOCONTROL:
			sm = new lfo_controller(modname);
			break;
		case synthmodnames::MOD_LPFILTER:
			sm = new lpfilter(modname);
			break;
		case synthmodnames::MOD_MODIFIER:
			sm = new modifier(modname);
			break;
		case synthmodnames::MOD_NOISEGEN:
			sm = new noise_generator(modname);
			break;
		case synthmodnames::MOD_NONEZERO:
			sm = 0; // absolutely no point in making more of these ever, so don't.
			break;
		case synthmodnames::MOD_OSCCLOCK:
			sm = new osc_clock(modname);
			break;
		case synthmodnames::MOD_SAMPLEHOLD:
			sm = new sample_hold(modname);
			break;
		case synthmodnames::MOD_SEQUENCER:
			sm = new sequencer(modname);
			break;
		case synthmodnames::MOD_SINEWAVE:
			sm = new sine_wave(modname);
			break;
		case synthmodnames::MOD_SQUAREWAVE:
			sm = new square_wave(modname);
			break;
		case synthmodnames::MOD_TRIGGER:
			sm = new trigger(modname);
			break;
		case synthmodnames::MOD_TRIWAVE:
			sm = new triangle_wave(modname);
			break;
		case synthmodnames::MOD_TRIWAVE2:
			sm = new triangle_wave2(modname);
			break;
		case synthmodnames::MOD_USERWAVE:
			sm = new user_wave(modname);
			break;
		case synthmodnames::MOD_SAMPLER:
			sm = new sampler(modname);
			break;
		case synthmodnames::MOD_WAVFILEOUT:
			sm = new wavfileout(modname);
			break;
		case synthmodnames::MOD_STEREOCHANNEL:
			sm = new stereo_channel(modname);
			break;
		case synthmodnames::MOD_STEREOMIXER:
			sm = new stereomixer(modname);
			break;
		case synthmodnames::MOD_RANDTRIGGER:
			sm = new randomtrigger(modname);
			break;
		case synthmodnames::MOD_LOGICTRIGGER:
			sm = new logictrigger(modname);
			break;
		case synthmodnames::MOD_SWITCHER:
			sm = new switcher(modname);
			break;
		case synthmodnames::MOD_WCNTSIGNAL:
			sm = new wcnt_signal(modname);
			break;
		case synthmodnames::MOD_HPFILTER:
			sm = new hpfilter(modname);
			break;
		case synthmodnames::MOD_COMBINER:
			sm = new combiner(modname);
			break;
		default:
			sm = 0;
	}
	if (!sm) {
		*err_msg = "\nmemory shortage for synth module: " + modname;
		genstatus = HALTED;
		return 0;
	}
	if (verbose)
		cout << "\nCreating synth module: " << *sm->get_username();
	bool dataread = false;
	switch(smt)
	{// modules with data to be read, besides inputs and params
		case synthmodnames::MOD_ADSR:
			dataread = read_adsr_envelope((adsr*)sm);
			break;
		case synthmodnames::MOD_SEQUENCER:
			dataread = read_sequencer_riffs((sequencer*)sm);
			break;
		case synthmodnames::MOD_STEREOMIXER:
			dataread = read_mixer_channels((stereomixer*)sm);
			break;
		case synthmodnames::MOD_USERWAVE:
			dataread = read_userwave_envelope((user_wave*)sm);
			break;
		case synthmodnames::MOD_SWITCHER:
			dataread = read_signals(sm);
			break;
		case synthmodnames::MOD_COMBINER:
			dataread = read_signals(sm);
			break;
		default:
			dataread = true;
	}
	if (!dataread) {
		genstatus = HALTED;
		delete sm;
		return 0;
	}
	if (!read_inputs(sm)){
		genstatus = HALTED;
		delete sm;
		return 0;
	}
	if (!read_params(sm)){
		genstatus = HALTED;
		delete sm;
		return 0;
	}
	com = read_command();
	if (*com != modname) {
		*err_msg = 
			"\nexpected module definition terminator: " + modname
			+ " got :" + *com + " instead.";
		genstatus = HALTED;
		delete sm;
		return 0;
	}
	return sm;
}

// bool synthfilereader::read_inputs(synthmod* sm)
//--------------------------------------------------------------------------
// read the inputs for the module.  
// return true if no problems occurred reading inputs
// return false if problems arose.
// if the module does not have inputs, it should not be a problem: return true.
bool synthfilereader::read_inputs(synthmod* sm)
{
	modinputlist* inlist = synthmod::get_inputlist()->get_inputlist_for_module(sm);
	if (!inlist) {
		*err_msg = "\nproblems getting inputs for module: " + *sm->get_username();
		return false;
	}
	modinput* inp = inlist->goto_first();
	inputnames* innames = synthmod::get_inputnames();
	outputnames* outnames = synthmod::get_outputnames();
	string inputname;
	while(inp) { // step through each  input for module
		inputname = *read_command();
		if (innames->getname(inp->getinputtype()) != inputname){
			*err_msg = 
				"\nExpected input type: " + innames->getname(inp->getinputtype()) 
				+ " for module: " + *sm->get_username() + " got " + inputname + " instead.";
			delete inlist;
			return false;
		} else {
			inputnames::IN_TYPE input_type = innames->get_type(&inputname);
			if (input_type == inputnames::IN_FIRST){
				*err_msg = 
					"\nUnknown input type: " + inputname + " for module: " 
					+ *sm->get_username();
				delete inlist;
				return false;
			}
			string outmodname;
			*synthfile >> outmodname;
			if (outmodname == "off") {
				outputnames::OUT_TYPE offout = outnames->get_nonezerotype(innames->getcategory(input_type));
				connector* connect = new connector(sm, input_type, outmodname, offout);
				if (!connect) {
					*err_msg = 
						"\ncould not create connector for: off 'output' \nin module: "
						+ *sm->get_username() + " input: " + inputname;
					delete inlist;
					return false;
				}
				if (!synthmod::get_connectlist()->add_connector(connect)) {
					*err_msg = 
						"\ncould not add connector object to list for: off 'output' \nin module: "
						+ *sm->get_username() + " input:" + inputname;
					delete inlist;
					delete connect;
					return false;
				}
				if (verbose)
					cout << "\nadded connector: " << inputname << "\toff";
			}
			else
			{
				string outputname;
				*synthfile >> outputname;
				outputnames::OUT_TYPE output_type = outnames->get_type(&outputname);
				if (output_type == outputnames::OUT_FIRST) {
					*err_msg = 
						"\nInput: " + inputname + " in module: " + *sm->get_username() +
						" set with unknown output type: " + outputname;
					delete inlist;
					return false;
				}
				if (innames->getcategory(input_type) != outnames->getcategory(output_type)){
					*err_msg = 
						"\nInput: " + inputname + " in module: " + *sm->get_username()
						+ " set with output type of wrong category: " + outputname;
					delete inlist;
					return false;
				}
				connector* connect = new connector(sm, input_type, outmodname, output_type);
				if (!connect) {
					*err_msg = 
						"\ncould not create connector object: " + *sm->get_username() +
						" " + inputname + " " + outmodname + " " + outputname;
					delete inlist;
					return false;
				}
				if (!synthmod::get_connectlist()->add_connector(connect)) {
					*err_msg = 
						"\ncould not add connector object to list: " + *sm->get_username() +
						" " + inputname + " " + outmodname + " " + outputname;
					delete inlist;
					delete connect;
					return false;
				}
				if (verbose){
					cout << "\nadded connector: " << inputname << "\t";
					cout << outmodname << "\t" << outputname;
				}
			}
		}
		inp = inlist->goto_next();
	}
	delete inlist;
	return true;
}

// same rules apply for read_params(..) as read_inputs(..)
// a module without params is not an error.
bool synthfilereader::read_params(synthmod* sm)
{
	modparamlist* parlist = 
		synthmod::get_paramlist()->get_paramlist_for_moduletype(sm->get_module_type());
	if (!parlist) {
		*err_msg = "\nproblems getting params for module: " + *sm->get_username();
		return false;
	}
	modparam* param = parlist->goto_first();
	paramnames* parnames = synthmod::get_paramnames();
	string paramname;
	while(param) {
		paramname = *read_command();
		if (parnames->getname(param->get_paramtype()) != paramname) {
			*err_msg = 
				"\nexpected parameter: " + parnames->getname(param->get_paramtype()) 
				+ " for module: " + *sm->get_username() + " got: " 
				+ paramname	+ " instead";
			delete parlist;
			return false;
		} else {
			paramnames::PAR_TYPE paramtype = parnames->get_type(&paramname);
			void* data = read_param_value(paramtype);
			if (!data) {
				*err_msg += " in module: " + *sm->get_username();
				genstatus = HALTED;
				return false;
			}
			if (!sm->set_param(paramtype, data)){
				destroy_tmp_param_data(paramtype, data);
				*err_msg = 
					"\nmodule :" + *sm->get_username() + " denied owning parmeter of type: "
					+ paramname;
				genstatus = HALTED;
				return false;
			}
			destroy_tmp_param_data(paramtype, data);
			if (verbose) {
				cout << "\nset module: " << *sm->get_username() << " parameter: ";
				cout << paramname << " to mysterious value";
			}
		}
		param = parlist->goto_next();
	}
	delete parlist;
	return true;
}

void* synthfilereader::read_param_value(paramnames::PAR_TYPE pt)
{
	paramnames* parnames = synthmod::get_paramnames();
	string svalue;
	char* cstr = 0;
	void* data = 0;
	conv->flush();
	switch(synthmod::get_paramnames()->getcategory(pt))
	{
		case CAT_DOUBLE:
			data = new double;
			if (!(*synthfile >> *(double*)data)) {
				*err_msg = "\nexpected floating point value for parameter: " + parnames->getname(pt);
				delete (double*)data;
				data = 0;
			}
			break;
		case CAT_SHORT:
			data = new short;
			if (!(*synthfile >> *(short*)data)) {
				*err_msg = "\nexpected integer value for parameter: " + parnames->getname(pt);
				delete (short*)data;
				data = 0;
			}
			break;
		case CAT_ULONG:
			data = new unsigned long;
			if (!(*synthfile >> *(unsigned long*)data)) {
				*err_msg = "\nexpected unsigned value for parameter: " + parnames->getname(pt);
				delete (unsigned long*)data;
				data = 0;
			}
			break;
		case CAT_TRIG:  // fall through to cat_state: different categories in IOCAT
		case CAT_STATE: // but use same data type (enum STATUS) in usage.
			data = new STATUS;
			*synthfile >> svalue;
			if (svalue == "on") 		*(STATUS*)data = ON;
			else if (svalue == "off")*(STATUS*)data = OFF;
			else {
				*err_msg = 
					"\nexpected on or off value for parameter: " + parnames->getname(pt)
					+ " got " + svalue + " instead.";
				delete (STATUS*)data;
				data = 0;
			}
			break;
		case CAT_FILENAME:
			if (!(*synthfile >> svalue)) {
				*err_msg = 
					"\nunable to read filename for parameter: " 
					+ parnames->getname(pt);
				data = 0;
			} else {
				cstr = new char[svalue.length() + 1];
				strncpy(cstr, svalue.c_str(), svalue.length());
				cstr[svalue.length()] = 0;
				data = cstr;
			}
			break;
		case CAT_NOTENAME:
			if (!(*synthfile >> svalue)) {
				*err_msg = 
					"\nunable to read notename for parameter: "
					+ parnames->getname(pt);
				data = 0;
			} else {
				cstr = new char[svalue.length() + 1];
				strncpy(cstr, svalue.c_str(), svalue.length());
				cstr[svalue.length()] = 0;
				data = cstr;
			}
			break;
		case CAT_MOD_FUNC:	// enum modifier::MOD_FUNC
			data = new modifier::MOD_FUNC;
			*synthfile >> svalue;
			if (svalue == "add") 		*(modifier::MOD_FUNC*)data = modifier::ADD;
			else if (svalue == "sub")*(modifier::MOD_FUNC*)data = modifier::SUB;
			else if (svalue == "mul")*(modifier::MOD_FUNC*)data = modifier::MUL;
			else if (svalue == "div")*(modifier::MOD_FUNC*)data = modifier::DIV;
			else if (svalue == "mod")*(modifier::MOD_FUNC*)data = modifier::MOD;
			else if (svalue == "sin")*(modifier::MOD_FUNC*)data = modifier::SIN;
			else if (svalue == "cos")*(modifier::MOD_FUNC*)data = modifier::COS;
			else if (svalue == "tan")*(modifier::MOD_FUNC*)data = modifier::TAN;
			else {
				*err_msg = 
					"\nunrecognised modifier function: " + svalue
					+ " for parameter: " + parnames->getname(pt);
				delete (modifier::MOD_FUNC*)data;
				data = 0;
			}
			break;
		case CAT_CLIP_MODE:	// enum stereo_amp::CLIP_MODE
			data = new stereo_amp::CLIP_MODE;
			*synthfile >> svalue;
			if (svalue == "clip") 
				*(stereo_amp::CLIP_MODE*)data = stereo_amp::CLIP;
			else if (svalue == "invert_clip")
				*(stereo_amp::CLIP_MODE*)data = stereo_amp::INVERT_CLIP;
			else {
				*err_msg = 
					"\nunrecognised stereo_amp clip_mode: " + svalue
					+ " for parameter: " + parnames->getname(pt);
				delete (stereo_amp::CLIP_MODE*)data;
				data = 0;
			}
			break;
		case CAT_LOOP_MODE:	// enum sampler::LOOP_MODE
			data = new sampler::LOOP_MODE;
			*synthfile >> svalue;
			if (svalue == "off")
				*(sampler::LOOP_MODE*)data = sampler::LOOP_OFF;
			else if (svalue == "on" || svalue == "bi")
				*(sampler::LOOP_MODE*)data = sampler::LOOP_ON;
			else {
				*err_msg = 
					"\nunrecognised sampler loop_mode: " + svalue
					+ " for parameter: " + parnames->getname(pt);
				delete (sampler::LOOP_MODE*)data;
				data = 0;
			}
			break;
		case CAT_WAVFILEIN:
			if (!(*synthfile >> svalue)) {
				*err_msg = 
					"\nunable to read wavfilein name for parameter: "
					+ parnames->getname(pt);
				data = 0;
			} else {
				data = sampler::get_wavfilein_list()->get_wavfilein_by_name(&svalue);
				if (!data) {
					*err_msg = 
					"\nno wavfilein open named: " + svalue;
					data = 0;
				}
			}
			break;
		case CAT_LOGIC:
			data = new logictrigger::LOGIC_FUNC;
			*synthfile >> svalue;
			if (svalue == "and") 		*(logictrigger::LOGIC_FUNC*)data = logictrigger::AND;
			else if (svalue == "or")	*(logictrigger::LOGIC_FUNC*)data = logictrigger::OR;
			else if (svalue == "xor")*(logictrigger::LOGIC_FUNC*)data = logictrigger::XOR;
			else {
				*err_msg = 
					"\nunrecognised logic function: " + svalue
					+ " for parameter: " + parnames->getname(pt);
				delete (logictrigger::LOGIC_FUNC*)data;
				data = 0;
			}
			
		default:
			*err_msg = 
				"\nunable to read value for parameter: " 
				+ parnames->getname(pt) + " requested invalid parameter category";
			data = 0;
	}
	return data;
}

void synthfilereader::destroy_tmp_param_data(paramnames::PAR_TYPE pt, void* data)
{
	switch(synthmod::get_paramnames()->getcategory(pt))
	{
		case CAT_DOUBLE:
			delete (double*)data;
			break;
		case CAT_SHORT:
			delete (short*)data;
			break;
		case CAT_ULONG:
			delete (unsigned long*)data;
			break;
		case CAT_TRIG:  // fall through to cat_state: different categories in IOCAT
		case CAT_STATE: // but use same data type (enum STATUS) in usage.
			delete (STATUS*)data;
			break;
		case CAT_FILENAME: // fall through
		case CAT_NOTENAME:
			delete[] (char*)data;
			break;
		case CAT_MOD_FUNC:	// enum modifier::MOD_FUNC
			delete (modifier::MOD_FUNC*)data;
			break;
		case CAT_CLIP_MODE:	// enum stereo_amp::CLIP_MODE
			delete (stereo_amp::CLIP_MODE*)data;
			break;
		case CAT_LOOP_MODE:	// enum sampler::LOOP_MODE
			delete (sampler::LOOP_MODE*)data;
			break;
		case CAT_WAVFILEIN:
			// don't want to delete data for wavfilein as it was not
			// created by read_param_value(..) - unlike the above
			break;
		case CAT_LOGIC:
			delete (logictrigger::LOGIC_FUNC*)data;
			break;
		default:
			break;
	}
}

bool synthfilereader::read_adsr_envelope(adsr* env)
{
	if (env->get_module_type() != synthmodnames::MOD_ADSR) {
		*err_msg = 
			"programmer error: requested to read adsr envelope into non adsr module: " 
			+ *env->get_username() + " of type: " + 
			synthmod::get_modnames()->get_name(env->get_module_type());
		return false;
	}
	string const* envelope;
	envelope = read_command();
	if (*envelope != "envelope") {
		*err_msg = 
			"\nin module: " + *env->get_username() + " expected: envelope got:"
			+ *envelope + " instead.";
		return false;
	}
	adsr_coord* coord = 0;
	adsr_coord::SECT sect[3] = { 
		adsr_coord::ADSR_ATTACK, 
		adsr_coord::ADSR_DECAY, 
		adsr_coord::ADSR_RELEASE 
	};
	string sectname[3] = { "attack", "decay", "release" };
	short i;
	string const* com = read_command();
	for (i = 0; i < 3; i++)
	{
		if (*com != sectname[i]) {
			*err_msg = "\nExpected adsr section: " + sectname[i] + " got " + *com + " instead.";
			return false;
		}
		while (*com == sectname[i])
		{
			if (!(coord = read_adsr_coord(sect[i]))) {
				*err_msg += (" for section: " + sectname[i] + " in adsr: " + *env->get_username());
				return false;
			}
			if (!env->insert_coord(coord)) {
				*err_msg = "\nUnable to add adsr section: " + sectname[i] + " to adsr: " + *env->get_username();
				return false;
			}
			if (verbose) {
				cout << "\nAdded adsr section: " << sectname[i];
				cout << " " << coord->get_upper_time() << " " << coord->get_upper_level();
				cout << " " << coord->get_lower_time() << " " << coord->get_lower_level();
			}
			com = read_command();
		}
	}
	if (*com != "envelope") {
		*err_msg = 
			"\nafter envelope shape definition in module: " + *env->get_username() + " expected: envelope got:"
			+ *envelope + " instead.";
		return false;
	}
	return true;
}

bool synthfilereader::read_sequencer_riffs(sequencer* seq)
{
	if (seq->get_module_type() != synthmodnames::MOD_SEQUENCER) {
		*err_msg = 
			"programmer error: requested to read riffs into non sequencer module: " 
			+ *seq->get_username() + " of type: " + 
			synthmod::get_modnames()->get_name(seq->get_module_type());
		return false;
	}
	string const *track = read_command();
	if (*track != "track") {
		*err_msg = 
			"\nin module: " + *seq->get_username() + " expected: track got:"
			+ *track + " instead.";
		return false;
	}
    string const *rcom = read_command();
	riffdata* riff;
    while (*rcom != "track") 
	{
		riff = sequencer::get_rifflist()->get_riff_by_name(rcom);
		if (!riff) {
			*err_msg = "\nNo riff previously defined as: " + *rcom;
			return false;
		}
		short start_bar;
		if (!(*synthfile >> start_bar)) {
			*err_msg = 
				"\nExpected start bar value for riff: " 
				+ *riff->get_username() + " in sequencer:" + *seq->get_username();
			return false;
		}
		if (start_bar < 0 || start_bar > 32766) {
			*err_msg = 
				"\nInvalid start bar value for riff: " + *rcom 
				+ " in sequencer: " + *seq->get_username();
			return false;
		}
		if (!seq->add_riff(riff, start_bar)) {
			*conv << start_bar;
			*err_msg = 
				"\nCould not add riff: " + *riff->get_username() + " at bar " 
				+ conv->str() + " - possibly a riff in that position already.";
			return false;
		}
        if (verbose)
			cout << "\nAdded riff: " << *riff->get_username() << " at bar: " << start_bar;
		rcom = read_command();
    }
	return true;
}

bool synthfilereader::read_mixer_channels(stereomixer* mix)
{
	if (mix->get_module_type() != synthmodnames::MOD_STEREOMIXER) {
		*err_msg = 
			"programmer error: requested to read mixer channel into non mixer module: " 
			+ *mix->get_username() + " of type: " + 
			synthmod::get_modnames()->get_name(mix->get_module_type());
		return false;
	}
	string const* desk = read_command();
	if (*desk != "desk") {
		*err_msg = 
			"\nin module: " + *mix->get_username() + " expected: desk got:"
			+ *desk + " instead.";
		return false;
	}
    string const *rcom = read_command();
	stereo_channel* chan = 0;
	synthmodlist* modlist = synthmod::get_modlist();
	synthmodnames* modnames = synthmod::get_modnames();
    while (*rcom != "desk") 
	{
		chan = (stereo_channel*)modlist->get_synthmod_by_name(rcom);
		if (!chan) {
			*err_msg = "\nNo mixer channel previously defined as: " + *rcom;
			return false;
		}
		if (chan->get_module_type() != synthmodnames::MOD_STEREOCHANNEL) {
			*err_msg = 
				"\nrequested mixer channel for mixer, got: " +
				modnames->get_name(chan->get_module_type()) + " named: " 
				+ *chan->get_username();
			return false;
		}
		if (!mix->add_channel(chan)) {
			*err_msg = 
				"\nCould not add mixer chanel: " + *chan->get_username() + " to mixer";
			return false;
		}
        if (verbose)
			cout << "\nAdded mixer channel: " << *chan->get_username();
		rcom = read_command();
    }
	return true;
}


bool synthfilereader::read_signals(synthmod* mod)
{
	synthmodnames::SYNTH_MOD_TYPE modtype = mod->get_module_type();
	synthmodnames* modnames = synthmod::get_modnames();
	string modtypename = modnames->get_name(modtype);
	combiner* comb = 0;
	switcher* swtch = 0;
	if (modtype == synthmodnames::MOD_SWITCHER)
		swtch = (switcher*)mod;
	else if (modtype == synthmodnames::MOD_COMBINER) 
		comb = (combiner*)mod;
	else {
		*err_msg = 
			"programmer error: requested to read wcntsignal into invalid module type: "
			+ *mod->get_username() + " of type: " + modtypename;
		return false;
	}
	string const* signals = read_command();
	if (*signals != "signals") {
		*err_msg = 
			"\nin module: " + *mod->get_username() + " expected: signals got:"
			+ *signals + " instead.";
		return false;
	}
    string const *rcom = read_command();
	wcnt_signal* wcntsig = 0;
	synthmodlist* modlist = synthmod::get_modlist();
    while (*rcom != "signals") 
	{
		wcntsig = (wcnt_signal*)modlist->get_synthmod_by_name(rcom);
		if (!wcntsig) {
			*err_msg = "\nNo wcnt_signal previously named: " + *rcom;
			return false;
		}
		if (wcntsig->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
			*err_msg = 
				"\nrequested wcnt_signal for " + modtypename + ", got: " +
				modnames->get_name(wcntsig->get_module_type()) + " named: " 
				+ *wcntsig->get_username();
			return false;
		}
		wcnt_signal* tmp;
		if (comb) 
			tmp = comb->add_signal(wcntsig); 
		else
			tmp = swtch->add_signal(wcntsig);
		if (!tmp) {
			*err_msg = 
				"\nCould not add wcnt_signal: " + *wcntsig->get_username() + " to "
				+ modtypename + ": " + *mod->get_username();
			return false;
		}
        if (verbose)
			cout << "\nAdded wcnt_signal: " << *wcntsig->get_username();
		rcom = read_command();
    }
	return true;
}


bool synthfilereader::read_switcher_signals(switcher* swtch)
{
	if (swtch->get_module_type() != synthmodnames::MOD_SWITCHER) {
		*err_msg = 
			"programmer error: requested to read wcntsignal into invalid module type: "
			+ *swtch->get_username() + " of type: " + 
			synthmod::get_modnames()->get_name(swtch->get_module_type());
		return false;
	}
	string const* signals = read_command();
	if (*signals != "signals") {
		*err_msg = 
			"\nin module: " + *swtch->get_username() + " expected: signals got:"
			+ *signals + " instead.";
		return false;
	}
    string const *rcom = read_command();
	wcnt_signal* wcntsig = 0;
	synthmodlist* modlist = synthmod::get_modlist();
	synthmodnames* modnames = synthmod::get_modnames();
    while (*rcom != "signals") 
	{
		wcntsig = (wcnt_signal*)modlist->get_synthmod_by_name(rcom);
		if (!wcntsig) {
			*err_msg = "\nNo swtcher wcnt_signal previously defined as: " + *rcom;
			return false;
		}
		if (wcntsig->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
			*err_msg = 
				"\nrequested swtcher wcnt_signal for swtcher, got: " +
				modnames->get_name(wcntsig->get_module_type()) + " named: " 
				+ *wcntsig->get_username();
			return false;
		}
		if (!swtch->add_signal(wcntsig)) {
			*err_msg = 
				"\nCould not add wcnt_signal: " + *wcntsig->get_username() + " to swtcher";
			return false;
		}
        if (verbose)
			cout << "\nAdded wcnt_signal: " << *wcntsig->get_username();
		rcom = read_command();
    }
	return true;
}


bool synthfilereader::read_userwave_envelope(user_wave* uwv)
{
	if (uwv->get_module_type() != synthmodnames::MOD_USERWAVE) {
		*err_msg = 
			"programmer error: requested to read user_wave envelope into non user_wave module: " 
			+ *uwv->get_username() + " of type: " + 
			synthmod::get_modnames()->get_name(uwv->get_module_type());
		return false;
	}
	string const* envelope;
	envelope = read_command();
	if (*envelope != "envelope") {
		*err_msg = 
			"\nin module: " + *uwv->get_username() + " expected: envelope got:"
			+ *envelope + " instead.";
		return false;
	}
	string const* com = read_command();
	wave_vertex* vert = 0;
	while (*com != "envelope") 
	{
		if (*com != "vertex") {
			*err_msg = 
				"\nin module: " + *uwv->get_username() + " expected vertex got: " 
				+ *com + " instead.";
			return false;
		}
		if (!(vert = read_wave_vertex())) {
			*err_msg += " in module: " + *uwv->get_username();
			return false;
		}
		if (!(uwv->add_vertex(vert))) {
			*err_msg = "\nunable to add vertex to user_wave";
			delete vert;
			return false;
		}
		if (verbose) {
			cout << "\nadded vertex: " << vert->get_upper()->get_position() << " ";
			cout << vert->get_upper()->get_level() << " ";
			cout << vert->get_lower()->get_position() << " ";
			cout << vert->get_lower()->get_level();
		}
		com = read_command();
	}
	return true;
}

wave_vertex* synthfilereader::read_wave_vertex()
{
	double udeg, ul, ldeg, ll;
	if (!(*synthfile >> udeg)) {
		*err_msg = "\nExpected value for upper degree pos";
		return 0;
	}
	if (udeg < 0) {
		*err_msg = "\nInvalid (negative) value for upper degree pos";
		return 0;
	}
	if (!(*synthfile >> ul)) {
		*err_msg = "\nExpected value for upper level";
		return 0;
	}
	if (!(*synthfile >> ldeg)) {
		*err_msg = "\nExpected value for lower degree pos";
		return 0;
	}
	if (ldeg < 0) {
		*err_msg = "\nInvalid (negative) value for lower degree pos";
		return 0;
	}
	if (!(*synthfile >> ll)) {
		*err_msg = "\nExpected value for lower level";
		return 0;
	}
	return new wave_vertex(udeg, ul, ldeg, ll);
}

adsr_coord* synthfilereader::read_adsr_coord(adsr_coord::SECT sect)
{
	double ut, ul, lt, ll;
	if (!(*synthfile >> ut)) {
		*err_msg = "\nExpected value for upper time";
		genstatus = HALTED;
		return 0;
	}
	if (ut < 0) {
		*err_msg = "\nInvalid value for upper time (negative)";
		genstatus = HALTED;
		return 0;
	}
	if (!(*synthfile >> ul)) {
		*err_msg = "\nExpected value for upper level";
		genstatus = HALTED;
		return 0;
	}
	if (!(*synthfile >> lt)) {
		*err_msg = "\nExpected value for lower time";
		genstatus = HALTED;
		return 0;
	}
	if (lt < 0) {
		*err_msg = "\nInvalid value for lower time (negative)";
		genstatus = HALTED;
		return 0;
	}
	if (!(*synthfile >> ll)) {
		*err_msg = "\nExpected value for lower level";
		genstatus = HALTED;
		return 0;
	}
	return new adsr_coord(sect, ut, ul, lt, ll);
}

riffdata *const synthfilereader::read_riffdata(string const *com)
{
    if (*com != "riff") {
        *err_msg = "\n command: " + *com + " issued to riffdata reader.";
        genstatus = MASSIVE_ERROR;
        return 0;
    }
    string riffname;
    *synthfile >> riffname;
    riffdata *riff = new riffdata(riffname);
    if (!riff) {
        *err_msg = "\nmemory shortage for new riff: " + riffname;
        genstatus = HALTED;
        return 0;
    }
    if (verbose)
        cout << "\nCreating riff: " << *riff->get_username();
    note_data *note;
    string const *rcom = read_command();
    while (*rcom != riffname) {
        if (*rcom == "note") {
            if (!(note = read_notedata())) {
                genstatus = HALTED;
                delete riff;
                return 0;
            }
            if (!riff->insert_and_position_note(note)) {
                *err_msg = "\nProblems inserting note in riff";
                genstatus = HALTED;
                delete note;
                delete riff;
                return 0;
            }
            if (verbose)
                cout << "\nadded note: " << note->
                    get_name() << " pos: " << note->
                    get_position() << " len: " << note->
                    get_length() << " vel:" << note->get_velocity();
        } else {
            *err_msg = "\nexpected 'note' got '" + *rcom + "' instead.";
            genstatus = HALTED;
            delete riff;
            return 0;
        }
        rcom = read_command();
    }
    return riff;
}

note_data *const synthfilereader::read_notedata()
{
    string name;
    *synthfile >> name;
    if (checknotename(name.c_str()) != 0) {
        *err_msg = "\nInvalid note name:" + name;
        genstatus = HALTED;
        return 0;
    }
    short nlen;
    short npos;
    double vel;
    if (!(*synthfile >> npos)) {
        *err_msg = "\nExpected position value for note " + name;
        return 0;
    }
    if (npos < 0 || npos > 32766) {
        *conv << npos;
        *err_msg =
            "\nInvalid position value in note " + name + " " +
            conv->str() + " valid range 0 to 32767";
        return 0;
    }
    if (!(*synthfile >> nlen)) {
        *err_msg = "\nExpected length value for note " + name;
        return 0;
    }
    if (nlen < 1 || nlen > 32766) {
        *conv << nlen;
        *err_msg = "\nInvalid length value for note " + name + " " +
					conv->str() + " valid range 1 to 32767";
        return 0;
    }
    if (!(*synthfile >> vel)) {
        *err_msg = "\nExpected velocity value for note " + name;
        return 0;
    }
    if (vel < 0.00 || vel > 1.00) {
        *conv << vel;
        *err_msg =
            "\nInvalid velocity value for note " + name + " " +
            conv->str() + " valid range 0.00 to 1.00";
        return 0;
    }
    note_data *newnote = new note_data(name.c_str(), nlen, npos, vel);
    if (!newnote) {
        *err_msg = "\nNot enough memory for note:" + name;
        *conv << npos;
        *err_msg += conv->str();
        *conv << nlen;
        *err_msg += conv->str();
        *conv << vel;
        *err_msg += conv->str();
        return 0;
    }
    return newnote;
}

wavfilein *const synthfilereader::read_wavfilein(string const *com)
{
    if (*com != "wavfilein") {
        *err_msg = "\n command: " + *com + " issued to wavfilein reader.";
        genstatus = MASSIVE_ERROR;
        return 0;
    }
    string wavname;
    *synthfile >> wavname;
	wavfilein* wavfile = new wavfilein;
	if (!wavfile) {
		*err_msg = "\nnot enough memory for wavfilein: " + wavname;
	}
	if (wavfile->open_wav(wavname.c_str()) != WAV_STATUS_OPEN) {
		switch(wavfile->get_status())
		{
			case WAV_STATUS_MEMERR:
				*err_msg = "\nnot enough memory to open wav: " + wavname;
				break;
			case WAV_STATUS_OPENERR:
				*err_msg = "\nerror occurred opening wav: " + wavname;
				break;
			case WAV_STATUS_WAVERR:
				*err_msg = "\nwav: " + wavname + "is not a wav file";
				break;
			case WAV_STATUS_MODE_ERR:
				*err_msg = "\nmode error with wav: " + wavname;
				break;
			case WAV_STATUS_NOT_FOUND:
				*err_msg = "\nwav: " + wavname + " was not found";
				break;
			case WAV_STATUS_INIT:
				*err_msg = "\nproblem initialising wav: " + wavname;
				break;
			case WAV_STATUS_OK:
				*err_msg = "\nwav: " + wavname + " is only OK, not opened.";
				break;
			case WAV_STATUS_OPEN:
				break;
		}
		delete wavfile;
		wavfile = 0;
	}
	return wavfile;
}

bool synthfilereader::skip_remarks()
{
    *synthfile >> *buff;
    while ((*buff)[0] == '/') {
        if ((*buff)[0] == '/') {
            char ch;
            while (synthfile->get(ch)) {
                if (ch == '\n')
                    break;
            }
            if (synthfile->eof())
                return false;   // error end of file
        }
        *synthfile >> *buff;
        if (synthfile->eof())
            return false;       // error end of file -- do i need this again?
    }
    return true;                // i'm happy - honest
}

#endif
