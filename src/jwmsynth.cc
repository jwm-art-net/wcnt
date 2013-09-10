#ifndef JWMSYNTH_H
#include "../include/jwmsynth.h"

#ifndef BARE_MODULES
jwmsynth::jwmsynth(int const argc, char **const argv) :
 version(0), path(0),iocatnames(0), modnames(0), innames(0), outnames(0),
 parnames(0), synthmodslist(0), inputlist(0), outputlist(0), paramlist(0),
 connectlist(0), synthfile(0), mdobjlist(0), dobj_names(0), dobj_list(0),
 dobjparam_list(0), dpar_names(0), exit_bar(0), in_bar_trig(0), in_bar(0),
 opts_count(argc), opts(argv)
{
    version = new char[6];
    strcpy(version, "1.127");
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
    dobj::register_dparnames(dpar_names = new dparamnames);
    dobj::register_dparlist(dobjparam_list = new dobjparamlist);
    dobj::register_dobjnames(dobj_names = new dobjnames);
    dobj::register_err_msg(dobj_err_msg = new string);
    dobj::register_dobjlist(dobj_list = new dobjlist);
    dobj::register_dobjdobjlist(dobj_dobjlist = new dobjdobjlist);
    synthmod::register_moddobjlist(mdobjlist = new moddobjlist);
    synthmod::register_err_msg(synthmod_err_msg = new string);
    // must register the various things above before creating
    // synthmodlist as it creates a nonezero module which
    // must have access to the lists - or it will cause seg fault.
    synthmod::register_modlist(synthmodslist = new synthmodlist);
    synthmod::register_connectlist(connectlist = new connectorlist);
    synthfile = new synthfilereader;
    stock_errs = new stockerrs;
    synthfile->set_stockerrs(stock_errs);
    valid = true;
}

jwmsynth::~jwmsynth()
{
    delete stock_errs;
    delete synthfile;
    delete connectlist;
    delete synthmodslist;
    delete mdobjlist;
    delete dobj_dobjlist;
    delete dobj_list;
    delete dobj_names;
    delete dobjparam_list;
    delete dpar_names;
    delete paramlist;
    delete outputlist;
    delete inputlist;
    delete parnames;
    delete outnames;
    delete innames;
    delete modnames;
    delete iocatnames;
    delete [] version;
    delete synthmod_err_msg;
    delete dobj_err_msg;
    if (path) delete [] path;
}

