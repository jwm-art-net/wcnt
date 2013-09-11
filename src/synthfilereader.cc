#include "../include/synthfilereader.h"
#include "../include/jwm_globals.h"
#include "../include/jwm_init.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/dobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/topdobjlist.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/connectorlist.h"
#include "../include/dobjparamlist.h"
#include "../include/modparamlist.h"
#include "../include/setparam.h"
#include "../include/parameditor.h"
#include "../include/inputeditor.h"
#include "../include/fxsparamlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjdobj.h"

#include "../include/listwork.h"

#include <iostream>
#include <fstream>
using namespace std; // just this once as it's used so much in here...

synthfilereader::synthfilereader() :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(WC_INCLUDE_FILE),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), wc_err_msg(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    wc_err_msg = new string;
    create_params();
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
}

synthfilereader::synthfilereader(WC_FILE_TYPE ft) :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(ft),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), wc_err_msg(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    wc_err_msg = new string;
    create_params();
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
}

synthfilereader::~synthfilereader()
{
    if (wc_filename)
        delete [] wc_filename;
    synthfile->close();
    delete synthfile;
    delete buff;
    if (command)
        delete command;
    delete synthheader;
    delete wc_err_msg;
    delete modnamelist;
    delete dobjnamelist;
}

void synthfilereader::set_wc_filename(char const* filename)
{
    if (wc_filename) {
        delete [] wc_filename;
        wc_filename = 0;
    }
    const char* path = jwm.path();
    // I still have no idea about what is wrong here
    // regarding ccmalloc/valgrind
    if (wc_file_type == WC_MAIN_FILE
        || *filename == '/' || !path)
    {
        wc_filename = new char[strlen(filename) + 1];
        strcpy(wc_filename, filename);
    }
    else {
        char* ptr;
        wc_filename = new char[strlen(filename) + strlen(path) + 1];
        strncpy(wc_filename, path, strlen(path));
        ptr = wc_filename + strlen(path);
        strcpy(ptr, filename);
    }
    return;
}

