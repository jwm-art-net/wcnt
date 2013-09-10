#ifndef SYNTHFILEREADER_H
#include "../include/synthfilereader.h"

// don't put this #include back in this file's header
#include "../include/connectorlist.h"
// it causes compilation problems!

synthfilereader::synthfilereader() :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0), mname_item(0), dname_item(0),
 modname(0), dobjname(0), wc_file_type(WC_INCLUDE_FILE), wcnt_id(0),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), wc_err_msg(0), conv(0), verbose(false)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    wc_err_msg = new string;
    // don't create conv here.
    create_params();
    dobjnamelist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    modnamelist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

synthfilereader::synthfilereader(WC_FILE_TYPE ft) :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0), mname_item(0), dname_item(0),
 modname(0), dobjname(0), wc_file_type(ft),wcnt_id(0),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), wc_err_msg(0), conv(0), verbose(false)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    wc_err_msg = new string;
    // don't create conv here.
    create_params();
    dobjnamelist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    modnamelist =
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

synthfilereader::~synthfilereader()
{
    synthfile->close();
    delete [] wc_filename;
    delete synthfile;
    delete buff;
    if (command)
        delete command;
    delete synthheader;
    delete wc_err_msg;
    if (conv) delete conv;
        goto_first_dobjname();
    while(dobjname) {
        delete dobjname;
        goto_next_dobjname();
    }
    delete dobjnamelist;
    goto_first_modname();
    while(modname) {
        delete modname;
        goto_next_modname();
    }
    delete modnamelist;
}

void synthfilereader::set_wc_filename(char const* filename)
{
    if (wc_filename)
        delete [] wc_filename;
    const char* path = synthmod::get_path();
    if (wc_file_type == WC_MAIN_FILE
        || *filename == '/' || path == NULL)
    {
        wc_filename = new char[strlen(filename)+1];
        strcpy(wc_filename, filename);
    }
    else {
        char* ptr;
        wc_filename = new char[strlen(filename) + strlen(path) + 1];
        strcpy(wc_filename, path);
        ptr = wc_filename + strlen(path);
        strcpy(ptr, filename);
    }
    return;
}

modnamedobj* synthfilereader::add_modname(modnamedobj* mn)
{
    if (!(mname_item = modnamelist->add_at_tail(mn)))
        return modname = 0;
    return modname = (modnamedobj*)mname_item->get_data();
}

/*
void synthfilereader::delete_modname(const char* mn)
{
    goto_first_modname();
    while (modname) {
        if (strcmp(mn, modname->get_modname()) == 0) {
            modnamelist->unlink_item(mname_item);
            delete mname_item;
            delete modname;
            goto_first_modname();
        }
        else
            goto_next_modname();
    }
}
*/

dobjnamedobj* synthfilereader::add_dobjname(dobjnamedobj* dn)
{
    if (!(dname_item = dobjnamelist->add_at_tail(dn)))
        return dobjname = 0;
    return dobjname = (dobjnamedobj*)dname_item->get_data();
}

/*
bool synthfilereader::delete_dobjname(const char* dn)
{
    goto_first_dobjname();
    while (dobjname) {
        if (strcmp(dn, dobjname->get_dobjname()) == 0) {
            dobjnamelist->unlink_item(dname_item);
            delete dname_item;
            delete dobjname;
            goto_first_dobjname();
        }
        else
            goto_next_dobjname();
    }
}
*/

bool synthfilereader::read_and_create()
{
    if (!wc_filename)
        return false;
    switch (open_file(wc_filename)) {
        case synthfilereader::NOT_FOUND:
            *wc_err_msg = "\nfile ";
            *wc_err_msg += wc_filename;
            *wc_err_msg += " not found.\n";
            return false;
        case synthfilereader::INVALID_HEADER:
            *wc_err_msg = "\nfile ";
            *wc_err_msg += wc_filename;
            *wc_err_msg += " does not contain a valid header.\n";
            return false;
        case synthfilereader::FILE_OPEN:
            break;
        case synthfilereader::FILE_READY:
            *wc_err_msg = "\nfile ";
            *wc_err_msg += wc_filename;
            *wc_err_msg += " inspires premature optimism.\n";
            return false;
    }
    unsigned long srate;
    double bpm;
    short bp_msr;
    short b_val;
    if (wc_file_type == WC_INCLUDE_FILE) {
        if (verbose) // pretty please right?
            cout << "\n";
        cout << "\n  Including ";
    }
    else
        cout << "\nProccessing ";
    cout << wc_filename;
    if (!read_header(&srate, &bpm, &bp_msr, &b_val)) {
        return false;
    }
    if (wc_file_type == WC_MAIN_FILE) {
        audio_samplerate = srate;
        sm_beats_per_minute = bpm;
        sm_beats_per_measure = bp_msr;
        sm_beat_value = b_val;
    }
    else {
        if (srate != audio_samplerate
            || bpm != sm_beats_per_minute
            || bp_msr != sm_beats_per_measure
            || b_val != sm_beat_value) 
        {
            *wc_err_msg = "\nfile ";
            *wc_err_msg += wc_filename;
            *wc_err_msg += " has conflicting header information ";
            *wc_err_msg += "with that already in use.";
            return false;
        }
    }
    string const *com = read_command();
    while (*com != wcnt_id)
    {
        if (!com) {
            return false;
        }
        if (dobj::get_dobjnames()->get_type(com->c_str()) 
            != dobjnames::DOBJ_FIRST)
        {
            if (!read_and_create_dobj(com))
                return false;
        }
        else {
            if (!read_and_create_synthmod(com))
                return false;
        }
        com = read_command();
    }
    if (verbose)
        cout << "\n\nend wcnt/jwmsynth: " << wc_filename;
    return true;
}