bool jwmsynth::generate_synth()
{
    if (!open_synthfile())
        return false;
    synthmod::register_path(path);
    if (!synthfile->read_header(&audio_samplerate, &sm_beats_per_minute,
                                &sm_beats_per_measure, &sm_beat_value)) {
        err_msg = synthfile->get_error_msg();
        return false;
    }
    cout << "\nProccessing ";
    cout << opts[wcfile_opt];
    string const *com = synthfile->read_command();
    while (*com != wcnt_id)
    {
        if (!com) {
            err_msg = synthfile->get_error_msg();
            return false;
        }
        if (dobj_names->get_type(com->c_str()) != dobjnames::DOBJ_FIRST){
            if (!read_and_create_dobj(com))
                return false;
        }
        else { // create synth module
            if (!read_and_create_synthmod(com))
                return false;
        }
        com = synthfile->read_command();
    }
    if (synthmodslist->get_first_of_type(synthmodnames::MOD_WCNT) == 0){
        err_msg = "\nfile ";
        err_msg += opts[wcfile_opt];
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
    *synthmod_err_msg = "";
    stockerrs::ERR_TYPE et = mod->validate();
    if (et != stockerrs::ERR_NO_ERROR) {
        err_msg = "\nModule ";
        err_msg += mod->get_username();
        if (strlen(mod->get_username()) >= 5)
            err_msg += " is a bit";
        else
            err_msg += " is slightly";
        if (mod->get_number_id() % 2 == 0)
            err_msg += " weird, ";
        else
            err_msg += " dubious, ";
        err_msg += *synthmod_err_msg;
        err_msg += stock_errs->get_prefix_err(et);
        err_msg += stock_errs->get_err(et);
        delete mod;
        return false;
    }
    if (!synthmodslist->add_module(mod)) {
        err_msg = "\ncould not add module ";
        err_msg += mod->get_username();
        err_msg += " to list.";
        delete mod;
        return false;
    }
    if (synthfile->is_verbose())
        cout << "\nend " << mod->get_username();
    return true;
}

bool jwmsynth::read_and_create_dobj(string const* com)
{
    dobj* dbj = synthfile->read_dobj(com);
    if (!dbj) {
        err_msg = synthfile->get_error_msg();
        return false;
    }
    if (!dbj->validate()) {
        err_msg = *dbj->get_error_msg();
        return false;
    }
    if (!dobj_list->add_dobj(dbj)) {
        err_msg = "\ncould not add data object to list.";
        return false;
    }
    if (synthfile->is_verbose())
        cout << "\nend " << dbj->get_username();
    return true;
}

bool jwmsynth::connect_synth()
{
    if (synthfile->is_verbose()) {
        cout << "\n\nEnd wcnt/jwmsynth: " << opts[wcfile_opt];
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
    synthmod* sm;
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
    cout << "\tone '";
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
    char const* wcnt = "\nwcnt ";
    char const* fname = " filename.wc ";
    char const* vrbl = "--verbose";
    char const* v = "-v";
    char const* hdr = "--header";
    char const* modhlp = "--module-help";
    char const* mh = "-mh";
    char const* inhlp = "--input-help";
    char const* inname = " inputname";
    char const* ih = "-ih";
    char const* dobjhlp = "--data-object-help";
    char const* dh = "-dh";
    char const* sampinf = "--sample-info";
    char const* si = "-si";
    char const* note = "--note-info";
    char const* ni = "-ni";
    char const* freq = "--freq-info";
    char const* fi = "-fi";
    char const* ors = " or ";
    if (opts_count == 1) {
        err_msg ="\n     e:james@jwm-art.net";
        err_msg+="\n       Made In England\n";
        err_msg+=wcnt;err_msg+=fname;
        err_msg+=wcnt;err_msg+=vrbl;err_msg+=ors;err_msg+=v;
            err_msg+=fname;
        err_msg+=wcnt;err_msg+=hdr;
        err_msg+=wcnt;err_msg+=modhlp;err_msg+=ors;err_msg+=mh;
        err_msg+=wcnt;err_msg+=modhlp;err_msg+=ors;err_msg+=mh;
            err_msg+=" modulename";
        err_msg+=wcnt;err_msg+=dobjhlp;err_msg+=ors;err_msg+=dh;
        err_msg+=wcnt;err_msg+=dobjhlp;err_msg+=ors;err_msg+=dh;
            err_msg+=" dataobjectname";
        err_msg+=wcnt;err_msg+=inhlp;err_msg+=ors;err_msg+=ih;
            err_msg+=inname;
        err_msg+=wcnt;err_msg+=fname;err_msg+=inhlp;err_msg+=ors;
            err_msg+=ih;err_msg+=inname;
        err_msg+=wcnt;err_msg+=sampinf;err_msg+=ors;err_msg+=si;
            err_msg+=" 16bitsample.wav";
        err_msg+=wcnt;err_msg+=note;err_msg+=ors;err_msg+=ni;
            err_msg+=" note name";
        err_msg+=wcnt;err_msg+=freq;err_msg+=ors;err_msg+=fi;
            err_msg+=" frequency  samplerate";
        err_msg += "\n";
        return false;
    }
    else if (opts_count == 2) {
        if (strcmp(modhlp, opts[1]) == 0 || strcmp(mh, opts[1]) == 0)
        {
            module_help();
            return false;
        } else if (strcmp(dobjhlp, opts[1]) == 0
                || strcmp(dh, opts[1]) == 0) 
        {
            dobj_help();
            return false;
        } else if (strcmp(hdr, opts[1]) == 0) {
            err_msg = "\nwcnt file header is ";
            err_msg += wcnt_id;
            return false;
        }
        wcfile_opt = 1;
        return true; // should be a filename then
    }
    else if (opts_count == 3) {
        if (strcmp(inhlp, opts[1]) == 0 || strcmp(ih, opts[1]) == 0) {
            input_help();
            return false;
        }
        if (strcmp(dobjhlp, opts[1]) == 0 || strcmp(dh, opts[1]) == 0) {
            dobj_help();
            return false;
        }
        if (strcmp(modhlp, opts[1]) == 0 || strcmp(mh, opts[1]) == 0) {
            module_help();
            return false;
        }
        if (strcmp(sampinf, opts[1]) == 0 || strcmp(si, opts[1]) == 0) {
            sample_info();
            return false;
        }
        if (strcmp(vrbl, opts[1]) == 0 || strcmp(v, opts[1]) == 0) {
            synthfile->set_verbose();
            wcfile_opt = 2;
            return true;
        }
        if (strcmp(vrbl, opts[2]) == 0 || strcmp(v, opts[2]) == 0) {
            synthfile->set_verbose();
            wcfile_opt = 1;
            return true;
        }
        if (strcmp(note, opts[1]) == 0 || strcmp(ni, opts[1]) == 0) {
            note_help();
            return false;
        }
    }
    else if (opts_count == 4) {
        if (strcmp(inhlp, opts[2]) == 0 || strcmp(ih, opts[2]) == 0) {
            wcfile_opt = 1;
            input_help();
            return false;
        }
        if (strcmp(freq, opts[1]) == 0 || strcmp(fi, opts[1]) == 0) {
            freq_help();
            return false;
        }
    }
    err_msg = "\nInvalid Options:";
    for (int i = 1; i < opts_count; i++) {
        err_msg += " ";
        err_msg += opts[i];
    }
    return false;
}

bool jwmsynth::open_synthfile()
{
    bool rtval = false;
    // get path part of filename, with trailing '/'
    char* filename = opts[wcfile_opt];
    char* ptr = filename;
    char* fnptr = ptr;
    while (*ptr != '\0') {
        if (*ptr == '/') fnptr = ptr + 1;
        ptr++;
    }
    if (fnptr != filename) {
        char c = *fnptr;
        *fnptr = '\0';
        path = new char[strlen(filename) + 1];
        strcpy(path, filename);
        *fnptr = c;
    }
    synthmod::register_path(path);
    switch (synthfile->open_file(filename, wcnt_id)) {
    case synthfilereader::NOT_FOUND:
        err_msg = "\nfile ";
        err_msg += filename;
        err_msg += " not found.\n";
        break;
    case synthfilereader::INVALID_HEADER:
        err_msg = "\nfile ";
        err_msg += filename;
        err_msg += " does not contain a valid header.\n";
        break;
    case synthfilereader::FILE_OPEN:
        rtval = true;
        break;
    case synthfilereader::FILE_READY:
        err_msg = "\nfile ";
        err_msg += filename;
        err_msg += " inspires premature optimism.\n";
        break;
    }
    return rtval;
}

void jwmsynth::module_help()
{
    char spaces[20];
    for (int i = 0; i < 20; i++) spaces[i] = ' ';
    string smname = (opts_count == 3) ? opts[2] : "";
    synthmodnames::SYNTH_MOD_TYPE smt 
     = modnames->get_type(smname.c_str());
    if (smt == synthmodnames::MOD_FIRST) {
        if (opts_count == 3)
            err_msg = "\nUnknown synth module: " + smname + "\n";
        else
            err_msg = "";
        err_msg += "\navailable module types are:\n\n";
        int i, ip = 0, l, mxl = 0;
        string mn;
        for (i = synthmodnames::MOD_FIRST + 1;
         i < synthmodnames::MOD_LAST; i++)
        { // find longest module name.
            mn = modnames->get_name(
             (synthmodnames::SYNTH_MOD_TYPE)i);
            l = mn.length();
            if (l > mxl) mxl = l;
        }
        mxl += 2;
        for (i = synthmodnames::MOD_FIRST + 1;
         i < synthmodnames::MOD_LAST; i++)
        {
            if (i != synthmodnames::MOD_NONEZERO) {
                ip++;
                mn = modnames->get_name((synthmodnames::SYNTH_MOD_TYPE)i);
                err_msg += mn;
                if (ip % 4 == 0) err_msg += "\n";
                else err_msg.append(spaces, mxl - mn.length());
            }
        }
        err_msg += "\n";
        return;
    }
    synthmod* sm = synthmodslist->create_module(smt, "username");
    err_msg = "\n";
    err_msg += modnames->get_name(smt);
    if (sm == 0) {
        err_msg += " module has not been fully\nincorporated into the ";
        err_msg += "wcnt user interface.  Oops! \n Send an email to ";
        err_msg += "james@jwm-art.net to complain.\n";
        return;
    }
    err_msg += "\n";
    err_msg += sm->get_username();
    dobj_help(smt);
    modinputlist* inlist = inputlist->get_inputlist_for_module(sm);
    modinput* input = inlist->goto_first();
    if (input) {
        err_msg += "\n// inputs for ";
        err_msg += modnames->get_name(smt);
        int mxl = 0;
        int l = 0;
        string in;
        while(input) { // get max len of input name
            in = innames->get_name(input->get_inputtype());
            l = in.length();
            if (l > mxl) mxl = l;
            input = inlist->goto_next();
        }
        mxl += 2;
        input = inlist->goto_first();
        while(input) {
            string in = innames->get_name(input->get_inputtype());
            err_msg += "\n    " + in;
            err_msg.append(spaces, mxl - in.length());
            err_msg += "modulename outputname";
            input = inlist->goto_next();
        }
    }
    delete inlist;
    modparamlist* parlist =
        synthmod::get_paramlist()->get_paramlist_for_moduletype(smt);
    modparam* param = parlist->goto_first();
    if (param) {
        paramnames* paramnames = synthmod::get_paramnames();
        err_msg += "\n// parameters for ";
        err_msg += modnames->get_name(smt);
        int mxl = 0;
        int l = 0;
        string pn;
        while(param) {
            pn = paramnames->get_name(param->get_paramtype());
            l = pn.length();
            if (l > mxl) mxl = l;
            param = parlist->goto_next();
        }
        mxl += 2;
        param = parlist->goto_first();
        while(param) {
            paramnames::PAR_TYPE pt = param->get_paramtype();
            pn = paramnames->get_name(pt);
            err_msg += "\n    " + pn;
            err_msg.append(spaces, mxl - pn.length());
            err_msg += iocatnames->get_name(paramnames->get_category(pt));
            param = parlist->goto_next();
        }
    }
    delete parlist;
    modoutputlist* outlist =
        synthmod::get_outputlist()->get_outputlist_for_module(sm);
    modoutput* output = outlist->goto_first();
    if (output) {
        outputnames* outnames = synthmod::get_outputnames();
        err_msg += "\n// outputs for ";
        err_msg += modnames->get_name(smt);
        while(output) {
            err_msg += "\n// ";
            err_msg += outnames->get_name(output->get_outputtype());
            output = outlist->goto_next();
        }
    }
    delete outlist;
    err_msg += "\n";
    err_msg += sm->get_username();
    err_msg += "\n";
    delete sm;
    return;
}

void jwmsynth::dobj_help(synthmodnames::SYNTH_MOD_TYPE smt)
{
    moddobjlist* mdlist = mdobjlist->get_moddobjlist_for_moduletype(smt);
    moddobj* mdbj = mdlist->goto_first();
    if (!mdbj) {
        delete mdlist;
        return;
    }
    err_msg += "\n// data objects for ";
    err_msg += modnames->get_name(smt);
    while(mdbj) {
        dobjnames::DOBJ_TYPE dt = mdbj->get_dobj_type();
        err_msg += "\n    ";
        err_msg += dobj_names->get_name(dt);
        dobjdobjlist* ddlist =
         dobj_dobjlist->get_dobjdobjlist_for_dobjtype(dt);
        dobjdobj* dd = ddlist->goto_first();
        while(dd) {
            dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
            err_msg += "\n        ";
            err_msg += dobj_names->get_name(sprogtype);
            delete dobj_list->create_dobj(sprogtype);
            dobj_help_params(sprogtype);
            dd = ddlist->goto_next();
        }
        err_msg += "\n    ";
        err_msg += dobj_names->get_name(dt);
        delete ddlist;
        mdbj = mdlist->goto_next();
    }
    err_msg += "\n";
    delete mdlist;
    return;
}

void jwmsynth::dobj_help()
{
    char spaces[20];
    for (int i = 0; i < 20; i++) spaces[i] = ' ';
    string dname = (opts_count == 3) ? opts[2] : "";
    dobjnames::DOBJ_TYPE dt = dobj_names->get_type(dname.c_str());
    if (dt == dobjnames::DOBJ_FIRST) {
        // incorrect dobj name or no name specified
        if (opts_count == 3)
            err_msg = "\nno data object available named " + dname;
        err_msg += "\navailable data object types are\n\n";
        int i, ip = 0; // display data objects not defined within modules
        string dn;
        int mxl = 0, l;
        for (i = dobjnames::DOBJ_DEFLISTS + 1;
         i < dobjnames::DOBJ_SYNTHMOD;i++) 
        {
            if (dobj_names->check_type((dobjnames::DOBJ_TYPE)i) == i) {
                dn = dobj_names->get_name((dobjnames::DOBJ_TYPE)i);
                l = dn.length();
                if (l > mxl) mxl = l;
            }
        }
        mxl += 2;
        for (i = dobjnames::DOBJ_DEFLISTS + 1;
         i < dobjnames::DOBJ_SYNTHMOD;i++)
        {
            if (dobj_names->check_type((dobjnames::DOBJ_TYPE)i) == i) {
                dn = dobj_names->get_name((dobjnames::DOBJ_TYPE)i);
                err_msg += dn;
                ip++;
                if (ip % 2 == 0) 
                    err_msg += "\n";
                else 
                    err_msg.append(spaces, mxl - dn.length());
            }
        }
        err_msg += "\n";
        return;
    }
    delete dobj_list->create_dobj(dt);
    dobjdobjlist* ddlist =
     dobj_dobjlist->get_dobjdobjlist_for_dobjtype(dt);
    dobjdobj* dd = ddlist->goto_first();
    err_msg += "\n";
    err_msg += dobj_names->get_name(dt);
    err_msg += "\nusername";
    dobj_help_params(dt);
    while(dd) {
        dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
        err_msg += "\n    ";
        err_msg += dobj_names->get_name(sprogtype);
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
    char spaces[20];
    for (int i = 0; i < 20; i++) spaces[i] = ' ';
    dobjparamlist * dparlist = 
     dobjparam_list->get_dobjparamlist_for_dobj_type(dt);
    dobjparam* dparam = dparlist->goto_first();
    string dn;
    int mxl = 0, l;
    while(dparam) {
        dn = dpar_names->get_name(dparam->get_dpartype());
        l = dn.length();
        if (l > mxl) mxl = l;
        dparam = dparlist->goto_next();
    }
    dparam = dparlist->goto_first();
    while(dparam) {
        IOCAT iocat = dpar_names->get_category(dparam->get_dpartype());
        err_msg += "\n            ";
        dn = dpar_names->get_name(dparam->get_dpartype());
        err_msg += dn;
        err_msg.append(spaces, mxl - dn.length() + 2);
        err_msg += iocatnames->get_name(iocat);
        dparam = dparlist->goto_next();
    }
    delete dparlist;
    return;
}

void jwmsynth::input_help()
{ 
// decide by the number of options passed on command line whether to 
// load a wcnt file or to create all possible modules in order to 
// access outputs.
    char spaces[20];
    for (int i = 0; i < 20; i++) spaces[i] = ' ';
    string inpname = (opts_count == 3) ? opts[2] : opts[3];
    inputnames::IN_TYPE intype = innames->get_type(inpname.c_str());
    if (intype == inputnames::IN_FIRST) {
        err_msg =
            "\nUnknown input type :" + inpname
            + "\navailable input types are:\n";
        int i, ip = 0;
        string in;
        int l, mxl = 0;
        for (i = inputnames::IN_FIRST + 1; i < inputnames::IN_LAST; i++) {
            in = innames->get_name((inputnames::IN_TYPE)i);
            l = in.length();
            if (l > mxl) mxl = l;
        }
        mxl += 2;
        for (i = inputnames::IN_FIRST + 1; i < inputnames::IN_LAST; i++) {
            in = innames->get_name((inputnames::IN_TYPE)i);
            err_msg += in;
            ip++;
            if (ip % 4 == 0)
                err_msg += "\n";
            else
                err_msg.append(spaces, mxl - in.length());
        }
        return;
    }
    err_msg = "";
    IOCAT incat = innames->get_category(intype);
    if (opts_count == 3) { // must create the modules
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
                synthmodslist->add_module(sm);
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
    string mn;
    int l, mxl = 0;
    while(output) {
        mn = output->get_synthmodule()->get_username();
        l = mn.length();
        if (l > mxl) mxl = l;
        output = outputs->goto_next();
    }
    mxl += 2;
    output = outputs->goto_first();
    while(output)
    { // this if statement prevents from displaying off off
        // now it only displays one off - less confusing.
        if (output->get_moduletype() != synthmodnames::MOD_NONEZERO) {
            mn = output->get_synthmodule()->get_username();
            err_msg += mn;
            err_msg.append(spaces, mxl - mn.length());
        }
        err_msg += outnames->get_name(output->get_outputtype());
        err_msg += "\n";
        output = outputs->goto_next();
    }
    delete outputs; // delete list we created
    return;
}

void jwmsynth::sample_info()
{
    wavfilein wavfile;
    wavfile.set_wav_filename(opts[2]);
    // set root note so that validate() won't complain about it
    wavfile.set_root_note("c0");
    // validate actually opens the file, so that it can validate the
    // format also.
    wavfile.validate();
    if (wavfile.get_status() != WAV_STATUS_OPEN) {
        err_msg = "\n";
        err_msg += *wavfile.get_error_msg();
        return;
    }
    err_msg = "\nheader info for wav: ";
    err_msg += opts[2];
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
        err_msg += 
         "\nbitrate not supported.  While wcnt will let you use";
        err_msg += " this sample, the sampler output will be garbage!";
        break;
    }
    if (wavfile.get_bitrate()!= WAV_BIT_16)	return;
    ostringstream conv;
    conv << "\t" << wavfile.get_sample_rate() << " hz";
    conv << "\n\tlength: " << wavfile.get_length() << " samples (";
    conv << (double)wavfile.get_length() / audio_samplerate;
    conv << " seconds)\n";
    err_msg += conv.str();
    return;
}

void jwmsynth::note_help()
{
    if (!check_notename(opts[2])) {
        err_msg = "\nInvalid note name";
        return;
    }
    ostringstream conv;
    conv << "\n\tfrequency " << note_to_freq(opts[2]);
    err_msg = conv.str();
    return;
}

void jwmsynth::freq_help()
{
    float frequency = atof(opts[2]);
    audio_samplerate = atoi(opts[3]);
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
