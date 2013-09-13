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
#include "../include/textstuff.h"
#include "../include/listwork.h"

#include <iostream>
#include <fstream>

using namespace std; // just this once as it's used so much in here...


#ifdef DEBUG_MSG
#define wc_err(fmt, ... )                               \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(wc_err_msg, STRBUFLEN, fmt, __VA_ARGS__);      \
}
#else
#define wc_err(fmt, ... ) \
    cfmt(wc_err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif

synthfilereader::synthfilereader() :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(WC_INCLUDE_FILE),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    create_params();
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
    wc_err_msg[0] = '\0';
}

synthfilereader::synthfilereader(WC_FILE_TYPE ft) :
 dobj(dobjnames::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(ft),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    create_params();
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
    wc_err_msg[0] = '\0';
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
            wc_err("File %s not found.", wc_filename);
            return false;
        case synthfilereader::INVALID_HEADER:
            wc_err("File %s does not contain a valid header.",
                                                            wc_filename);
            return false;
        case synthfilereader::FILE_OPEN:
            break;
        case synthfilereader::FILE_READY:
            wc_err("File %s inspires premature optimism.", wc_filename);
            return false;
    }
    unsigned long srate;

    cout << (wc_file_type == WC_INCLUDE_FILE
                ? "  Including "
                : "Processing ") << wc_filename << endl;

    if (!read_header(&srate)) {
        return false;
    }
    if (wc_file_type == WC_MAIN_FILE)
        jwm.samplerate(srate);
    else {
        if (srate != jwm.samplerate()) {
            wc_err("Warning! Header data in file %s conflicts with header "
                    "data already in use.", wc_filename);
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
        cout << "end wcnt/jwmsynth: " << wc_filename << endl;
    return true;
}

bool synthfilereader::read_and_create_synthmod(string const* com)
{
    synthmod *mod = read_synthmodule(com);
    if (!mod) {
        return false;
    }
    if (jwm.is_verbose())
        cout << "end " << mod->get_username() << endl;
    if (include_mod(mod->get_username())) {
        if (mod->get_module_type() == synthmodnames::WCNTEXIT)
        {
            if (jwm.get_modlist()->
                    get_first_of_type(synthmodnames::WCNTEXIT))
            {
                wc_err("Cannot create more than one wcnt_exit module. Not "
                                    "creating %s.", mod->get_username());
                delete mod;
                return 0;
            }
        }
        if (!jwm.get_modlist()->add_module(mod)) {
            wc_err("Failed to add module %s to list.", mod->get_username());
            delete mod;
            return false;
        }
    }
    else {
        if (jwm.is_verbose()) {
            cout << "***** " << mod->get_username()
                 << " is not being included *****" << endl;
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
            wc_err("%s", dobj::get_error_msg());
            return false;
        }
    }
    string dbjuname = dbj->get_username();
    if (include_dbj(dbj->get_username())) {
        if (!jwm.get_dobjlist()->add_dobj(dbj)) {
            wc_err("Could not add data object %s to list.",
                                        dbj->get_username());
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
                    wc_err("%s", wcf->get_wc_error_msg());
                    return false;
                }
                break;
            }
            case dobjnames::DEF_PARAMEDITOR:
            {
                parameditor* pe = (parameditor*)dbj;
                if (!pe->do_param_edits()) {
                    wc_err("%s", dobj::get_error_msg());
                    return false;
                }
                break;
            }
            case dobjnames::DEF_INPUTEDITOR:
            {
                inputeditor* ie = (inputeditor*)dbj;
                if (!ie->create_connectors()) {
                    wc_err("%s", dobj::get_error_msg());
                    return false;
                }
                break;
            }
        }
    }
    else {
        if (jwm.is_verbose()) {
            cout << "***** " << dbj->get_username()
                 << " is not being included *****" << endl;
        }
        delete dbj;
    }
    if (dbj->get_object_type() == dobjnames::DEF_WCFILE)
        cout << "    (back in " << wc_filename << ")" << endl;
    if (jwm.is_verbose())
        cout << "end " << dbjuname << endl;
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