bool synthfilereader::read_and_create_synthmod(string const* com)
{
    synthmod *mod = read_synthmodule(com);
    if (!mod) {
        return false;
    }
    if (verbose)
        cout << "\nend " << mod->get_username();
    if (include_mod(mod->get_username())) {
        if (!synthmod::get_modlist()->add_module(mod)) {
            *wc_err_msg = "\ncould not add module ";
            *wc_err_msg += mod->get_username();
            *wc_err_msg += " to list.";
            delete mod;
            return false;
        }
    }
    else {
        if (verbose) {
            cout << "\n\n***** " << mod->get_username();
            cout << " is not being included *****";
        }
        delete mod;
    }
    return true;
}

bool synthfilereader::read_and_create_dobj(string const* com)
{
    dobj* dbj = read_dobj(com);
    if (!dbj)
        return false;
    if (!dbj->validate()) {
        *wc_err_msg = *dbj->get_error_msg();
        return false;
    }
    string dbjuname = dbj->get_username();
    if (include_dbj(dbj->get_username())) {
        if (!dobj::get_dobjlist()->add_dobj(dbj)) {
            *wc_err_msg = "\ncould not add data object ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += " to list.";
            return false;
        }
        switch(dbj->get_object_type())
        {
            default:
            // nothing done by default
                break;
            case dobjnames::DEF_WCFILE:
            {
                synthfilereader* wcf = (synthfilereader*)dbj;
                wcf->set_wcnt_id(wcnt_id);
                if (verbose)
                    wcf->set_verbose();
                if (!(wcf->read_and_create())) { // ooooh
                    *wc_err_msg = wcf->get_wc_error_msg();
                    return false;
                }
                break;
            }
            case dobjnames::DEF_PARAMEDITOR:
            {
                parameditor* pe = (parameditor*)dbj;
                if (verbose)
                    pe->set_verbose();
                if (!pe->do_param_edits()) {
                    *wc_err_msg = *dobj::get_error_msg();
                    return false;
                }
                break;
            }
            case dobjnames::DEF_INPUTEDITOR:
            {
                inputeditor* ie = (inputeditor*)dbj;
                if (verbose)
                    ie->set_verbose();
                if (!ie->create_connectors()) {
                    *wc_err_msg = *dobj::get_error_msg();
                    return false;
                }
                break;
            }
        }
    }
    else {
        if (verbose) {
            cout << "\n\n***** " << dbj->get_username();
            cout << " is not being included *****";
        }
        delete dbj;
    }
    if (verbose)
        cout << "\nend " << dbjuname;
    return true;
}

bool synthfilereader::include_mod(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    goto_first_modname();
    if (!modname)
        return (mod_action == WC_INCLUDE) ? false : true;
    while(modname) {
        if (strcmp(modname->get_modname(), name) == 0)
            return (mod_action == WC_INCLUDE) ? true : false;
        goto_next_modname();
    }
    return (mod_action == WC_INCLUDE) ? false : true;
}

bool synthfilereader::include_dbj(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    goto_first_dobjname();
    if (!dobjname)
        return (dobj_action == WC_INCLUDE) ? false : true;
    while(dobjname) {
        if (strcmp(dobjname->get_dobjname(), name) == 0)
            return (dobj_action == WC_INCLUDE) ? true : false;
        goto_next_dobjname();
    }
    return (dobj_action == WC_INCLUDE) ? false : true;
}

/*
bool synthfilereader::transmit_excluded(synthfilereader* sfr)
{
    if (mod_action == WC_EXCLUDE) {
        goto_first_modname();
        while (modname) {
            if (sfr->get_module_action() == WC_EXCLUDE) {
                modnamedobj* mnd = new modnamedobj;
                mnd->set_modname(modname->get_modname());
                sfr->add_modname(mnd);
            }
            else
                sfr->delete_modname(modname->get_modname());
        }
    }
    if (dobj_action == WC_EXCLUDE) {
        goto_first_dobjname();
        while (dobjname) {
            if (sfr->get_dobj_action() == WC_EXCLUDE) {
                dobjnamedobj* dnd = new dobjnamedobj;
                dnd->set_dobjname(dobjname->get_dobjname());
                sfr->add_dobjname(dnd);
            }
            else
                sfr->delete_dobjname(dobjname->get_dobjname());
        }
    }
}
*/