bool synthfilereader::read_and_create()
{
    if (!wc_filename)
        return false;
    switch (open_file()) {
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
    if (wc_file_type == WC_INCLUDE_FILE) {
        if (jwm.is_verbose()) // pretty please right?
            cout << "\n";
        cout << "\n  Including ";
    }
    else
        cout << "\nProcessing ";
    cout << wc_filename;
    if (!read_header(&srate)) {
        return false;
    }
    if (wc_file_type == WC_MAIN_FILE)
        jwm.samplerate(srate);
    else {
        if (srate != jwm.samplerate()) {
            *wc_err_msg = "\nWarning!\nFile ";
            *wc_err_msg+= wc_filename;
            *wc_err_msg+= " has conflicting header information"
                          " with that already in use.";
        }
    }
    string const *com = read_command();
    while (*com != wcnt_id)
    {
        if (jwm.get_dobjnames()->get_type(com->c_str())
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
        if (!com) {
            return false;
        }
    }
    if (jwm.is_verbose())
        cout << "\n\nend wcnt/jwmsynth: " << wc_filename;
    return true;
}

bool synthfilereader::read_and_create_synthmod(string const* com)
{
    synthmod *mod = read_synthmodule(com);
    if (!mod) {
        return false;
    }
    if (jwm.is_verbose())
        cout << "\nend " << mod->get_username();
    if (include_mod(mod->get_username())) {
        if (mod->get_module_type() == synthmodnames::WCNTEXIT)
        {
            if (jwm.get_modlist()->
                    get_first_of_type(synthmodnames::WCNTEXIT))
            {
                *wc_err_msg
                   = "\nCannot create more than one wcnt_exit module: ";
                *wc_err_msg += mod->get_username();
                delete mod;
                return 0;
            }
        }
        if (!jwm.get_modlist()->add_module(mod)) {
            *wc_err_msg = "\ncould not add module ";
            *wc_err_msg += mod->get_username();
            *wc_err_msg += " to list.";
            delete mod;
            return false;
        }
    }
    else {
        if (jwm.is_verbose()) {
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
    if (include_dbj(dbj->get_username())){
        if (!dbj->validate()) {
        *wc_err_msg = *dbj->get_error_msg();
        return false;
        }
    }
    string dbjuname = dbj->get_username();
    if (include_dbj(dbj->get_username())) {
        if (!jwm.get_dobjlist()->add_dobj(dbj)) {
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
                if (!(wcf->read_and_create())) { // ooooh
                    *wc_err_msg = wcf->get_wc_error_msg();
                    return false;
                }
                break;
            }
            case dobjnames::DEF_PARAMEDITOR:
            {
                parameditor* pe = (parameditor*)dbj;
                if (!pe->do_param_edits()) {
                    *wc_err_msg = *dobj::get_error_msg();
                    return false;
                }
                break;
            }
            case dobjnames::DEF_INPUTEDITOR:
            {
                inputeditor* ie = (inputeditor*)dbj;
                if (!ie->create_connectors()) {
                    *wc_err_msg = *dobj::get_error_msg();
                    return false;
                }
                break;
            }
        }
    }
    else {
        if (jwm.is_verbose()) {
            cout << "\n\n***** " << dbj->get_username();
            cout << " is not being included *****";
        }
        delete dbj;
    }
    if (dbj->get_object_type() == dobjnames::DEF_WCFILE)
        cout << "\n    (back in " << wc_filename << ")";
    if (jwm.is_verbose())
        cout << "\nend " << dbjuname;
    return true;
}

bool synthfilereader::include_mod(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    modnamedobj* modname = modnamelist->goto_first();
    while(modname) {
        if (strcmp(modname->get_modname(), name) == 0)
            return (mod_action == WC_INCLUDE) ? true : false;
        modname = modnamelist->goto_next();
    }
    return (mod_action == WC_INCLUDE) ? false : true;
}

bool synthfilereader::include_dbj(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    dobjnamedobj* dobjname = dobjnamelist->goto_first();
    while(dobjname) {
        if (strcmp(dobjname->get_dobjname(), name) == 0)
            return (dobj_action == WC_INCLUDE) ? true : false;
        dobjname = dobjnamelist->goto_next();
    }
    return (dobj_action == WC_INCLUDE) ? false : true;
}

synthmod *const synthfilereader::read_synthmodule(string const *com)
{
    synthmodnames::SYNTH_MOD_TYPE
                smt = jwm.get_modnames()->get_type(com->c_str());
    if (smt == synthmodnames::FIRST 
        || smt == synthmodnames::NONEZERO)
    {
        *wc_err_msg = "\nUnrecognised wcnt/jwmsynth module: " + *com;
        return 0;
    }
    string modname;
    *synthfile >> modname;
    if (strcmp(modname.c_str(), "off") == 0) {
        *wc_err_msg = "\ncannot use reserved word off to name module ";
        *wc_err_msg += *com;
        return 0;
    }
    if (strcmp(modname.c_str(),
        jwm.get_dobjnames()->get_name(dobjnames::LST_EDITS)) == 0)
    {
        *wc_err_msg = "\ncannot use reserved word ";
        *wc_err_msg +=
            jwm.get_dobjnames()->get_name(dobjnames::LST_EDITS);
        *wc_err_msg += " to name module ";
        *wc_err_msg += *com;
        return 0;
    }
    const char* const grpname = get_groupname(modname.c_str());
    if (grpname) {
        delete [] grpname;
        *wc_err_msg = "\nthe ";
        *wc_err_msg += jwm.get_modnames()->get_name(smt);
        *wc_err_msg += " name ";
        *wc_err_msg += modname;
        *wc_err_msg += " uses the . character which is reserved for ";
        *wc_err_msg += "grouped modules only. use the group data object";
        *wc_err_msg += " to add a module to a group.";
        return 0;
    }
    if (include_mod(modname.c_str())) {
        if (jwm.get_modlist()->get_synthmod_by_name(modname.c_str()))
        {
            *wc_err_msg = "\nsynth module already exists named ";
            *wc_err_msg += modname;
            return 0;
        }
        dobj* dbj = jwm.get_dobjlist()->get_dobj_by_name(modname.c_str());
        if (dbj){ // formality because of parameditor.cc workings.
            *wc_err_msg = "\nwill not name ";
            *wc_err_msg += *com;
            *wc_err_msg += " ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += 
             ", the name has already been taken by data object of type ";
            *wc_err_msg +=
             jwm.get_dobjnames()->get_name(dbj->get_object_type());
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (jwm.is_verbose()) {
        cout << "\n\n================================";
        cout << "\nCreating synth module " << modname;
    }
    synthmod* sm = jwm.get_modlist()->create_module(smt, modname.c_str());
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
    const dobjnames* dbjnames = jwm.get_dobjnames();
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
    if (strcmp(dobjname.c_str(),
        dbjnames->get_name(dobjnames::LST_EDITS)) == 0)
    {
        *wc_err_msg = "\ncannot use reserved word ";
        *wc_err_msg += dbjnames->get_name(dobjnames::LST_EDITS);
        *wc_err_msg += " to name data object ";
        *wc_err_msg += *com;
        return 0;
    }
    const char* const grpname = get_groupname(dobjname.c_str());
    if (grpname) {
        delete [] grpname;
        *wc_err_msg = "\nthe ";
        *wc_err_msg += dbjnames->get_name(dobjtype);
        *wc_err_msg += " name ";
        *wc_err_msg += dobjname;
        *wc_err_msg += " uses the . character which is reserved for";
        *wc_err_msg += "grouped modules only.";
        return 0;
    }
    if (include_dbj(dobjname.c_str())) {
        if (jwm.get_dobjlist()->get_dobj_by_name(dobjname.c_str())) {
            *wc_err_msg = "\ndata object " + *com
                      + " already exists named "+dobjname;
            return 0;
        }
        synthmod* sm =
            jwm.get_modlist()->get_synthmod_by_name(dobjname.c_str());
        if (sm){ // formality because of parameditor.cc workings.
            *wc_err_msg = "\nwill not name ";
            *wc_err_msg += *com;
            *wc_err_msg += " ";
            *wc_err_msg += sm->get_username();
            *wc_err_msg += 
                ", the name has already been taken by module of type ";
            *wc_err_msg +=
                jwm.get_modnames()->get_name(sm->get_module_type());
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (jwm.is_verbose()) {
        cout << "\n\n================================";
        cout << "\nCreating data object " << dobjname;
    }
    dobj* dbj = jwm.get_dobjlist()->create_dobj(dobjtype);
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
        if (jwm.is_verbose()) cout << "\n----\nvalidating...";
        stockerrs::ERR_TYPE et = dbj->validate();
        if (et != stockerrs::ERR_NO_ERROR) {
            *wc_err_msg = "\nIn data object ";
            *wc_err_msg += dbj->get_username();
            *wc_err_msg += ", parameter ";
            *wc_err_msg += *dbj->get_error_msg();
            *wc_err_msg += " ";
            *wc_err_msg += jwm.get_stockerrs()->get_prefix_err(et);
            *wc_err_msg += jwm.get_stockerrs()->get_err(et);
            delete dbj;
            return 0;
        }
        if (jwm.is_verbose()) cout << "Ok.";
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
    // get first encapsulating list type
    dobjdobjlist* enc_list = jwm.get_topdobjlist()->get_first_of_type(dt);
    if (!enc_list) // not dobjs in this dbj dobj
        return true;
    const dobjnames* dbjnames = jwm.get_dobjnames();
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
        if (jwm.is_verbose())
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
                    jwm.get_dobjlist()->create_dobj(sprogtype);
                if (sprog == 0) {
                    *wc_err_msg = "\n***major error***";
                    *wc_err_msg += "\ncould not create data object ";
                    *wc_err_msg += sprogname;
                    *wc_err_msg += " for data object ";
                    *wc_err_msg += dbj->get_username();
                    delete dd_list;
                    return false;
                }
                if (jwm.is_verbose())
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
                        *wc_err_msg +=
                            jwm.get_stockerrs()->get_prefix_err(et);
                        *wc_err_msg += jwm.get_stockerrs()->get_err(et);
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
                        *wc_err_msg += *dbj->get_error_msg();
                        delete sprog;
                        delete dd_list;
                        return false;
                    }
                }
                else
                    delete sprog;
                if (jwm.is_verbose())
                    cout << "\nadded " << sprogname;
                com = *read_command();
            }
            dd = dd_list->goto_next();
        }
        delete dd_list;
        enc_list = jwm.get_topdobjlist()->get_next_of_type();
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
    moddobjlist* mdbjslist = jwm.get_moddobjlist();
    moddobj* mdbj = mdbjslist->get_first_of_type(smt);
    const dobjnames* dbjnames = jwm.get_dobjnames();
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
        if (jwm.is_verbose())
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
                if (jwm.is_verbose())
                    cout << "\n--------\ncreating " << xsprogname;
                dobj* dbj = jwm.get_dobjlist()->create_dobj(sprogtype);
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
                if (inc_current) {
                    stockerrs::ERR_TYPE et = dbj->validate();
                    if (et!= stockerrs::ERR_NO_ERROR) {
                        *wc_err_msg = ", data object ";
                        *wc_err_msg += xdbjname;
                        *wc_err_msg += ", ";
                        *wc_err_msg += *dbj->get_error_msg();
                        *wc_err_msg += " ";
                        *wc_err_msg +=
                            jwm.get_stockerrs()->get_prefix_err(et);
                        *wc_err_msg += jwm.get_stockerrs()->get_err(et);
                        delete dbj;
                        return false;
                    }
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
                if (jwm.is_verbose())
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
// to add to the string list paramnames::TYPE PAR_STR_LIST.

bool synthfilereader::
read_dobj_params(dobj* dbj, const char* endterm)
{
    dobjnames::DOBJ_TYPE dobjtype = dbj->get_object_type();
    dobjparamlist::linkedlist*
        parlist = new_list_of_by(jwm.get_dparlist(), dobjtype);

    dobjparam* param = parlist->goto_first();
    char const* enda = jwm.get_dobjnames()->get_name(dobjtype);
    while(param) {
        ostringstream conv;
        paramnames::PAR_TYPE pt = param->get_partype();
        string parname;
        if (pt != paramnames::STR_UNNAMED
            && pt != paramnames::STR_LIST) 
        {
            parname = *read_command();
        }
        string* datastr;
        if (pt == paramnames::STR_LIST) {
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
            if (!setpar::set_param(dbj, par, pt, val, &conv)) {
                *wc_err_msg = setpar::err_msg;
                delete datastr;
                delete parlist;
                return false;
            }
        }
        delete datastr;
        if (jwm.is_verbose()) {
            cout << "\nparameter ";
            cout << parname << "\t" << conv.str();
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
    #ifdef DEBUG_STRLIST_PAR
    cout << "\nread_string_list_param:";
    if (enda) cout << " enda = " << enda;
    if (endb) cout << " endb = " << endb;
    #endif
    if (enda == 0 && endb == 0) {
        *wc_err_msg = "\nread_string_list_param(char*, char*)";
        *wc_err_msg += " called with NULL arguements. error in";
        *wc_err_msg += " code implimentation - somewhere!";
        invalidate();
        return 0;
    }
    string strlist;
    const string* com;
    bool ready_to_finish = false;
    while(true) {
        if (!(com = read_command())) {
            *wc_err_msg = "\nUnexpected end of file...\nThe data object "
                "is missing its editlist terminator...";
            invalidate();
            return 0;
        }
        #ifdef DEBUG_STRLIST_PAR
        cout << "\nread_string_list_param got " << *com;
        #endif
        if (enda) {
            if (strcmp(com->c_str(), enda) == 0) {
                if (!ready_to_finish) {
                    *wc_err_msg = "\nmalformed ";
                    *wc_err_msg += enda;
                    *wc_err_msg += " in ";
                    *wc_err_msg += endb;
                    invalidate();
                    return 0;
                }
                // tell read_command() the next command it should
                // return has already been read and it is enda:
                command = new string(enda);
                #ifdef DEBUG_STRLIST_PAR
                cout << "\ncommand set to " << enda;
                cout << "\nreturning : " << strlist;
                #endif
                return new string(strlist);
            }
        }
        if (endb) {
            if (strcmp(com->c_str(), endb) == 0) {
                if (!ready_to_finish) {
                    *wc_err_msg = "\nunexpected termination of ";
                    *wc_err_msg += endb;
                    invalidate();
                    return 0;
                }
                command = new string(endb);
                #ifdef DEBUG_STRLIST_PAR
                cout << "\ncommand set to " << endb;
                cout << "\nreturning : " << strlist;
                #endif
                return new string(strlist);
            }
        }
        strlist += *com;
        strlist += " ";
        ready_to_finish = true;
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
    modinputlist::linkedlist* inlist =
        new_list_of_by(jwm.get_inputlist(), sm);

    modinput* inp = inlist->goto_first();
    const inputnames* innames = jwm.get_inputnames();
    const outputnames* outnames = jwm.get_outputnames();
    string inputname;
    if (jwm.is_verbose() && inp)
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
                = innames->get_type(inputname.c_str());
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
                    jwm.get_connectlist()->add_connector(connect);
                    if (jwm.is_verbose()) {
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
                    jwm.get_connectlist()->add_connector(connect);
                    if (jwm.is_verbose()){
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
    modparamlist::linkedlist* parlist =
        new_list_of_by(jwm.get_paramlist(), sm->get_module_type());

//        jwm.get_paramlist()->get_paramlist_for_moduletype(
//            sm->get_module_type());

    if (!parlist) {
        *wc_err_msg = "\n*** Big problems getting params ***";
        return false;
    }
    modparam* param = parlist->goto_first();
    string paramname;
    if (jwm.is_verbose() && param)
        cout << "\n--------";
    while(param) {
        paramname = *read_command();
        paramnames::PAR_TYPE pt = param->get_paramtype();
        ostringstream conv;
        string datastr;
        *synthfile >> datastr;
        if (inc_current) {
            char const* par = paramname.c_str();
            char const* val = datastr.c_str();
            if (!setpar::set_param(sm, par, pt, val, &conv)) {
                *wc_err_msg = setpar::err_msg;
                delete parlist;
                return false;
            }
        }
        if (jwm.is_verbose()) {
            cout << "\nparameter ";
            cout << paramname << "\t" << conv.str();
        }
        param = parlist->goto_next();
    }
    delete parlist;
    return true;
}

synthfilereader::FILE_STATUS
synthfilereader::open_file()
{
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

bool synthfilereader::read_header(unsigned long *samplerate)
{
    if (filestatus != FILE_OPEN) {
        *wc_err_msg = "\nProgrammer Error! Attempted read of header";
        *wc_err_msg += " when file not open.";
        return 0;
    }
    if (!skip_remarks()) {
        *wc_err_msg = "Unexpected end of file.";
        return false;
    }
    if (*buff == "header") {
        if (!eff_ing_header_bodge(samplerate))
            return false;
    }
    else {
        if (*buff == "samplerate") {
            if (!(*synthfile >> *samplerate)) {
                *wc_err_msg = "Expected value for samplerate.";
                return false;
            }
            if (*samplerate > 4000 && *samplerate < 200000) {
                if (jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
                    cout << "\nsamplerate set at " << *samplerate;
            }
            else {
                ostringstream conv;
                conv << *samplerate;
                *wc_err_msg =
                    "Invalid samplerate: " + conv.str() +
                    ". valid values between 4000 and 200000.";
                return false;
            }
        }
        else {
            *wc_err_msg = "Expected 'header' or 'samplerate' got "
                       + *buff + " instead.";
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
        // in all likelyhood, this msg will get overwritten (nevermind):
        *wc_err_msg = "\nUnexpected end of file.\n";
        return 0;
    }
    return buff;
}


bool synthfilereader::skip_remarks()
{
    *synthfile >> *buff;
    if (synthfile->eof())
        return false;
    while ((*buff)[0] == '/') {
        if ((*buff)[1] == '/') {
            char ch;
            while (synthfile->get(ch)) {
                if (ch == '\n')
                    break;
            }
        }
        else return true; // single / character == not comment.
        *synthfile >> *buff;
        if (synthfile->eof())
            return false; // catches EOF immediately AFTER a comment
    }
    if (synthfile->eof())
        return false;
    return true;          // i'm happy - honest
}

// (snip swearing,  wingeing, and excuse making, for what follows ;)

bool synthfilereader::eff_ing_header_bodge(unsigned long *samplerate)
{
    ifstream headerfile;
    string hf_name;
    *synthfile >> hf_name;
    // stop reading from that (synthfile) for mo now we got header name.
    char const* path = jwm.path();
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
    if (jwm.is_verbose())
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
            if (jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
                cout << "\nsamplerate set at " << *samplerate;
        } else {
            ostringstream conv;
            conv << *samplerate;
            *wc_err_msg = "in header: " + hf_name +
                       " Invalid samplerate: " + conv.str() +
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
    filestatus = FILE_READY;
    headerfile.close();
    return true;
}

stockerrs::ERR_TYPE synthfilereader::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

bool synthfilereader::set_param(paramnames::PAR_TYPE pt, void* data)
{
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
        case paramnames::FILENAME:
            set_wc_filename((char*)data);
            return true;
        case paramnames::MOD_ACTION:
            mod_action = (ACTION)(*(int*)data);
            return true;
        case paramnames::DOBJ_ACTION:
            dobj_action = (ACTION)(*(int*)data);
            return true;
        default:
            return false;
    }
}

void const* synthfilereader::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FILENAME:      return wc_filename;
        case paramnames::MOD_ACTION:    return &mod_action;
        case paramnames::DOBJ_ACTION:   return &dobj_action;
        default: return 0;
    }
}

dobj const* synthfilereader::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_DOBJNAME:
        retv = dobjnamelist->add_at_tail((dobjnamedobj*)dbj)->get_data();
        if (!retv)
            *err_msg="\ncould not add dobjname to " + *get_username();
        break;
    case dobjnames::SIN_MODNAME:
        retv = modnamelist->add_at_tail((modnamedobj*)dbj)->get_data();
        if (!retv)
            *err_msg="\ncould not add modname to " + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
    }
    return retv;
}

bool synthfilereader::done_params = false;

void synthfilereader::create_params()
{
    if (done_params == true)
        return;
    dobjparamlist* dl = jwm.get_dparlist();
    dl->add_dobjparam(dobjnames::DEF_WCFILE, paramnames::FILENAME);
    dl->add_dobjparam(dobjnames::DEF_WCFILE, paramnames::MOD_ACTION);
    jwm.get_fxsparamlist()->add_param("include/exclude",
                                      paramnames::MOD_ACTION);
    dl->add_dobjparam(dobjnames::DEF_WCFILE, paramnames::DOBJ_ACTION);
    jwm.get_fxsparamlist()->add_param("include/exclude",
                                      paramnames::DOBJ_ACTION);
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_WCFILE, dobjnames::LST_MODULES)->
            add_dobjdobj(dobjnames::LST_MODULES,
                dobjnames::SIN_MODNAME);
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_WCFILE, dobjnames::LST_DOBJS)->
            add_dobjdobj(dobjnames::LST_DOBJS,
                dobjnames::SIN_DOBJNAME);
    done_params = true;
}