synthmod* synthfilereader::read_synthmodule(string const *com)
{
    synthmodnames::SYNTH_MOD_TYPE
                smt = jwm.get_modnames()->get_type(com->c_str());
    if (smt == synthmodnames::FIRST 
        || smt == synthmodnames::NONEZERO)
    {
        wc_err("Unrecognised wcnt/jwmsynth module %s.", com->c_str());
        return 0;
    }
    string modname;
    *synthfile >> modname;
    if (strcmp(modname.c_str(), "off") == 0) {
        wc_err("Cannot use reserved word off to name module %s.",
                                                        com->c_str());
        return 0;
    }
    if (strcmp(modname.c_str(),
        jwm.get_dobjnames()->get_name(dobjnames::LST_EDITS)) == 0)
    {
        wc_err("Cannot use reserved word %s to name module %s.",
                jwm.get_dobjnames()->get_name(dobjnames::LST_EDITS),
                com->c_str());
        return 0;
    }
    const char* const grpname = get_groupname(modname.c_str());
    if (grpname) {
        delete [] grpname;
        wc_err("The %s module name %s uses the '.' character which is "
                "reserved for grouped modules only (use the group data "
                "object if you want to add the module to a group.)",
                jwm.get_modnames()->get_name(smt), modname.c_str());
        return 0;
    }
    if (include_mod(modname.c_str())) {
        if (jwm.get_modlist()->get_synthmod_by_name(modname.c_str()))
        {
            wc_err("A synth module already exists named %s.",
                                                    modname.c_str());
            return 0;
        }
        dobj* dbj = jwm.get_dobjlist()->get_dobj_by_name(modname.c_str());
        if (dbj){ // formality because of parameditor.cc workings.
            wc_err("Cannot not name %s as %s, the name is already in use "
                    "by a data object of type %s.",  com->c_str(),
                                                    dbj->get_username(),
                    jwm.get_dobjnames()->get_name(dbj->get_object_type()));
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (jwm.is_verbose()) {
        cout << "================================" << endl;
        cout << "Creating synth module " << modname << endl;
    }
    synthmod* sm = jwm.get_modlist()->create_module(smt, modname.c_str());
    if (!read_dobjs(sm)){
        wc_err("In module %s, %s.", sm->get_username(), wc_err_msg);
        delete sm;
        return 0;
    }
    if (!read_inputs(sm)){
        wc_err("In module %s, %s.", sm->get_username(), wc_err_msg);
        delete sm;
        return 0;
    }
    if (!read_params(sm)){
        wc_err("In module %s, %s.", sm->get_username(), wc_err_msg);
        delete sm;
        return 0;
    }
    com = read_command();
    if (*com != modname) {
        wc_err("In module %s expected definition termination %s, "
                "got %s instead.", sm->get_username(), sm->get_username(),
                                                            com->c_str());
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
dobj* synthfilereader::read_dobj(string const* com)
{
    const dobjnames* dbjnames = jwm.get_dobjnames();
    dobjnames::DOBJ_TYPE dobjtype = dbjnames->get_type(com->c_str());
    dobjnames::DOBJ_TYPE subtype = dbjnames->get_sub_type(dobjtype);
    if (subtype < dobjnames::DOBJ_DEFS ||
            subtype >= dobjnames::DOBJ_SYNTHMOD)
    {
        wc_err("%s is not a standalone data object.", com->c_str());
        return 0;
    }
    string dobjname;
    *synthfile >> dobjname;
    if (strcmp(dobjname.c_str(), "off") == 0) {
        wc_err("Cannot use reserved word off to name %s.", com->c_str());
        return 0;
    }
    if (strcmp(dobjname.c_str(),
        dbjnames->get_name(dobjnames::LST_EDITS)) == 0)
    {
        wc_err("Cannot use reserved word %s to name data object %s.",
                dbjnames->get_name(dobjnames::LST_EDITS), com->c_str());
        return 0;
    }
    const char* const grpname = get_groupname(dobjname.c_str());
    if (grpname) {
        delete [] grpname;
        wc_err("The data object %s name %s used the '.' character which is "
                "strictly reserved for use within groups.",
                dbjnames->get_name(dobjtype), dobjname.c_str());
        return 0;
    }
    if (include_dbj(dobjname.c_str())) {
        if (jwm.get_dobjlist()->get_dobj_by_name(dobjname.c_str())) {
            wc_err("A data object %s already uses the name %s.",
                    com->c_str(), dobjname.c_str());
            return 0;
        }
        synthmod* sm =
            jwm.get_modlist()->get_synthmod_by_name(dobjname.c_str());
        if (sm){ // formality because of parameditor.cc workings.
            wc_err("Cannot name %s %s, the name is already in use by a "
                    "module of type %s.", com->c_str(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()));
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (jwm.is_verbose()) {
        cout << "================================" << endl;
        cout << "Creating data object " << dobjname << endl;
    }
    dobj* dbj = jwm.get_dobjlist()->create_dobj(dobjtype);
    if (dbj == 0) {
        wc_err("Could note create data object of type %s.", com->c_str());
        return 0;
    }
    dbj->set_username(dobjname.c_str());
    // read dobj parameters (if any)
    if (!read_dobj_params(dbj, 0)){
        wc_err("In data object %s %s.", dbj->get_username(), wc_err_msg);
        delete dbj;
        return 0;
    }
    // read dobj's dobjies (if any)
    if (!read_dobjs(dbj)){
        delete dbj;
        return 0;
    }
    if (include_dbj(dbj->get_username())) {
        if (jwm.is_verbose()) cout << "---- validating..." << endl;
        stockerrs::ERR_TYPE et = dbj->validate();
        if (et != stockerrs::ERR_NO_ERROR) {
            wc_err("In data object %s, parameter %s %s %s", 
                    dbj->get_username(), dobj::get_error_msg(),
                    jwm.get_stockerrs()->get_prefix_err(et),
                    jwm.get_stockerrs()->get_err(et));
            delete dbj;
            return 0;
        }
        if (jwm.is_verbose()) cout << "Ok." << endl;
    }
    com = read_command();
    if (*com != dobjname) {
        wc_err("In data object %s expected definition termination %s, got "
                                        "%s instead.", dbj->get_username(),
                                        dbj->get_username(), com->c_str());
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
            wc_err("In data object %s expected %s got %s instead.",
                    dbj->get_username(), enc_name, enc_com.c_str());
            return false;
        }
        dobjdobjlist* dd_list;
        dd_list = enc_list->get_dobjdobjlist_for_dobjtype(enc_type);
        dobjdobj* dd = dd_list->goto_first();
        if (jwm.is_verbose())
            cout << "-------- for " << enc_name << endl;
        while(dd) {
            string com = *read_command();
            dobjnames::DOBJ_TYPE sprogtype = dd->get_dobj_sprog();
            char const* sprogname = dbjnames->get_name(sprogtype);
            while (com != enc_name) {
                if (com != sprogname) {
                    wc_err("In data object %s expected %s got %s instead.",
                            dbj->get_username(), sprogname, com.c_str());
                    delete dd_list;
                    return false;
                }
                dobj* sprog =
                    jwm.get_dobjlist()->create_dobj(sprogtype);
                if (sprog == 0) {
                    wc_err("*** MAJOR ERROR *** could not create data "
                            "object %s for data object %s.",
                            sprogname, dbj->get_username());
                    delete dd_list;
                    return false;
                }
                if (jwm.is_verbose())
                    cout << "-------- creating " << sprogname << endl;
                if (!read_dobj_params(sprog, enc_com.c_str())) {
                    wc_err("In data object %s %s.", dbj->get_username(),
                                                    wc_err_msg);
                    delete sprog;
                    delete dd_list;
                    return false;
                }
                if (include_dbj(dbj->get_username())) {
                    stockerrs::ERR_TYPE et = sprog->validate();
                    if (et != stockerrs::ERR_NO_ERROR) {
                        wc_err("Data object %s has error in %s %s %s %s.",
                                dbj->get_username(),
                                sprogname,
                                dobj::get_error_msg(),
                                jwm.get_stockerrs()->get_prefix_err(et),
                                jwm.get_stockerrs()->get_err(et));
                        delete sprog;
                        delete dd_list;
                        return false;
                    }
                    // add sprog to dbj, not dobjlist  . . .
                    if (!dbj->add_dobj(sprog)) {
                        wc_err("*** MAJOR ERROR *** Could not add data "
                                "object %s to data object %s.", sprogname,
                                dbj->get_username(),
                                dobj::get_error_msg());
                        delete sprog;
                        delete dd_list;
                        return false;
                    }
                }
                else
                    delete sprog;
                if (jwm.is_verbose())
                    cout << "added " << sprogname << endl;
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
            wc_err("expected %s got %s instead", xdbjname, dbjname.c_str());
            return false;
        }
        if (jwm.is_verbose())
            cout << "-------- for " + dbjname << endl;
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
                    wc_err("data object %s expected %s got %s instead",
                            xdbjname, xsprogname, com.c_str());
                    return false;
                }
                if (jwm.is_verbose())
                    cout << "-------- creating " << xsprogname << endl;
                dobj* dbj = jwm.get_dobjlist()->create_dobj(sprogtype);
                if (dbj == 0) { // failed to create dobj
                    wc_err("*** MAJOR ERROR *** Could not create data "
                            "object %s data object %s.", xsprogname,
                                                        xdbjname);
                    return false;
                }
                if (!read_dobj_params(dbj, 0)) {
                    wc_err("%s, %s %s", dbjname.c_str(), xsprogname,
                                                dobj::get_error_msg());
                    delete dbj;
                    return false;
                }
                if (inc_current) {
                    stockerrs::ERR_TYPE et = dbj->validate();
                    if (et!= stockerrs::ERR_NO_ERROR) {
                        wc_err("data object %s %s %s %s.",
                                xdbjname, dobj::get_error_msg(),
                                jwm.get_stockerrs()->get_prefix_err(et),
                                jwm.get_stockerrs()->get_err(et));
                        delete dbj;
                        return false;
                    }
                }
                if (inc_current) {
                    // add to synthmodule, not dobjlist  . . .
                    if (!sm->add_dobj(dbj)) {
                        wc_err("*** MAJOR ERROR *** Could not add data "
                                "object %s %s in data object %s.",
                                xsprogname, sm->get_error_msg(), xdbjname);
                        delete dbj;
                        return false;
                    }
                }
                else
                    delete dbj;
                if (jwm.is_verbose())
                    cout << "added data object " << xsprogname << endl;
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
                wc_err("%s", setpar::err_msg.c_str());
                delete datastr;
                delete parlist;
                return false;
            }
        }
        delete datastr;
        if (jwm.is_verbose()) {
            cout << "parameter " << parname << "\t" << conv.str() << endl;
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
    cout << "read_string_list_param:";
    if (enda) cout << " enda = " << enda;
    if (endb) cout << " endb = " << endb;
    cout << endl;
    #endif
    if (enda == 0 && endb == 0) {
        wc_err("%s", "*** MAJOR ERROR *** read_string_list_param(char*, "
                                    "char*) called with NULL arguements.");
        invalidate();
        return 0;
    }
    string strlist;
    const string* com;
    bool ready_to_finish = false;
    while(true) {
        if (!(com = read_command())) {
            wc_err("%s", "Unexpected EOF. Data object missing editlist "
                                                            "terminator.");
            invalidate();
            return 0;
        }
        #ifdef DEBUG_STRLIST_PAR
        cout << "read_string_list_param got " << *com << endl;
        #endif
        if (enda) {
            if (strcmp(com->c_str(), enda) == 0) {
                if (!ready_to_finish) {
                    wc_err("Malformed %s in %s.", enda, endb);
                    invalidate();
                    return 0;
                }
                // tell read_command() the next command it should
                // return has already been read and it is enda:
                command = new string(enda);
                #ifdef DEBUG_STRLIST_PAR
                cout << "command set to " << enda << endl;
                cout << "returning : " << strlist << endl;
                #endif
                return new string(strlist);
            }
        }
        if (endb) {
            if (strcmp(com->c_str(), endb) == 0) {
                if (!ready_to_finish) {
                    wc_err("Unexpected termination of %s.", endb);
                    invalidate();
                    return 0;
                }
                command = new string(endb);
                #ifdef DEBUG_STRLIST_PAR
                cout << "command set to " << endb << endl;
                cout << "returning : " << strlist << endl;
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
        cout << "--------" << endl;
    while(inp) { // step through each  input for module
        inputname = *read_command();
        if (innames->get_name(inp->get_inputtype()) != inputname){
            wc_err("expected input type %s got %s instead.",
                    innames->get_name(inp->get_inputtype()),
                                            inputname.c_str());
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
                        cout << "added connector "
                             << inputname << "\toff" << endl;
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
                    wc_err("input %s set with unknown output type %s.",
                                                    inputname.c_str(),
                                                    outputname.c_str());
                    delete inlist;
                    return false;
                }
                if (innames->get_category(input_type)
                    != outnames->get_category(output_type))
                {
                    wc_err("input %s set with wrong category of output %s.",
                                                    inputname.c_str(),
                                                    outputname.c_str());
                    delete inlist;
                    return false;
                }
                if (inc_mod) {
                    connector* connect =
                        new connector(sm, input_type, 
                            outmodname.c_str(), output_type);
                    jwm.get_connectlist()->add_connector(connect);
                    if (jwm.is_verbose()){
                        cout << "added connector " << inputname << "\t"
                             << outmodname << "\t" << outputname << endl;
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
        wc_err("%s", "*** MAJOR ERROR *** BIG problems getting params.");
        return false;
    }
    modparam* param = parlist->goto_first();
    string paramname;
    if (jwm.is_verbose() && param)
        cout << "--------" << endl;
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
                wc_err("%s", setpar::err_msg.c_str());
                delete parlist;
                return false;
            }
        }
        if (jwm.is_verbose()) {
            cout << "parameter ";
            cout << paramname << "\t" << conv.str() << endl;
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
        wc_err("%s", "*** MAJOR ERROR *** Attempted read of header without "
                                                            "open file");
        return 0;
    }
    if (!skip_remarks()) {
        wc_err("%s", "Unexpected EOF.");
        return false;
    }
    if (*buff == "header") {
        if (!eff_ing_header_bodge(samplerate))
            return false;
    }
    else {
        if (*buff == "samplerate") {
            if (!(*synthfile >> *samplerate)) {
                wc_err("%s", "Expected value for samplerate.");
                return false;
            }
            if (*samplerate > 4000 && *samplerate < 200000) {
                if (jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
                    cout << "samplerate set at " << *samplerate << endl;
            }
            else {
                ostringstream conv;
                conv << *samplerate;
                wc_err("Invalid samplerate %s. Valid values between "
                                    "4000 and 200000.", conv.str().c_str());
                return false;
            }
        }
        else {
            wc_err("Expected 'header' or 'samplerate' got %s instead.",
                                                            buff->c_str());
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
        wc_err("%s", "File not ready!");
        return 0;
    }
    if (command) {
        *buff = *command;
        delete command;
        command = 0;
    }
    else if (!skip_remarks()) {
        // in all likelyhood, this msg will get overwritten (nevermind):
        wc_err("%s", "Unexpected EOF");
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
        wc_err("Requested header refused open %s.", hf_name.c_str());
        return false;
    }
    if (jwm.is_verbose())
        cout << "Reading header info from file: " + hf_name << endl;
    headerfile >> *buff;
    if (*buff == "samplerate") {
        if (!(headerfile >> *samplerate)) {
            wc_err("In header file %s expected value for samplerate.",
                                                    hf_name.c_str());
            headerfile.close();
            return false;
        }
        if (*samplerate > 4000 && *samplerate < 200000) {
            if (jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
                cout << "samplerate set at " << *samplerate << endl;
        } else {
            ostringstream conv;
            conv << *samplerate;
            wc_err("In header %d Invalid samplerate %s valid values "
                    "between 4000 and 200000.",  hf_name.c_str(),
                                                conv.str().c_str());
            headerfile.close();
            return false;
        }
    } else {
        wc_err("In header %s expected samplerate got %s instead.",
                                    hf_name.c_str(), buff->c_str());
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
            dobjerr("Could not add dobjname to %s.", get_username());
        break;
    case dobjnames::SIN_MODNAME:
        retv = modnamelist->add_at_tail((modnamedobj*)dbj)->get_data();
        if (!retv)
            dobjerr("Could not add modname to %s.", get_username());
        break;
    default:
        dobjerr("*** MAJOR ERROR *** Bad attempt made to add "
                "invalid object type to %s.", get_username());
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