synthmod *const synthfilereader::read_synthmodule(string const *com)
{
    synthmodnames::SYNTH_MOD_TYPE
    smt = synthmod::get_modnames()->get_type(com->c_str());
    if (smt == synthmodnames::MOD_FIRST 
        || smt == synthmodnames::MOD_NONEZERO)
    {
        *wc_err_msg = "\nUnrecognised wcnt/jwmsynth module: " + *com;
        return 0;
    }
    if (smt == synthmodnames::MOD_WCNT) {
        if (synthmod::get_modlist()->
            get_first_of_type(synthmodnames::MOD_WCNT))
        {
            *wc_err_msg
               = "\nCannot create more than one wcnt_exit module ";
            *wc_err_msg += *com;
            return 0;
        }
    }
    string modname;
    *synthfile >> modname;
    if (strcmp(modname.c_str(), "off") == 0) {
        *wc_err_msg = "\ncannot use reserved word off to name ";
        *wc_err_msg += *com;
        return 0;
    }
    if (include_mod(modname.c_str())) {
        if (synthmod::get_modlist()->
            get_synthmod_by_name(modname.c_str()))
        {
            *wc_err_msg = "\nsynth module already exists named ";
            *wc_err_msg += modname;
            return 0;
        }
        dobj* dbj =
            dobj::get_dobjlist()->get_dobj_by_name(modname.c_str());
        if (dbj){ // formality because of parameditor.cc workings.
            *wc_err_msg = "\nwill not name ";
            *wc_err_msg += *com;
            *wc_err_msg += " ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += 
             ", the name has already been taken by data object of type ";
            *wc_err_msg +=
             dobj::get_dobjnames()->get_name(dbj->get_object_type());
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (verbose) {
        cout << "\n\n================================";
        cout << "\nCreating synth module " << modname;
    }
    synthmod*
     sm = synthmod::get_modlist()->create_module(smt, modname.c_str());
    if (!read_dobjs(sm)){
        *wc_err_msg = sm->get_username() + *wc_err_msg;
        *wc_err_msg = "\nIn module " + *wc_err_msg;
        delete sm;
        return 0;
    }
    if (!read_inputs(sm)){
        *wc_err_msg = sm->get_username() + *wc_err_msg;
        *wc_err_msg = "\nIn module " + *wc_err_msg;
        delete sm;
        return 0;
    }
    if (!read_params(sm)){
        *wc_err_msg = sm->get_username() + *wc_err_msg;
        *wc_err_msg = "\nIn module " + *wc_err_msg;
        delete sm;
        return 0;
    }
    com = read_command();
    if (*com != modname) {
        *wc_err_msg = "\nIn module ";
        *wc_err_msg += sm->get_username();
        *wc_err_msg += ", expected definition termination ";
        *wc_err_msg += sm->get_username();
        *wc_err_msg += ", got " + *com + " instead.";
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
    if (subtype < dobjnames::DOBJ_DEFS ||
            subtype >= dobjnames::DOBJ_SYNTHMOD)
    {
        *wc_err_msg = "\n" + *com + " is not a standalone data object.";
        return 0;
    }
    string dobjname;
    *synthfile >> dobjname;
    if (strcmp(dobjname.c_str(), "off") == 0) {
        *wc_err_msg = "\ncannot use reserved word off to name ";
        *wc_err_msg += *com;
        return 0;
    }
    if (include_dbj(dobjname.c_str())) {
        if (dobj::get_dobjlist()->get_dobj_by_name(dobjname.c_str())) {
            *wc_err_msg = "\ndata object " + *com
                      + " already exists named "+dobjname;
            return 0;
        }
        synthmod* sm =
         synthmod::get_modlist()->get_synthmod_by_name(dobjname.c_str());
        if (sm){ // formality because of parameditor.cc workings.
            *wc_err_msg = "\nwill not name ";
            *wc_err_msg += *com;
            *wc_err_msg += " ";
            *wc_err_msg += sm->get_username();
            *wc_err_msg += 
                ", the name has already been taken by module of type ";
            *wc_err_msg +=
                synthmod::get_modnames()->get_name(sm->get_module_type());
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (verbose) {
        cout << "\n\n================================";
        cout << "\nCreating data object " << dobjname;
    }
    dobj* dbj = dobj::get_dobjlist()->create_dobj(dobjtype);
    if (dbj == 0) {
        *wc_err_msg = "\ncould note create data object of type " + *com;
        return 0;
    }
    dbj->set_username(dobjname.c_str());
    // read dobj parameters (if any)
    if (!read_dobj_params(dbj, 0)){
        *wc_err_msg = dbj->get_username() + *wc_err_msg;
        *wc_err_msg = "\nIn data object " + *wc_err_msg;
        delete dbj;
        return 0;
    }
    // read dobj's dobjies (if any)
    if (!read_dobjs(dbj)){
        delete dbj;
        return 0;
    }
    if (include_dbj(dbj->get_username())) {
        stockerrs::ERR_TYPE et = dbj->validate();
        if (et != stockerrs::ERR_NO_ERROR) {
            *wc_err_msg = "\nIn data object ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += ", parameter ";
            *wc_err_msg += *dbj->get_error_msg();
            *wc_err_msg += " ";
            *wc_err_msg += stock_errs->get_prefix_err(et);
            *wc_err_msg += stock_errs->get_err(et);
            delete dbj;
            return 0;
        }
    }
    com = read_command();
    if (*com != dobjname) {
        *wc_err_msg = "\nIn data object ";
        *wc_err_msg += dbj->get_username();
        *wc_err_msg += ", expected definition termination ";
        *wc_err_msg += dbj->get_username();
        *wc_err_msg += ", got " + *com + " instead.";
        delete dbj;
        return 0;
    }
    // read_dobjs(dobj*) stops when it encounters the username of the dobj
    return dbj;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_dobjs(dobj* dbj)
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
    // get first encapsulating list type
    dobjdobjlist* enc_list = get_topdobjlist()->get_first_of_type(dt);
    if (!enc_list) // not dobjs in this dbj dobj
        return true;
    while(enc_list) {
        dobjdobj* enc_dobj = enc_list->goto_first();
        string enc_com = *read_command();
        dobjnames::DOBJ_TYPE enc_type = enc_dobj->get_dobj_sprog();
        char const* enc_name = dbjnames->get_name(enc_type);
        if (enc_com != enc_name) {
            *wc_err_msg = "\nIn data object ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += ", expected ";
            *wc_err_msg += enc_name;
            *wc_err_msg += ", got ";
            *wc_err_msg += enc_com;
            *wc_err_msg += " instead";
            return false;
        }
        dobjdobjlist* dd_list;
        dd_list = enc_list->get_dobjdobjlist_for_dobjtype(enc_type);
        dobjdobj* dd = dd_list->goto_first();
        if (verbose)
            cout << "\n--------\nfor " << enc_name;
        while(dd) {
            string com = *read_command();
            dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
            char const* sprogname = dbjnames->get_name(sprogtype);
            while (com != enc_name) {
                if (com != sprogname) {
                    *wc_err_msg = "\nIn data object ";
                    *wc_err_msg += dbj->get_username();
                    *wc_err_msg += ", expected ";
                    *wc_err_msg += sprogname;
                    *wc_err_msg += ", got ";
                    *wc_err_msg += com;
                    *wc_err_msg += " instead";
                    delete dd_list;
                    return false;
                }
                dobj* sprog =
                    dobj::get_dobjlist()->create_dobj(sprogtype);
                if (sprog == 0) {
                    *wc_err_msg = "\n***major error***";
                    *wc_err_msg += "\ncould not create data object ";
                    *wc_err_msg += sprogname;
                    *wc_err_msg += " for data object ";
                    *wc_err_msg += dbj->get_username();
                    delete dd_list;
                    return false;
                }
                if (verbose)
                    cout << "\n--------\ncreating " << sprogname;
                if (!read_dobj_params(sprog, enc_com.c_str())) {
                    *wc_err_msg = dbj->get_username() + *wc_err_msg;
                    *wc_err_msg = "\nIn data object " + *wc_err_msg;
                    delete sprog;
                    delete dd_list;
                    return false;
                }
                if (include_dbj(dbj->get_username())) {
                    stockerrs::ERR_TYPE et = sprog->validate();
                    if (et != stockerrs::ERR_NO_ERROR) {
                        *wc_err_msg = "\nIn data object ";
                        *wc_err_msg += dbj->get_username();
                        *wc_err_msg += ", error in ";
                        *wc_err_msg += sprogname;
                        *wc_err_msg += ", ";
                        *wc_err_msg += *sprog->get_error_msg();
                        *wc_err_msg += " ";
                        *wc_err_msg += stock_errs->get_prefix_err(et);
                        *wc_err_msg += stock_errs->get_err(et);
                        delete sprog;
                        delete dd_list;
                        return false;
                    }
                    // add sprog to dbj, not dobjlist  . . .
                    if (!dbj->add_dobj(sprog)) {
                        *wc_err_msg = "\n***major error***";
                        *wc_err_msg += "\ncould not add data object ";
                        *wc_err_msg += sprogname;
                        *wc_err_msg += " to data object ";
                        *wc_err_msg += dbj->get_username();
                        delete sprog;
                        delete dd_list;
                        return false;
                    }
                }
                else
                    delete sprog;
                if (verbose)
                    cout << "\nadded " << sprogname;
                com = *read_command();
            }
            dd = dd_list->goto_next();
        }
        delete dd_list;
        enc_list = get_topdobjlist()->get_next_of_type();
    }
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
    moddobjlist* mdbjslist = synthmod::get_moddobjlist();
    moddobj* mdbj = mdbjslist->get_first_of_type(smt);
    dobjnames* dbjnames = dobj::get_dobjnames();
    while(mdbj) { // module may contain more than one list (ie timemap)
        string dbjname = *read_command();//one of envelope/track etc
        dobjnames::DOBJ_TYPE dt = mdbj->get_first_child();
        char const* xdbjname = dbjnames->get_name(dt);
        if (dbjname != xdbjname) {
            *wc_err_msg = ", expected ";
            *wc_err_msg += xdbjname;
            *wc_err_msg += " got ";
            *wc_err_msg += dbjname;
            *wc_err_msg += " instead";
            return false;
        }
        if (verbose)
            cout << "\n--------\nfor " + dbjname;
        dobjdobjlist* ddlist = mdbj->get_dobjdobjlist();
        dobjdobj* dd = ddlist->goto_first();
        while(dd) {//maybe the dobj has one or more dobjies inside?
            dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
            char const* xsprogname = dbjnames->get_name(sprogtype);
            string com = *read_command();
            // now read the list of items (each item's type is sprogtype)
            while (com != dbjname) {
                if (com != xsprogname) {
                    // check name of item matches expected
                    *wc_err_msg = ", data object ";
                    *wc_err_msg += xdbjname;
                    *wc_err_msg += ", expected ";
                    *wc_err_msg += xsprogname;
                    *wc_err_msg += ", got ";
                    *wc_err_msg += com;
                    *wc_err_msg += " instead.";
                    return false;
                }
                if (verbose)
                    cout << "\n--------\ncreating " << xsprogname;
                dobj* dbj = dobj::get_dobjlist()->create_dobj(sprogtype);
                if (dbj == 0) { // failed to create dobj
                    *wc_err_msg =
                     "\n***major error***\ncould not create data object ";
                    *wc_err_msg += xsprogname;
                    *wc_err_msg += ", data object ";
                    *wc_err_msg += xdbjname;
                    return false;
                }
                if (!read_dobj_params(dbj, 0)) {
                    *wc_err_msg = xsprogname + *wc_err_msg;
                    *wc_err_msg = ", " + *wc_err_msg;
                    *wc_err_msg = xdbjname + *wc_err_msg;
                    *wc_err_msg = ", " + *wc_err_msg;
                    *wc_err_msg += *dbj->get_error_msg();
                    delete dbj;
                    return false;
                }
                stockerrs::ERR_TYPE et = dbj->validate();
                if (et!= stockerrs::ERR_NO_ERROR) {
                    *wc_err_msg = ", data object ";
                    *wc_err_msg += xdbjname;
                    *wc_err_msg += ", ";
                    *wc_err_msg += *dbj->get_error_msg();
                    *wc_err_msg += " ";
                    *wc_err_msg += stock_errs->get_prefix_err(et);
                    *wc_err_msg += stock_errs->get_err(et);
                    delete dbj;
                    return false;
                }
                if (inc_current) {
                    // add to synthmodule, not dobjlist  . . .
                    if (!sm->add_dobj(dbj)) {
                        *wc_err_msg =
                        "\n*major error*\ncould not add data object ";
                        *wc_err_msg += xsprogname;
                        *wc_err_msg += *sm->get_error_msg();
                        *wc_err_msg += ", in data object ";
                        *wc_err_msg += xdbjname;
                        delete dbj;
                        return false;
                    }
                }
                else
                    delete dbj;
                if (verbose)
                    cout << "\nadded data object " << xsprogname;
                com = *read_command();
            }
            dd = ddlist->goto_next();
        }
        mdbj = mdbjslist->get_next_of_type();
    }
    return true;
}

//------------------------------------------------------------------------
// bool synthfilereader::read_dobj_params(dobj* dbj)
// reads dparams for all data objects
//------------------------------------------------------------------------
// return true  if succesful.
// return false on fail.
// enda & endb in most instances are both NULL. they should only be used
// when reading an EDIT dobj type contained in an EDIT_LST dobjtype. they
// point to a char string which is the EDIT dobj name, and another which 
// is the EDITLIST dobj name and are used to terminate reading strings
// to add to the string list paramnames::PAR_TYPE PAR_STR_LIST.

bool synthfilereader::
read_dobj_params(dobj* dbj, const char* endterm)
{
    dobjnames::DOBJ_TYPE dobjtype = dbj->get_object_type();
    dobjparamlist* parlist = dobj::get_dparlist()->
                              get_dobjparamlist_for_dobj_type(dobjtype);
    dobjparam* param = parlist->goto_first();
    char const* enda = dobj::get_dobjnames()->get_name(dobjtype);
    while(param) {
        if (conv)
            delete conv;
        conv = new ostringstream;
        paramnames::PAR_TYPE pt = param->get_partype();
        string parname;
        if (pt != paramnames::PAR_STR_UNNAMED
            && pt != paramnames::PAR_STR_LIST) 
        {
            parname = *read_command();
        }
        string* datastr;
        if (pt == paramnames::PAR_STR_LIST) {
            datastr = (string*)read_string_list_param(enda, endterm);
            if (!datastr)
                return false;
        }
        else {
            datastr = new string;
            *synthfile >> *datastr;
        }
        if (inc_current) {
            char const* par = parname.c_str();
            char const* val = datastr->c_str();
            if (!setpar::set_dobj_param(dbj, par, pt, val, conv)) {
                *wc_err_msg = setpar::err_msg;
                delete datastr;
                delete parlist;
                return false;
            }
        }
        delete datastr;
        if (verbose) {
            cout << "\nparameter ";
            cout << parname << "\t" << conv->str();
        }
        param = parlist->goto_next();
    }
    delete parlist;
    return true;
}

const string* 
synthfilereader::read_string_list_param
    (const char* enda, const char* endb)
{
    if (enda == 0 && endb == 0) {
        *wc_err_msg = "\nread_string_list_param(char*, char*)";
        *wc_err_msg += " called with NULL arguements. error in";
        *wc_err_msg += " code implimentation - somewhere!";
        invalidate();
        return 0;
    }
    string strlist;
    const string* com;
    while(true) {
        if (!(com = read_command()))
            return 0;
        if (enda) {
            if (strcmp(com->c_str(), enda) == 0) {
                // tell read_command() the next command it should
                // return has already been read and it is enda:
                command = new string(enda);
                return new string(strlist);
            }
        }
        if (endb) {
            if (strcmp(com->c_str(), endb) == 0) {
                command = new string(endb);
                return new string(strlist);
            }
        }
        strlist += *com;
        strlist += " ";
    }
    return 0;
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
    bool inc_mod = include_mod(sm->get_username());
    modinputlist* inlist;
    inlist = synthmod::get_inputlist()->get_inputlist_for_module(sm);
    modinput* inp = inlist->goto_first();
    inputnames* innames = synthmod::get_inputnames();
    outputnames* outnames = synthmod::get_outputnames();
    string inputname;
    if (verbose && inp)
        cout << "\n--------";
    while(inp) { // step through each  input for module
        inputname = *read_command();
        if (innames->get_name(inp->get_inputtype()) != inputname){
            *wc_err_msg = ", expected input type ";
            *wc_err_msg += innames->get_name(inp->get_inputtype());
            *wc_err_msg += ", got ";
            *wc_err_msg += inputname;
            *wc_err_msg += " instead";
            delete inlist;
            return false;
        } else {
            inputnames::IN_TYPE input_type
             =innames->get_type(inputname.c_str());
            string outmodname;
            *synthfile >> outmodname;
            if (outmodname == "off") {
                if (inc_mod) {
                    outputnames::OUT_TYPE offout =
                     outnames->get_nonezerotype(
                     innames->get_category(input_type));
                    connector* connect = new
                     connector(sm, input_type,
                        outmodname.c_str(), offout);
                    synthmod::get_connectlist()->add_connector(connect);
                    if (verbose) {
                        cout << "\nadded connector ";
                        cout << inputname << "\toff";
                    }
                }
            }
            else
            {
                string outputname;
                *synthfile >> outputname;
                outputnames::OUT_TYPE output_type =
                    outnames->get_type(outputname.c_str());
                if (output_type == outputnames::OUT_FIRST) {
                    *wc_err_msg = ", input ";
                    *wc_err_msg += inputname;
                    *wc_err_msg += " set with unknown output type ";
                    *wc_err_msg += outputname;
                    delete inlist;
                    return false;
                }
                if (innames->get_category(input_type)
                        != outnames->get_category(output_type))
                {
                    *wc_err_msg = ", input ";
                    *wc_err_msg += inputname;
                    *wc_err_msg += " set with wrong category of output, ";
                    *wc_err_msg += outputname;
                    delete inlist;
                    return false;
                }
                if (inc_mod) {
                    connector* connect =
                        new connector(sm, input_type, 
                            outmodname.c_str(), output_type);
                    synthmod::get_connectlist()->add_connector(connect);
                    if (verbose){
                        cout << "\nadded connector " << inputname << "\t";
                        cout << outmodname << "\t" << outputname;
                    }
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
        *wc_err_msg = "\n*** Big problems getting params ***";
        return false;
    }
    modparam* param = parlist->goto_first();
    string paramname;
    if (verbose && param)
        cout << "\n--------";
    while(param) {
        paramname = *read_command();
        paramnames::PAR_TYPE pt = param->get_paramtype();
        if (conv)
            delete conv;
        conv = new ostringstream;
        string datastr;
        *synthfile >> datastr;
        if (inc_current) {
            char const* par = paramname.c_str();
            char const* val = datastr.c_str();
            if (!setpar::set_mod_param(sm, par, pt, val, conv)) {
                *wc_err_msg = setpar::err_msg;
                delete parlist;
                return false;
            }
        }
        if (verbose) {
            cout << "\nparameter ";
            cout << paramname << "\t" << conv->str();
        }
        param = parlist->goto_next();
    }
    delete parlist;
    return true;
}

synthfilereader::FILE_STATUS
synthfilereader::open_file(char *synthfilename)
{
    wc_filename = new char[strlen(synthfilename) + 1];
    strcpy(wc_filename, synthfilename);
    synthfile->open(wc_filename);
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
    unsigned long *samplerate, double *bpm,
    short *beatspermeasure, short *beatvalue)
{
    if (conv) delete conv;
    conv = new ostringstream;
    if (filestatus != FILE_OPEN) {
        *wc_err_msg = "\nArrrp! Programmer Error! Shpoooochuuha...\n";
        return 0;
    }
    if (!skip_remarks()) {
        *wc_err_msg = "Unexpected end of file.";
        return false;
    }
    if (*buff == "header") {
        if (!eff_ing_header_bodge(samplerate, bpm, beatspermeasure,
                                  beatvalue))
        {
            return false;
        }
    }
    else {
        if (*buff == "samplerate") {
            if (!(*synthfile >> *samplerate)) {
                *wc_err_msg = "Expected value for samplerate.";
                return false;
            }
            if (*samplerate > 4000 && *samplerate < 200000) {
                if (verbose && wc_file_type == WC_MAIN_FILE)
                    cout << "\nsamplerate set at " << *samplerate;
            }
            else {
                *conv << *samplerate;
                *wc_err_msg =
                    "Invalid samplerate: " + conv->str() +
                    ". valid values between 4000 and 200000.";
                return false;
            }
        }
        else {
            *wc_err_msg = "Expected 'header' or 'samplerate' got "
                       + *buff + " instead.";
            return false;
        }
        if (!skip_remarks()) {
            *wc_err_msg = "Unexpected end of file.";
            return false;
        }
        if (*buff == "bpm") {
            if (!(*synthfile >> *bpm)) {
                *wc_err_msg = "Expected value for bpm";
                return false;
            }
            if (*bpm >= 20 && *bpm <= 1000) {
                if (verbose && wc_file_type == WC_MAIN_FILE)
                    cout << "\nbpm set at " << *bpm;
            }
            else {
                *conv << *bpm;
                *wc_err_msg =
                    "Invalid bpm: " + conv->str() +
                    ". valid values between 20 and 1000.";
                return false;
            }
        }
        else {
            *wc_err_msg = "Expected 'bpm' got " + *buff + " instead.";
            return false;
        }
        if (!skip_remarks()) {
            *wc_err_msg = "Unexpected end of file.";
            return false;
        }
        if (*buff == "signature") {
            if (!(*synthfile >> *beatspermeasure)) {
                *wc_err_msg =
                 "Expected value for time signature - beats per measure.";
                return false;
            }
            if (*beatspermeasure < 1 || *beatspermeasure > 16) {
                *conv << *beatspermeasure;
                *wc_err_msg =
                 "Invalid time signature with beats per measure: "
                 + conv->str() + ". valid value in range 1 to 16.";
                return false;
            }
            char ch;
            while (synthfile->get(ch)) {
                if (ch == '/') break;
            }
            if (synthfile->eof()) {
                *wc_err_msg =
                 "Unexpected end of file while scanning time signature.";
                return false;
            }
            if (!(*synthfile >> *beatvalue)) {
                *wc_err_msg =
                    "Expected value for time signature - beat value.";
                return false;
            }
            if (*beatvalue < 1 || *beatvalue > 128) {
                *conv << *beatvalue;
                *wc_err_msg =
                    "Invalid time signature with beat value: " +
                    conv->str() + ". valid value in range 1 to 128.";
                return false;
            }
            if (verbose && wc_file_type == WC_MAIN_FILE) {
                cout << "\ntime signature set to " << *beatspermeasure;
                cout << "/" << *beatvalue;
            }
        }
        else {
            *wc_err_msg = "Expected 'signature'.";
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
        *wc_err_msg = "\nFile not ready!\n";
        return 0;
    }
    if (command) {
        *buff = *command;
        delete command;
        command = 0;
    }
    else if (!skip_remarks()) {
        *wc_err_msg = "\nUnexpected end of file.\n";
        return 0;
    }
    return buff;
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
        double *bpm, short *beatspermeasure, short *beatvalue)
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
        *wc_err_msg = "Requested header refused open: " + hf_name;
        return false;
    }
    if (verbose)
        cout << "\nReading header info from file: " + hf_name;
    headerfile >> *buff;
    if (*buff == "samplerate") {
        if (!(headerfile >> *samplerate)) {
            *wc_err_msg = "In header file: " + hf_name;
            *wc_err_msg += " expected value for samplerate.";
            headerfile.close();
            return false;
        }
        if (*samplerate > 4000 && *samplerate < 200000) {
            if (verbose && wc_file_type == WC_MAIN_FILE)
                cout << "\nsamplerate set at " << *samplerate;
        } else {
            *conv << *samplerate;
            *wc_err_msg = "in header: " + hf_name +
                       " Invalid samplerate: " + conv->str() +
                       ". valid values between 4000 and 200000.";
            headerfile.close();
            return false;
        }
    } else {
        *wc_err_msg = "in header: " + hf_name +
                   " expected 'samplerate' got " + *buff + " instead.";
        headerfile.close();
        return false;
    }
    headerfile >> *buff;
    if (*buff == "bpm") {
        if (!(headerfile >> *bpm)) {
            *wc_err_msg = "in header: " + hf_name 
             + " expected value for bpm";
            headerfile.close();
            return false;
        }
        if (*bpm >= 20 && *bpm <= 1000) {
            if (verbose && wc_file_type == WC_MAIN_FILE)
                cout << "\nbpm set at " << *bpm;
        } else {
            *conv << *bpm;
            *wc_err_msg = "in header: " + hf_name +
                       " Invalid bpm: " + conv->str() +
                       ". valid values between 20 and 1000.";
            headerfile.close();
            return false;
        }
    } else {
        *wc_err_msg = "in header: " + hf_name + " Expected 'bpm' got "
                   + *buff + " instead.";
        headerfile.close();
        return false;
    }
    headerfile >> *buff;
    if (*buff == "signature") {
        if (!(headerfile >> *beatspermeasure)) {
            *wc_err_msg = "in header: " + hf_name
             + " Expected value for time signature - beats per measure.";
            return false;
        }
        if (*beatspermeasure < 1 || *beatspermeasure > 16) {
            *conv << *beatspermeasure;
            *wc_err_msg = "in header: " + hf_name 
             + " Invalid time signature with beats per measure: "
             + conv->str() + ". valid value in range 1 to 16.";
            headerfile.close();
            return false;
        }
        char ch;
        while (headerfile.get(ch)) {
            if (ch == '/')
                break;
        }
        if (headerfile.eof()) {
            *wc_err_msg =
                "Unexpected end of file while scanning time signature.";
            headerfile.close();
            return false;
        }
        if (!(headerfile >> *beatvalue)) {
            *wc_err_msg =
                "Expected value for time signature - beat value.";
            headerfile.close();
            return false;
        }
        if (*beatvalue < 1 || *beatvalue > 128) {
            *conv << *beatvalue;
            *wc_err_msg = "in header: " + hf_name 
             + " Invalid time signature with beat value: "
             + conv->str() + ". valid value in range 1 to 128.";
            headerfile.close();
            return false;
        }
        if (verbose && wc_file_type == WC_MAIN_FILE) {
            cout << "\ntime signature set to " << *beatspermeasure;
            cout << "/" << *beatvalue;
        }
    } else {
        *wc_err_msg = "in header: " + hf_name + " Expected 'signature'.";
        headerfile.close();
        return false;
    }
    filestatus = FILE_READY;
    headerfile.close();  // holy mudcow we made it!
    return true;
}

stockerrs::ERR_TYPE synthfilereader::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

bool synthfilereader::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
    case paramnames::PAR_FILENAME:
        set_wc_filename((char*)data);
        retv = true;
        break;
    case paramnames::PAR_MOD_ACTION:
        set_module_action((ACTION)(*(int*)data));
        retv = true;
        break;
    case paramnames::PAR_DOBJ_ACTION:
        set_dobj_action((ACTION)(*(int*)data));
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* synthfilereader::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_FILENAME:
        retv = wc_filename;
        break;
    case paramnames::PAR_MOD_ACTION:
        retv = &mod_action;
        break;
    case paramnames::PAR_DOBJ_ACTION:
        retv = &dobj_action;
        break;
    default:
        retv = 0;
    }
    return retv;
}

dobj const* synthfilereader::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_DOBJNAME:
        if (!(retv = add_dobjname((dobjnamedobj*)dbj)))
            *err_msg="\ncould not add dobjname to " + *get_username();
        break;
    case dobjnames::SIN_MODNAME:
        if (!(retv = add_modname((modnamedobj*)dbj)))
            *err_msg="\ncould not add modname to " + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
    }
    return retv;
}

stockerrs* synthfilereader::stock_errs = 0;

bool synthfilereader::done_params = false;

void synthfilereader::create_params()
{
    if (done_params == true)
        return;
    get_dparlist()->add_dobjparam(
        dobjnames::DEF_WCFILE, paramnames::PAR_FILENAME);
    get_dparlist()->add_dobjparam(
        dobjnames::DEF_WCFILE, paramnames::PAR_MOD_ACTION);
    get_fxsparamlist()->add_param("include/exclude",
        paramnames::PAR_MOD_ACTION);
    get_dparlist()->add_dobjparam(
        dobjnames::DEF_WCFILE, paramnames::PAR_DOBJ_ACTION);
    get_fxsparamlist()->add_param("include/exclude",
        paramnames::PAR_DOBJ_ACTION);

    dobjdobjlist* dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_WCFILE, dobjnames::LST_MODULES);
    dbjlist->add_dobjdobj(dobjnames::LST_MODULES, dobjnames::SIN_MODNAME);

    dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_WCFILE, dobjnames::LST_DOBJS);
    dbjlist->add_dobjdobj(dobjnames::LST_DOBJS, dobjnames::SIN_DOBJNAME);

    done_params = true;
}

#endif
