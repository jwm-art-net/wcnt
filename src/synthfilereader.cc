#ifndef SYNTHFILEREADER_H
#include "../include/synthfilereader.h"

#ifndef BARE_MODULES

synthfilereader::synthfilereader() :
 filename(0), filestatus(NOT_FOUND), synthfile(0), buff(0), 
 synthheader(0), err_msg(0), conv(0), genstatus(HALTED), verbose(false)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    err_msg = new string;
    // don't create conv here.
}

synthfilereader::~synthfilereader()
{
    synthfile->close();
    delete [] filename;
    delete synthfile;
    delete buff;
    delete synthheader;
    delete err_msg;
    if (conv) delete conv;
}

synthfilereader::FILE_STATUS
synthfilereader::open_file(char *synthfilename, string wcnt_id)
{
    filename = new char[strlen(synthfilename) + 1];
    strcpy(filename, synthfilename);
    synthfile->open(filename);
    if (!synthfile->is_open())
        filestatus = NOT_FOUND;
    else {
        *synthfile >> *synthheader;
        if (*synthheader == wcnt_id)
            filestatus = FILE_OPEN;
        else
            filestatus = INVALID_HEADER;
    }
    return filestatus;
}

bool synthfilereader::read_header(
    unsigned long *samplerate, short *bpm,
    short *beatspermeasure, short *beatvalue)
{
    if (conv) delete conv;
    conv = new ostringstream;
    if (filestatus != FILE_OPEN) {
        genstatus = HALTED;
        *err_msg = "\nArrrp! Programmer Error! Shpoooochuuha...\n";
        return 0;
    }
    if (!skip_remarks()) {
        *err_msg = "Unexpected end of file.";
        genstatus = MASSIVE_ERROR;
        return false;
    }

    if (*buff == "header") {
        if (!eff_ing_header_bodge(samplerate, bpm, beatspermeasure,
                                  beatvalue)) return false;
    }
    else {
        if (*buff == "samplerate") {
            if (!(*synthfile >> *samplerate)) {
                *err_msg = "Expected value for samplerate.";
                genstatus = HALTED;
                return false;
            }
            if (*samplerate > 4000 && *samplerate < 200000) {
                if (verbose)
                    cout << "\nsamplerate set at " << *samplerate;
            }
            else {
                *conv << *samplerate;
                *err_msg =
                    "Invalid samplerate: " + conv->str() +
                    ". valid values between 4000 and 200000.";
                genstatus = HALTED;
                return false;
            }
        }
        else {
            *err_msg = "Expected 'header' or 'samplerate' got "
                       + *buff + " instead.";
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
                if (verbose) cout << "\nbpm set at " << *bpm;
            }
            else {
                *conv << *bpm;
                *err_msg =
                    "Invalid bpm: " + conv->str() +
                    ". valid values between 20 and 1000.";
                genstatus = HALTED;
                return false;
            }
        }
        else {
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
                if (ch == '/') break;
            }
            if (synthfile->eof()) {
                *err_msg =
                 "Unexpected end of file while scanning time signature.";
                genstatus = HALTED;
                return false;
            }
            if (!(*synthfile >> *beatvalue)) {
                *err_msg =
                    "Expected value for time signature - beat value.";
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
            if (verbose) {
                cout << "\ntime signature set to " << *beatspermeasure;
                cout << "/" << *beatvalue;
            }
        }
        else {
            *err_msg = "Expected 'signature'.";
            genstatus = HALTED;
            return false;
        }
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

synthmod *const synthfilereader::read_synthmodule(string const *com)
{
    synthmodnames::SYNTH_MOD_TYPE
    smt = synthmod::get_modnames()->get_type(com->c_str());
    if (smt == synthmodnames::MOD_FIRST 
        || smt == synthmodnames::MOD_NONEZERO)
    {
        *err_msg = "\nUnrecognised wcnt/jwmsynth module: " + *com;
        return 0;
    }
    if (smt == synthmodnames::MOD_WCNT) {
        if (synthmod::get_modlist()->
            get_first_of_type(synthmodnames::MOD_WCNT))
        {
            *err_msg = "\nCannot create more than one wcnt_exit module "
                       + *com;
            return 0;
        }
    }
    string modname;
    *synthfile >> modname;
    if (synthmod::get_modlist()->get_synthmod_by_name(modname.c_str())){
        *err_msg = "\nsynth module already exists named " + modname;
        return 0;
    }
    if (verbose) {
        cout << "\n\n================================";
        cout << "\nCreating synth module " << modname;
    }
    synthmod*
     sm = synthmod::get_modlist()->create_module(smt, modname.c_str());
    if (!read_dobjs(sm)){
        *err_msg = sm->get_username() + *err_msg;
        *err_msg = "\nIn module " + *err_msg;
        genstatus = HALTED;
        delete sm;
        return 0;
    }
    if (!read_inputs(sm)){
        *err_msg = sm->get_username() + *err_msg;
        *err_msg = "\nIn module " + *err_msg;
        genstatus = HALTED;
        delete sm;
        return 0;
    }
    if (!read_params(sm)){
        *err_msg = sm->get_username() + *err_msg;
        *err_msg = "\nIn module " + *err_msg;
        genstatus = HALTED;
        delete sm;
        return 0;
    }
    com = read_command();
    if (*com != modname) {
        *err_msg = "\nIn module ";
        *err_msg += sm->get_username();
        *err_msg += ", expected definition termination ";
        *err_msg += sm->get_username();
        *err_msg += ", got " + *com + " instead.";
        genstatus = HALTED;
        delete sm;
        return 0;
    }
    return sm;
}

//------------------------------------------------------------------------
// dobj* const synthfilereader::read_dobj(string const* command)
// only reads those data objects which are defined outside of a
// synth module
//------------------------------------------------------------------------
// string const* command points to the dobj name as entered by user
// in file.  no need to check command is a valid dobj type name as
// excution would not have reached this far otherwise
// (see jwmsynth::generate_synth())
// note: these dobjies may have parameters, or dobjies, or both.
//
//---
// return dobj on success
// return 0 on failure
//--
dobj* const synthfilereader::read_dobj(string const* com)
{
    dobjnames* dbjnames = dobj::get_dobjnames();
    dobjnames::DOBJ_TYPE dobjtype = dbjnames->get_type(com->c_str());
    dobjnames::DOBJ_TYPE subtype = dbjnames->get_sub_type(dobjtype);
    if (subtype < dobjnames::DOBJ_DEFLISTS ||
            subtype > dobjnames::DOBJ_DEFSINGLES)
    {
        *err_msg = "\n" + *com + " is not a standalone data object.";
        return 0;
    }
    string dobjname;
    *synthfile >> dobjname;
    if (dobj::get_dobjlist()->get_dobj_by_name(dobjname.c_str())) {
        *err_msg= "\ndata object " + *com
                  + " already exists named "+dobjname;
        return 0;
    }
    if (verbose) {
        cout << "\n\n================================";
        cout << "\nCreating data object " << dobjname;
    }
    dobj* dbj = dobj::get_dobjlist()->create_dobj(dobjtype);
    if (dbj == 0) {
        *err_msg = "\ncould note create data object of type " + *com;
        return 0;
    }
    dbj->set_username(dobjname.c_str());
    // read dobj parameters (if any)
    if (!read_dobj_params(dbj)){
        genstatus = HALTED;
        delete dbj;
        return 0;
    }
    // read dobj's dobjies (if any)
    if (!read_dobjs(dbj)){
        genstatus = HALTED;
        delete dbj;
        return 0;
    }
    stockerrs::ERR_TYPE et = dbj->validate();
    if (et != stockerrs::ERR_NO_ERROR) {
        *err_msg = "\nIn data object ";
        *err_msg += dbj->get_username();
        *err_msg += ", parameter ";
        *err_msg += *dbj->get_error_msg();
        *err_msg += " ";
        *err_msg += stock_errs->get_prefix_err(et);
        *err_msg += stock_errs->get_err(et);
        delete dbj;
        return 0;
    }
    // read_dobjs(dobj*) stops when it encounters the username of the dobj
    return dbj;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_dobjs(dobj* sm)
// only reads those data objects which are defined within a dobj
//------------------------------------------------------------------------
// read the dobjs (data objects) for the dobj.
//---
// return true  if succesful.
// return true  if dobj contains no dobjies
// return false if it landed on it's backside.
//--
bool synthfilereader::read_dobjs(dobj* dbj)
{
    dobjnames::DOBJ_TYPE dt = dbj->get_object_type();
    dobjnames* dbjnames = dobj::get_dobjnames();
    dobjdobjlist* ddlist = dobj::get_dobjdobjlist()->
                           get_dobjdobjlist_for_dobjtype(dt);
    dobjdobj* dd = ddlist->goto_first();
    // layout is structured differently to read_dobjs(synthmod*)
    // ie the riff contains the notes, (after it's params)
    // whereas an adsr contains the envelope contains the coords.
    // therefore no encapsulating dobj name is read here.
    string com = *read_command();
    while(dd) {
        dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
        char const* sprogname = dbjnames->get_name(sprogtype);
        if (com != sprogname) {
            *err_msg = "\nIn data object ";
            *err_msg += dbj->get_username();
            *err_msg += ", expected ";
            *err_msg += sprogname;
            *err_msg += ", got ";
            *err_msg += com;
            *err_msg += " instead";
            delete ddlist;
            return false;
        }
        while (com == sprogname) {
            dobj* sprog = dobj::get_dobjlist()->create_dobj(sprogtype);
            if (sprog == 0) {
                *err_msg = "\n***major error***";
                *err_msg += "\ncould not create data object ";
                *err_msg += sprogname;
                *err_msg += " for data object ";
                *err_msg += dbj->get_username();
                delete ddlist;
                return false;
            }
            if (verbose)
                cout << "\n--------\ncreating " << sprogname;
            if (!read_dobj_params(sprog)) {
                *err_msg = dbj->get_username() + *err_msg;
                *err_msg = "\nIn data object " + *err_msg;
                delete sprog;
                delete ddlist;
                return false;
            }
            stockerrs::ERR_TYPE et = sprog->validate();
            if (et != stockerrs::ERR_NO_ERROR) {
                *err_msg = "\nIn data object ";
                *err_msg += dbj->get_username();
                *err_msg += ", error in ";
                *err_msg += sprogname;
                *err_msg += ", ";
                *err_msg += *sprog->get_error_msg();
                *err_msg += " ";
                *err_msg += stock_errs->get_prefix_err(et);
                *err_msg += stock_errs->get_err(et);
                delete sprog;
                delete ddlist;
                return false;
            }
            // add sprog to dbj, not dobjlist  . . .
            if (!dbj->add_dobj(sprog)) {
                *err_msg =
                    "\n***major error***\ncould not add data object ";
                *err_msg += sprogname;
                *err_msg += " to data object ";
                *err_msg += dbj->get_username();
                delete sprog;
                delete ddlist;
                return false;
            }
            if (verbose)
                cout << "\nadded " << sprogname;
            com = *read_command();
        }
        dd = ddlist->goto_next();
    }
    if (com != dbj->get_username()) {
        *err_msg = "\nIn data object ";
        *err_msg += dbj->get_username();
        *err_msg += ", expected definition termination ";
        *err_msg += dbj->get_username();
        *err_msg += ", got ";
        *err_msg += com;
        *err_msg += " instead";
        genstatus = HALTED;
        delete ddlist;
        return false;
    }
    delete ddlist;
    return true;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_dobjs(synthmod* sm)
// only reads those data objects which are defined within a synth module
//------------------------------------------------------------------------
// read the dobjs (data objects) for the module.
// note: a dobj type which is encapsulated by a synth module, apart from
// not actually being a dobj, has no parameters.  the synthmod has
// parameters which may or may not effect it. it will though have
// dobj types inside, with parameters.
//---
// return true  if succesful.
// return true  as modules lacking dobjs present no problems.
// return false if it fell headfirst into the void.
//--
bool synthfilereader::read_dobjs(synthmod* sm)
{
    synthmodnames::SYNTH_MOD_TYPE smt = sm->get_module_type();
    moddobjlist* mdbjslist =
        synthmod::get_moddobjlist()->get_moddobjlist_for_moduletype(smt);
    moddobj* mdbj = mdbjslist->goto_first();
    dobjnames* dbjnames = dobj::get_dobjnames();
    while(mdbj) { // module may contain more than one list (ie timemap)
        string dbjname = *read_command();//one of envelope/track etc
        dobjnames::DOBJ_TYPE dt = mdbj->get_dobj_type();
        char const* xdbjname = dbjnames->get_name(dt);
        if (dbjname != xdbjname) {
            *err_msg = ", expected ";
            *err_msg += xdbjname;
            *err_msg += " got ";
            *err_msg += dbjname;
            *err_msg += " instead";
            delete mdbjslist;
            return false;
        }
        if (verbose)
            cout << "\n--------\nfor " + dbjname;
        dobjdobjlist* ddlist = dobj::get_dobjdobjlist()->
                               get_dobjdobjlist_for_dobjtype(dt);
        dobjdobj* dd = ddlist->goto_first();
        while(dd) {//maybe the dobj has one or more dobjies inside?
            dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
            char const* xsprogname = dbjnames->get_name(sprogtype);
            string com = *read_command();
            // now read the list of items (each item's type is sprogtype)
            while (com != dbjname) {
                if (com != xsprogname) {
                    // check name of item matches expected
                    *err_msg = ", data object ";
                    *err_msg += xdbjname;
                    *err_msg += ", expected ";
                    *err_msg += xsprogname;
                    *err_msg += ", got ";
                    *err_msg += com;
                    *err_msg += " instead.";
                    delete ddlist;
                    delete mdbjslist;
                    return false;
                }
                if (verbose)
                    cout << "\n--------\ncreating " << xsprogname;
                dobj* dbj = dobj::get_dobjlist()->create_dobj(sprogtype);
                if (dbj == 0) { // failed to create dobj
                    *err_msg =
                     "\n***major error***\ncould not create data object ";
                    *err_msg += xsprogname;
                    *err_msg += ", data object ";
                    *err_msg += xdbjname;
                    delete mdbjslist;
                    delete ddlist;
                    return false;
                }
                if (!read_dobj_params(dbj)) {
                    *err_msg = xdbjname + *err_msg;
                    *err_msg = ", data object " + *err_msg;
                    *err_msg += *dbj->get_error_msg();
                    delete dbj;
                    delete mdbjslist;
                    delete ddlist;
                    return false;
                }
                stockerrs::ERR_TYPE et = dbj->validate();
                if (et!= stockerrs::ERR_NO_ERROR) {
                    *err_msg = ", data object ";
                    *err_msg += xdbjname;
                    *err_msg += ", ";
                    *err_msg += *dbj->get_error_msg();
                    *err_msg += " ";
                    *err_msg += stock_errs->get_prefix_err(et);
                    *err_msg += stock_errs->get_err(et);
                    delete dbj;
                    delete mdbjslist;
                    delete ddlist;
                    return false;
                }
                // add to synthmodule, not dobjlist  . . .
                if (!sm->add_dobj(dbj)) {
                    *err_msg =
                        "\n***major error***\ncould not add data object ";
                    *err_msg += xsprogname;
                    *err_msg += *sm->get_error_msg();
                    *err_msg += ", in data object ";
                    *err_msg += xdbjname;
                    delete dbj;
                    delete mdbjslist;
                    delete ddlist;
                    return false;
                }
                if (verbose)
                    cout << "\nadded data object " << xsprogname;
                com = *read_command();
            }
            dd = ddlist->goto_next();
        }
        delete ddlist;
        mdbj = mdbjslist->goto_next();
    }
    delete mdbjslist;
    return true;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_dobj_params(dobj* dbj)
// reads dparams for all data objects
//------------------------------------------------------------------------
// return true  if succesful.
// return false on fail.
bool synthfilereader::read_dobj_params(dobj* dbj)
{
    dobjnames::DOBJ_TYPE dobjtype = dbj->get_object_type();
    dobjparamlist* dparlist = dobj::get_dparlist()->
                              get_dobjparamlist_for_dobj_type(dobjtype);
    dobjparam* dparam = dparlist->goto_first();
    dparamnames* dpnames = dobj::get_dparnames();
    while(dparam) {
        string dparname = *read_command();
        if (dpnames->get_name(dparam->get_dpartype()) != dparname) {
            *err_msg = ", data object ";
            *err_msg += dobj::get_dobjnames()->get_name(dobjtype);
            *err_msg += ", expected parameter name ";
            *err_msg += dpnames->get_name(dparam->get_dpartype());
            *err_msg += ", got ";
            *err_msg += dparname;
            *err_msg += " instead.";
            delete dparlist;
            return false;
        }
        // not quite sure how to properly use ostringstream :/
        if (conv) delete conv;
        conv = new ostringstream;
        IOCAT iocat = dpnames->get_category(dparam->get_dpartype());
        void* data = read_iocat_value(iocat);
        if (!data) {
            *err_msg = dobj::get_dobjnames()->get_name(dobjtype)
             + *err_msg;
            *err_msg = ", data object " + *err_msg;
            *err_msg += " for parameter ";
            *err_msg += dpnames->get_name(dparam->get_dpartype());
            delete dparlist;
            return false;
        }
        if (!dbj->set_dparam(dparam->get_dpartype(), data)) {
            destroy_iocat_data(iocat, data);
            *err_msg = "\ninvalid parameter ";
            *err_msg += dpnames->get_name(dparam->get_dpartype());
            *err_msg += " for data object ";
            *err_msg += dobj::get_dobjnames()->get_name(dobjtype);
            *err_msg += "\n";
            *err_msg += *dbj->get_error_msg();
            delete dparlist;
            return false;
        }
        destroy_iocat_data(iocat, data);
        if (verbose) {
            cout << "\nparameter ";
            cout << dpnames->get_name(dparam->get_dpartype());
            cout << "\t--> " << conv->str();
        }
        dparam = dparlist->goto_next();
    }
    delete dparlist;
    return true;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_inputs(synthmod* sm)
//------------------------------------------------------------------------
// read the inputs for the module.
// return true  if no problems occurred reading inputs
// return true  if the module does not have inputs, it's not a problem.
// return false if problems arose.
bool synthfilereader::read_inputs(synthmod* sm)
{
    modinputlist* inlist =
        synthmod::get_inputlist()->get_inputlist_for_module(sm);
    modinput* inp = inlist->goto_first();
    inputnames* innames = synthmod::get_inputnames();
    outputnames* outnames = synthmod::get_outputnames();
    string inputname;
    if (verbose && inp)
        cout << "\n--------";
    while(inp) { // step through each  input for module
        inputname = *read_command();
        if (innames->get_name(inp->get_inputtype()) != inputname){
            *err_msg = ", expected input type ";
            *err_msg += innames->get_name(inp->get_inputtype());
            *err_msg += ", got ";
            *err_msg += inputname;
            *err_msg += " instead";
            delete inlist;
            return false;
        } else {
            inputnames::IN_TYPE input_type
             =innames->get_type(inputname.c_str());
            string outmodname;
            *synthfile >> outmodname;
            if (outmodname == "off") {
                outputnames::OUT_TYPE offout =
                 outnames->get_nonezerotype(
                 innames->get_category(input_type));
                connector* connect = new
                 connector(sm, input_type, outmodname.c_str(), offout);
                synthmod::get_connectlist()->add_connector(connect);
                if (verbose)
                    cout << "\nadded connector " << inputname << "\toff";
            }
            else
            {
                string outputname;
                *synthfile >> outputname;
                outputnames::OUT_TYPE output_type =
                    outnames->get_type(outputname.c_str());
                if (output_type == outputnames::OUT_FIRST) {
                    *err_msg = ", input ";
                    *err_msg += inputname;
                    *err_msg += " set with unknown output type ";
                    *err_msg += outputname;
                    delete inlist;
                    return false;
                }
                if (innames->get_category(input_type)
                        != outnames->get_category(output_type))
                {
                    *err_msg = ", input ";
                    *err_msg += inputname;
                    *err_msg += " set with wrong category of output, ";
                    *err_msg += outputname;
                    delete inlist;
                    return false;
                }
                connector* connect = new
           connector(sm, input_type, outmodname.c_str(), output_type);
                synthmod::get_connectlist()->add_connector(connect);
                if (verbose){
                    cout << "\nadded connector " << inputname << "\t";
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
    modparamlist* parlist
     = synthmod::get_paramlist()->get_paramlist_for_moduletype(
      sm->get_module_type());
    if (!parlist) {
        *err_msg = "\n*** Big problems getting params ***";
        return false;
    }
    modparam* param = parlist->goto_first();
    paramnames* paramnames = synthmod::get_paramnames();
    string paramname;
    if (verbose && param)
        cout << "\n--------";
    while(param) {
        paramname = *read_command();
        if (paramnames->get_name(param->get_paramtype()) != paramname) {
            *err_msg = ", expected parameter ";
            *err_msg += paramnames->get_name(param->get_paramtype());
            *err_msg += " got ";
            *err_msg += paramname;
            *err_msg += " instead";
            delete parlist;
            return false;
        } else {
            paramnames::PAR_TYPE paramtype
            = paramnames->get_type(paramname.c_str());
            // delete conv (ostringstream) and re-create to erase string
            // inside i don't know how else to do this yet.
            // doing it here, because read_param_value puts the
            // parameter value in conv so it can be displayed.
            if (conv) delete conv;
            conv = new ostringstream;
            IOCAT iocat =
                synthmod::get_paramnames()->get_category(paramtype);
            void* data = read_iocat_value(iocat);
            if (!data) {
                *err_msg = paramnames->get_name(param->get_paramtype())
                 + *err_msg;
                *err_msg = ", for parameter " + *err_msg;
                genstatus = HALTED;
                delete parlist;
                return false;
            }
            if (!sm->set_param(paramtype, data)){
                // inconsistency in the synthmodule, if debugged
                // this is not needed.
                destroy_iocat_data(iocat, data);
                *err_msg += "denial of owning parmeter type ";
                *err_msg += paramname;
                genstatus = HALTED;
                delete parlist;
                return false;
            }
            destroy_iocat_data(iocat, data);
            if (verbose) {
                cout << "\nparameter ";
                cout << paramname << "\t" << conv->str();
            }
        }
        param = parlist->goto_next();
    }
    delete parlist;
    return true;
}

void* synthfilereader::read_iocat_value(IOCAT iocat)
{
    string svalue;
    char* cstr = 0;
    void* data = 0;
    switch(iocat)
    {
    case CAT_DOUBLE:
        data = new double;
        if (!(*synthfile >> *(double*)data)) {
            *err_msg = ", expected floating point value";
            delete (double*)data;
            return 0;
        }
        *conv << *(double*)data;
        break;
    case CAT_SHORT:
        data = new short;
        if (!(*synthfile >> *(short*)data)) {
            *err_msg = ", expected integer value (hint:32767 max)";
            delete (short*)data;
            return 0;
        }
        *conv << *(short*)data;
        break;
    case CAT_ULONG:
        data = new unsigned long;
        if (!(*synthfile >> *(unsigned long*)data)) {
            *err_msg = ", expected unsigned long integer value";
            delete (unsigned long*)data;
            return 0;
        }
        *conv << *(unsigned long*)data;
        break;
    case CAT_TRIG: // CAT_TRIG same as CAT_STATE
    case CAT_STATE:// except in behaviour
        data = new STATUS;
        *synthfile >> svalue;
        if (svalue == "on")
            *(STATUS*)data = ON;
        else if (svalue == "off")
            *(STATUS*)data = OFF;
        else {
            *err_msg = ", expected string value on or off, got ";
            *err_msg += svalue;
            *err_msg += " instead";
            delete (STATUS*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_STRING:
        if (!(*synthfile >> svalue)) {
            *err_msg = ", unable to read string value";
            return 0;
        } else {
            cstr = new char[svalue.length() + 1];
            strncpy(cstr, svalue.c_str(), svalue.length());
            cstr[svalue.length()] = 0;
            data = cstr;
        }
        *conv << svalue;
        break;
    case CAT_MOD_FUNC:	// enum modifier::MOD_FUNC
        data = new modifier::MOD_FUNC;
        *synthfile >> svalue;
        if (svalue == "add")
            *(modifier::MOD_FUNC*)data=modifier::ADD;
        else if (svalue == "sub")
            *(modifier::MOD_FUNC*)data=modifier::SUB;
        else if (svalue == "mul")
            *(modifier::MOD_FUNC*)data=modifier::MUL;
        else if (svalue == "div")
            *(modifier::MOD_FUNC*)data=modifier::DIV;
        else if (svalue == "mod")
            *(modifier::MOD_FUNC*)data=modifier::MOD;
        else if (svalue == "sin")
            *(modifier::MOD_FUNC*)data=modifier::SIN;
        else if (svalue == "cos")
            *(modifier::MOD_FUNC*)data=modifier::COS;
        else if (svalue == "tan")
            *(modifier::MOD_FUNC*)data=modifier::TAN;
        else {
            *err_msg = ", unrecognised modifier function " + svalue;
            delete (modifier::MOD_FUNC*)data;
            return 0;
        }
        *conv << svalue; // ignored if error situ
        break;
    case CAT_LOGIC:
        data = new logictrigger::LOGIC_FUNC;
        *synthfile >> svalue;
        if (svalue == "and")
            *(logictrigger::LOGIC_FUNC*)data = logictrigger::AND;
        else if (svalue == "or")
            *(logictrigger::LOGIC_FUNC*)data = logictrigger::OR;
        else if (svalue == "xor")
            *(logictrigger::LOGIC_FUNC*)data = logictrigger::XOR;
        else {
            *err_msg =
                ", unrecognised logic function " + svalue;
            delete (logictrigger::LOGIC_FUNC*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_LOOP_MODE:	// enum sampler::LOOP_MODE
        data = new LOOP_MODE;
        *synthfile >> svalue;
        if (svalue == "off")
            *(LOOP_MODE*)data = LOOP_OFF;
        else if (svalue == "fwd")
            *(LOOP_MODE*)data = LOOP_FWD;
        else if (svalue == "rev")
            *(LOOP_MODE*)data = LOOP_REV;
        else if (svalue == "bi")
            *(LOOP_MODE*)data = LOOP_BI;
        else {
            *err_msg = ", unrecognised sampler loop_mode " + svalue;
            delete (LOOP_MODE*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_PLAY_DIR:
        data = new PLAY_DIR;
        *synthfile >> svalue;
        if (svalue == "fwd")
            *(PLAY_DIR*)data = PLAY_FWD;
        else if (svalue == "rev")
            *(PLAY_DIR*)data = PLAY_REV;
        else {
            *err_msg = ", unrecognised sampler play_dir " + svalue;
            delete (PLAY_DIR*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_PLAY_MODE:
        data = new PLAY_MODE;
        *synthfile >> svalue;
        if (svalue == "stop")
            *(PLAY_MODE*)data = PLAY_STOP;
        else if (svalue == "wrap")
            *(PLAY_MODE*)data = PLAY_WRAP;
        else if (svalue == "bounce")
            *(PLAY_MODE*)data = PLAY_BOUNCE;
        else if (svalue == "jump")
            *(PLAY_MODE*)data = PLAY_JUMP;
        else {
            *err_msg = ", unrecognised sampler play_mode " + svalue;
            delete (PLAY_MODE*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_JUMP_DIR:
        data = new JUMP_DIR;
        *synthfile >> svalue;
        if (svalue == "play")
            *(JUMP_DIR*)data = JUMP_PLAY_DIR;
        else if (svalue == "loop")
            *(JUMP_DIR*)data = JUMP_LOOP_DIR;
        else {
            *err_msg = ", unrecognised sampler jump_mode " + svalue;
            delete (JUMP_DIR*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_METER:
        data = new timesig;
        if (!(*synthfile >> ((timesig*)data)->beatsperbar)) {
            *err_msg = ", expected integer value for beats per bar";
            delete (timesig*)data;
            return 0;
        }
        char ch;
        while (synthfile->get(ch) && ch == ' ');
        if (ch != '/') {
            *err_msg = ", expected seperator /";
            delete (timesig*)data;
            return 0;
        }
        if (!(*synthfile >> ((timesig*)data)->beatvalue)) {
            *err_msg = ", expected integer value";
            delete (timesig*)data;
            return 0;
        }
        *conv << ((timesig*)data)->beatsperbar;
        *conv << "/";
        *conv << ((timesig*)data)->beatvalue;
        break;
    case CAT_ADSRSECT:
        data = new adsr_coord::SECT;
        *synthfile >> svalue;
        if (svalue == "attack")
            *(adsr_coord::SECT*)data = adsr_coord::ADSR_ATTACK;
        else if (svalue == "decay")
            *(adsr_coord::SECT*)data = adsr_coord::ADSR_DECAY;
        else if (svalue == "sustain")
            *(adsr_coord::SECT*)data = adsr_coord::ADSR_SUSTAIN;
        else if (svalue == "release")
            *(adsr_coord::SECT*)data = adsr_coord::ADSR_RELEASE;
        else {
            *err_msg = ", unrecognised ADSR section " + svalue;
            delete (adsr_coord::SECT*)data;
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_DOBJ:
        if (!(*synthfile >> svalue)) {
            *err_msg = ", unable to read name of data object";
            return 0;
        }
        data = dobj::get_dobjlist()->get_dobj_by_name(svalue.c_str());
        if (!data) {
            *err_msg = ", no data object named " + svalue + " found";
            return 0;
        }
        *conv << svalue;
        break;
    case CAT_SYNTHMOD:
        if (!(*synthfile >> svalue)) {
            *err_msg = ", unable to read synth module name";
            return 0;
        }
        data =
         synthmod::get_modlist()->get_synthmod_by_name(svalue.c_str());
        if (!data) {
            *err_msg = ", no synth module found named " + svalue;
            return 0;
        }
        *conv << svalue;
        break;
    default:
        *err_msg = "invalid category";
        return 0;
    }
    return data;
}

void synthfilereader::destroy_iocat_data(IOCAT iocat, void* data)
{
    switch(iocat)
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
    case CAT_TRIG:  // cat_trig same as
    case CAT_STATE: // cat_state
        delete (STATUS*)data;
        break;
    case CAT_STRING:
        delete [] (char*)data;
        break;
    case CAT_MOD_FUNC:
        delete (modifier::MOD_FUNC*)data;
        break;
    case CAT_LOGIC:
        delete (logictrigger::LOGIC_FUNC*)data;
        break;
    case CAT_LOOP_MODE:
        delete (LOOP_MODE*)data;
        break;
    case CAT_PLAY_DIR:
        delete (PLAY_DIR*)data;
        break;
    case CAT_PLAY_MODE:
        delete (PLAY_MODE*)data;
        break;
    case CAT_JUMP_DIR:
        delete (JUMP_DIR*)data;
        break;
    case CAT_METER:
        delete (timesig*)data;
        break;
    case CAT_ADSRSECT:
        delete (adsr_coord::SECT*)data;
        break;
    case CAT_SYNTHMOD:	// don't delete as it's not created
    case CAT_DOBJ:		// in read_iocat_value(...)
    default:
        break;
    }
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
                return false;// error end of file
        }
        *synthfile >> *buff;
        if (synthfile->eof())
            return false; // error end of file -- do i need this again?
    }
    return true;          // i'm happy - honest
}

// (snip swearing,  wingeing, and excuse making, for what follows ;)

bool synthfilereader::eff_ing_header_bodge(unsigned long *samplerate,
        short *bpm, short *beatspermeasure, short *beatvalue)
{
    ifstream headerfile;
    string hf_name;
    *synthfile >> hf_name;
    // stop reading from that (synthfile) for mo now we got header name.
    char const* path = synthmod::get_path();
    if (!(hf_name[0] == '/' || path == NULL)) {
        string tmp = hf_name;
        hf_name = path + tmp;
    } 
    headerfile.open(hf_name.c_str());// and open header file and be really
    
    // fussy about layout 'cos i'm not messing about with whitespace
    // and remark processing here.  but first check it's opened oK
    if (!headerfile.is_open()) {
        *err_msg = "Requested header refused open: " + hf_name;
        genstatus = HALTED;
        return false;
    }
    if (verbose)
        cout << "\nReading header info from file: " + hf_name;
    headerfile >> *buff;
    if (*buff == "samplerate") {
        if (!(headerfile >> *samplerate)) {
            *err_msg = "In header file: " + hf_name;
            *err_msg += " expected value for samplerate.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        if (*samplerate > 4000 && *samplerate < 200000) {
            if (verbose)
                cout << "\nsamplerate set at " << *samplerate;
        } else {
            *conv << *samplerate;
            *err_msg = "in header: " + hf_name +
                       " Invalid samplerate: " + conv->str() +
                       ". valid values between 4000 and 200000.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
    } else {
        *err_msg = "in header: " + hf_name +
                   " expected 'samplerate' got " + *buff + " instead.";
        genstatus = HALTED;
        headerfile.close();
        return false;
    }
    headerfile >> *buff;
    if (*buff == "bpm") {
        if (!(headerfile >> *bpm)) {
            *err_msg = "in header: " + hf_name 
             + " expected value for bpm";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        if (*bpm >= 20 && *bpm <= 1000) {
            if (verbose)
                cout << "\nbpm set at " << *bpm;
        } else {
            *conv << *bpm;
            *err_msg = "in header: " + hf_name +
                       " Invalid bpm: " + conv->str() +
                       ". valid values between 20 and 1000.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
    } else {
        *err_msg = "in header: " + hf_name + " Expected 'bpm' got "
                   + *buff + " instead.";
        genstatus = HALTED;
        headerfile.close();
        return false;
    }
    headerfile >> *buff;
    if (*buff == "signature") {
        if (!(headerfile >> *beatspermeasure)) {
            *err_msg = "in header: " + hf_name
             + " Expected value for time signature - beats per measure.";
            genstatus = HALTED;
            return false;
        }
        if (*beatspermeasure < 1 || *beatspermeasure > 16) {
            *conv << *beatspermeasure;
            *err_msg = "in header: " + hf_name 
             + " Invalid time signature with beats per measure: "
             + conv->str() + ". valid value in range 1 to 16.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        char ch;
        while (headerfile.get(ch)) {
            if (ch == '/')
                break;
        }
        if (headerfile.eof()) {
            *err_msg =
                "Unexpected end of file while scanning time signature.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        if (!(headerfile >> *beatvalue)) {
            *err_msg = "Expected value for time signature - beat value.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        if (*beatvalue < 1 || *beatvalue > 128) {
            *conv << *beatvalue;
            *err_msg = "in header: " + hf_name 
             + " Invalid time signature with beat value: "
             + conv->str() + ". valid value in range 1 to 128.";
            genstatus = HALTED;
            headerfile.close();
            return false;
        }
        if (verbose) {
            cout << "\ntime signature set to " << *beatspermeasure;
            cout << "/" << *beatvalue;
        }
    } else {
        *err_msg = "in header: " + hf_name + " Expected 'signature'.";
        genstatus = HALTED;
        headerfile.close();
        return false;
    }
    filestatus = FILE_READY;
    headerfile.close();  // holy mudcow we made it!
    return true;
}

// (snip more gripes about not coding for 8 months)

#endif
#endif
