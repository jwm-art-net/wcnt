#include "../include/synthfilereader.h"

#include "../include/connectorlist.h"
#include "../include/directivenames.h"
#include "../include/dobjlist.h"
#include "../include/fxsparamlist.h"
#include "../include/globals.h"
#include "../include/listwork.h"
#include "../include/modoutputlist.h"
#include "../include/setparam.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/textstuff.h"
#include "../include/ui_dobjitem.h"
#include "../include/ui_moditem.h"

#include <iostream>
#include <fstream>

using namespace std; // just this once as it's used so much in here...


#define wc_err(fmt, ... ) \
    cfmt(wc_err_msg, STRBUFLEN, fmt, __VA_ARGS__)

bool synthfilereader::samplerate_set(false);

synthfilereader::synthfilereader() :
 dobj::base(dobj::DEF_WCFILE),
 wc_filename(0), mod_action(WC_EXCLUDE), dobj_action(WC_EXCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(WC_INCLUDE_FILE),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), inc_current(false), mod_lineage(false), autogroup(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
    wc_err_msg[0] = '\0';
    set_flags(DO_ALLOW_NAMELESS);
}

synthfilereader::synthfilereader(WC_FILE_TYPE ft) :
 dobj::base(dobj::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(ft),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), inc_current(false), mod_lineage(false), autogroup(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
    wc_err_msg[0] = '\0';
    set_flags(DO_ALLOW_NAMELESS);
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

void synthfilereader::set_wc_filename(const char* filename)
{
    if (wc_filename) {
        delete [] wc_filename;
        wc_filename = 0;
    }
    const char* path = wcnt::jwm.path();
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
        case synthfilereader::FILE_READY:
            break;
        default:
            wc_err("file %s unknown/program error.", wc_filename);
            return false;
    }

    cout << (wc_file_type == WC_INCLUDE_FILE
                ? "  Including "
                : "Processing ") << wc_filename << endl;

    const char* com = read_command();

    // end processing on footer identical to file_id read as header.
    while (strcmp(com, synthheader->c_str()) != 0)
    {
        directive::TYPE dt;
        // checking here starts with quickest test, and progresses in terms
        // of how many items being tested against...
        if (*com == ';')
            print_msg();
        else if (autogroup && strcmp(autogroup->get_username(), com) == 0) {
            if (!autogroup_stop(com))
                return false;
        }
        else if ((dt = directive::names::type(com)) > directive::ERR_TYPE)
        {
            if (!process_directive(dt))
                return false;
        }
        else if (dobj::names::category(com) == dobj::CAT_DEF)
        {
            mod_lineage = false;
            if (!read_and_create_dobj(com))
                return false;
        }
        else {
            mod_lineage = true;
            if (!read_and_create_synthmod(com))
                return false;
        }
        com = read_command();
        if (!com) {
            return false;
        }
    }
    if (wcnt::jwm.is_verbose())
        cout << "end wcnt/jwmsynth: " << wc_filename << endl;
    return true;
}


bool synthfilereader::process_directive(directive::TYPE dt)
{
    switch(dt)
    {
        case directive::AUTOGROUP_ADD:
            return autogroup_add(read_command());
        case directive::AUTOGROUP_CREATE:
            return autogroup_create(read_command());
        //case directive::AUTOGROUP_STOP:
        //    return autogroup_stop(read_command());
        //    break;
        case directive::SAMPLE_RATE:
            return read_samplerate(read_command());
        default:
            break;
    }
    return false;
}


bool synthfilereader::autogroup_add(const char* com)
{
    if (wcnt::jwm.is_verbose()) {
        cout << "-------------------------" << endl;
        cout << "autogroup_add " << com << endl;
        cout << "-------------------------" << endl;
    }

    if (autogroup) {
        wc_err("Cannot use autogroup_add while within autogroup %s.",
               autogroup->get_username());
    }
    // check there is a group already created with this name...
    dobj::base* dbj = wcnt::get_dobjlist()->get_dobj_by_name(com);
    if (!dbj) {
        wc_err("Cannot use autogroup_add, no group named %s exists.", com);
        return false;
    }
    // check named dobj is a group
    if (dbj->get_object_type() != dobj::DEF_GROUP) {
        wc_err("Data object %s specified for autogroup_add is of type %s, "
               "not a group.", com, dobj::names::get(dbj->get_object_type()));
    }

    autogroup = static_cast<group*>(dbj);
    wcnt::jwm.get_modlist()->set_autogroup(autogroup);
    return true;
}

bool synthfilereader::autogroup_create(const char* com)
{
    if (wcnt::jwm.is_verbose()) {
        cout << "-------------------------" << endl;
        cout << "autogroup_create " << com << endl;
        cout << "-------------------------" << endl;
    }

    if (autogroup) {
        wc_err("Cannot create new autogroup %s without stopping current"
               " autogroup %s.", com, autogroup->get_username());
        return false;
    }
    // check new group name is not already in use as a data object
    dobj::base* dbj = wcnt::get_dobjlist()->get_dobj_by_name(com);
    if (dbj) {
        wc_err("Cannot create autogroup, the name %s is already in use "
               "by a %s data object.", com,
               dobj::names::get(dbj->get_object_type()));
        return false;
    }
    // check new group name is not already in use as a module
    synthmod::base* sm = wcnt::jwm.get_modlist()->get_synthmod_by_name(com);
    if (sm) {
        wc_err("Cannot create autogroup, the name %s is already in use "
               "by a %s module.", com,
               synthmod::names::get(sm->get_module_type()));
        return false;
    }

    autogroup = new group;
    autogroup->set_username(com);

    if (!wcnt::get_dobjlist()->add_dobj(autogroup)) {
        wc_err("Could not add data object %s to list.",
               autogroup->get_username());
        return false;
    }

    wcnt::jwm.get_modlist()->set_autogroup(autogroup);

    return true;
}

bool synthfilereader::autogroup_stop(const char* com)
{
    if (wcnt::jwm.is_verbose()) {
        cout << "-------------------------" << endl;
        cout << "autogroup_stop " << com << endl;
        cout << "-------------------------" << endl;
    }

    if (strcmp(com, autogroup->get_username()) != 0) {
        wc_err("Autogroup_stop directive %s mismatches active autogroup %s.",
               com, autogroup->get_username());
        return 0;
    }
    // update group_pending connections
    connectorlist::linkedlist*
            cl = wcnt::get_connectlist()->get_group_pending_connections();

    if (!cl) {
        wc_err("List failure for group %s pending connections.", com);
        return 0;
    }

    synthmod::list* sml = wcnt::jwm.get_modlist();
    char* gn = 0;
    connector* c = cl->goto_first();
    while (c) {
        const char* outmod = c->get_output_module_name();
        D_CONNECT("checking pending connection using output module name: %s "
                  "(to input module %s)\n", outmod, c->get_input_module()->get_username());
        gn = c->get_input_module()->get_group_name();
        if (strcmp(gn, autogroup->get_username()) == 0) {
            // due to autogroup_add, grouping can be staged, so *must* ensure
            // group names match!
            if (!has_groupname(outmod)) {
                char* gsmn = set_groupname(autogroup->get_username(), outmod);
                synthmod::base* sm = sml->get_synthmod_by_name(gsmn);
                if (sm) {
                    // precedence for outputs with matching module name is for
                    // within the autogroup group, and such a module has been
                    // found, so update connector to refer to the grouped module
                    // (it's likely possible the ungrouped name does not exist!)
                    if (wcnt::jwm.is_verbose())
                        cout << "updating pending autogroup connection for output "
                                "module " << outmod << " to " << gsmn << endl;
                    c->set_output_module_name(gsmn);
                    c->clear_group_pending();
                }
                else if ((sm = sml->get_synthmod_by_name(outmod))) {
                    // module isn't within the current autogroup,
                    // BUT, has been found outside of it, so clear group pending.
                    D_CONNECT("Clearing group pending on connection with non-group output module %s\n", outmod);
                    c->clear_group_pending();
                }
                delete [] gsmn;
            }
        }
        delete [] gn;
        c = cl->goto_next();
    }

    delete cl;

    // group data object destruction will be handled by dobjlist,
    // but autogroup is finished for now so set the ptr null.
    autogroup = 0;
    wcnt::jwm.get_modlist()->clear_autogroup();

    return true;
}


void synthfilereader::print_msg()
{
    string ln;
    getline(*synthfile, ln);
    cout << ";" << ln << std::endl;
}

bool synthfilereader::read_and_create_synthmod(const char* com)
{
    synthmod::base* mod = read_synthmodule(com);
    if (!mod) {
        return false;
    }
    if (wcnt::jwm.is_verbose())
        cout << "end " << mod->get_username() << endl;
    if (include_mod(mod->get_username())) {
        if (mod->get_module_type() == synthmod::WCNTEXIT)
        {
            if (wcnt::jwm.get_modlist()->
                    get_first_of_type(synthmod::WCNTEXIT))
            {
                wc_err("Cannot create more than one wcnt_exit module. Not "
                                    "creating %s.", mod->get_username());
                delete mod;
                return 0;
            }
        }
        if (!wcnt::jwm.get_modlist()->add_module(mod)) {
            wc_err("Failed to add module %s to list.", mod->get_username());
            delete mod;
            return false;
        }
    }
    else {
        if (wcnt::jwm.is_verbose()) {
            cout << "***** " << mod->get_username()
                 << " is not being included *****" << endl;
        }
        delete mod;
    }
    return true;
}

bool synthfilereader::read_and_create_dobj(const char* com)
{
    dobj::base* dbj = read_dobj(com);
    if (!dbj)
        return false;
    if (include_dbj(dbj->get_username())) {
        if (!wcnt::get_dobjlist()->add_dobj(dbj)) {
            wc_err("Could not add data object %s to list.",
                                        dbj->get_username());
            return false;
        }
        if (!dbj->do_actions()) {
            if (dbj->get_object_type() == dobj::DEF_WCFILE)
                wc_err("%s", static_cast<synthfilereader*>(dbj)->get_wc_error_msg());
            else
                wc_err("%s", dobj::base::get_error_msg());
            return false;
        }

        if (dbj->get_object_type() == dobj::DEF_WCFILE)
            cout << "    (back in " << wc_filename << ")" << endl;
        if (wcnt::jwm.is_verbose())
            cout << "end " << dbj->get_username() << endl;
    }
    else {
        if (wcnt::jwm.is_verbose()) {
            cout << "***** " << dbj->get_username()
                 << " is not being included *****" << endl;
        }
        delete dbj;
    }

    return true;
}

bool synthfilereader::include_mod(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    // look through the list of named modules to see if name matches.
    // if so, then mod_action will determine if the module is included
    // (ie used) or excluded (ie discarded).
    modnamedobj* modname = modnamelist->goto_first();
    while(modname) {
        if (strcmp(modname->get_modname(), name) == 0)
            return (mod_action == WC_INCLUDE);
        modname = modnamelist->goto_next();
    }
    return (mod_action == WC_INCLUDE) ? false : true;
}

bool synthfilereader::include_dbj(const char* name)
{
    if (wc_file_type == WC_MAIN_FILE)
        return true;
    if (mod_lineage && inc_current)
        return true;
    // look through the list of named data object to see if name matches.
    // if so, then dobj_action will determine if the data object is included
    // (ie used) or excluded (ie discarded).
    dobjnamedobj* dobjname = dobjnamelist->goto_first();
    while(dobjname) {
        if (strcmp(dobjname->get_dobjname(), name) == 0)
            return (dobj_action == WC_INCLUDE);
        dobjname = dobjnamelist->goto_next();
    }
    return (dobj_action == WC_INCLUDE) ? false : true;
}

synthmod::base* synthfilereader::read_synthmodule(const char* com)
{
    synthmod::TYPE smt = synthmod::names::type(com);
    if (smt == synthmod::ERR_TYPE || smt == synthmod::NONEZERO)
    {
        if (wcnt::could_be_header(com)) {
            wc_err("Mismatch between footer %s and header %s.",
                                        com, synthheader->c_str());
            return 0;
        }
        wc_err("Unrecognised wcnt/jwmsynth module %s.", com);
        return 0;
    }

    string modname;
    bool nameless = false;
    *synthfile >> modname;

    if (strcmp(modname.c_str(), "[") == 0) {
        // nameless module handling
        char* n = nameless_name();
        modname = n;
        delete [] n;
        nameless = true;
    }

    if (wcnt::name_is_reserved_word(modname.c_str())) {
        wc_err("Cannot use reserved word %s to name %s module.",
               modname.c_str(), com);
        return 0;
    }
    // prevent the user from defining a module using a grouped name
    const char* const grpname = get_groupname(modname.c_str());
    if (grpname) {
        delete [] grpname;
        wc_err("The %s module name %s uses the '.' character which is "
                "reserved for grouped modules only (use the group data "
                "object if you want to add the module to a group.)",
                synthmod::names::get(smt), modname.c_str());
        return 0;
    }

    // we need to check that the grouped module name is not in use, but not
    // set the module to have that grouped name until later...
    string nongroupname = modname;

    if (autogroup) {
        modname = "." + modname;
        modname = autogroup->get_username() + modname;
    }

    if (include_mod(modname.c_str())) {
        if (wcnt::jwm.get_modlist()->get_synthmod_by_name(modname.c_str()))
        {
            wc_err("A synth module already exists named %s.",
                                                    modname.c_str());
            return 0;
        }
        dobj::base* dbj =
                wcnt::get_dobjlist()->get_dobj_by_name(modname.c_str());
        if (dbj){ // formality because of parameditor.cc workings.
            wc_err("Cannot not name %s as %s, the name is already in use "
                    "by a data object of type %s.", com,
                                                    dbj->get_username(),
                    dobj::names::get(dbj->get_object_type()));
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;

    if (wcnt::jwm.is_verbose()) {
        cout << "================================" << endl;
        cout << "Creating synth module " << modname << endl;
    }

    // use the non grouped name for module creation, because...
    synthmod::base* sm = synthmod::list::create_module(smt, nongroupname.c_str());

    if (nameless && !sm->flag(synthmod::base::SM_ALLOW_NAMELESS)) {
        wc_err("Module of type %s is not allowed to be nameless.",
               modname.c_str(), synthmod::names::get(smt));
        delete sm;
        return 0;
    }

    // read_ui_moditems cannot handle autogrouping...
    // ...because in the file itself the module has the non-grouped name
    if (!read_ui_moditems(sm)) {
        wc_err("In module %s, %s.", sm->get_username(), wc_err_msg);
        delete sm;
        return 0;
    }

    // and pretend auto grouping doesn't exist here also!
    com = read_command();
    if (strcmp(com, (nameless ? "]" : nongroupname.c_str())) != 0) {
        wc_err("In module %s type %s expected definition termination %s, got %s instead.",
               (nameless ? "[nameless]" : sm->get_username()),
               synthmod::names::get(smt), (nameless ? "]" : nongroupname.c_str()), com);
        delete sm;
        return 0;
    }

    // uhuh, so we must create a module to determine if it's groupable or not
    if (autogroup && sm->flag(synthmod::base::SM_UNGROUPABLE)) {
        wc_err("The %s module %s within the autogroup section %s is not "
        "a groupable module.", synthmod::names::get(smt), com,
               autogroup->get_username());
        delete sm;
        return 0;
    }
    // okay now it's safe!
    if (autogroup) {
        synthmod::base* ret = autogroup->autogroup_module(sm);
        // SM_UNGROUPABLE only reason this autogroup_module would fail,
        // and that was already caught earlier, above.
        if (!ret) {
            wc_err("Undefined error autogrouping module %s",
                   sm->get_username());
            return 0;
        }
    }
    return sm;
}

//------------------------------------------------------------------------
// dobj::base* const synthfilereader::read_dobj(string const* command)
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
dobj::base* synthfilereader::read_dobj(const char* com)
{
    dobj::TYPE dobjtype = dobj::names::type(com);

    if (dobj::names::category(dobjtype) != dobj::CAT_DEF)
    {
        wc_err("%s is not a standalone data object.", com);
        return 0;
    }
    string dobjname;
    bool nameless = false;
    *synthfile >> dobjname;

    if (strcmp(dobjname.c_str(), "[") == 0) {
        // nameless data object handling
        char* n = nameless_name();
        dobjname = n;
        delete [] n;
        nameless = true;
    }

    if (wcnt::name_is_reserved_word(dobjname.c_str())) {
        wc_err("Cannot use reserved word %s to name %s data object.",
                            dobjname.c_str(), com);
        return 0;
    }

    const char* const grpname = get_groupname(dobjname.c_str());
    if (grpname) {
        delete [] grpname;
        wc_err("The data object %s name %s used the '.' character which is "
                "strictly reserved for use within groups.",
                dobj::names::get(dobjtype), dobjname.c_str());
        return 0;
    }
    if (include_dbj(dobjname.c_str())) {
        dobj::base* exists = wcnt::get_dobjlist()->get_dobj_by_name(dobjname.c_str());
        if (exists) {
            wc_err("A %s data object already uses the name %s.",
                    dobj::names::get(exists->get_object_type()), dobjname.c_str());
            return 0;
        }
        synthmod::base* sm =
            wcnt::jwm.get_modlist()->get_synthmod_by_name(dobjname.c_str());
        if (sm){ // formality because of parameditor.cc workings.
            wc_err("Cannot name %s %s, the name is already in use by a "
                    "module of type %s.", com, sm->get_username(),
                    synthmod::names::get(sm->get_module_type()));
            return 0;
        }
        inc_current = true;
    }
    else
        inc_current = false;
    if (wcnt::jwm.is_verbose()) {
        cout << "================================" << endl;
        cout << "Creating data object " << dobjname << endl;
    }
    dobj::base* dob = wcnt::get_dobjlist()->create_dobj(dobjtype);
    if (dob == 0) {
        wc_err("Could note create data object of type %s.", com);
        return 0;
    }

    if (nameless && !dob->is_allow_nameless()) {
        wc_err("Data object of type %s is not allowed to be nameless.",
               dobj::names::get(dobjtype));
        delete dob;
        return 0;
    }

    dob->set_username(dobjname.c_str());

    if (autogroup && !dob->is_allow_in_autogroup()) {
        wc_err("Data object %s of type %s is not allowed inside an autogroup.",
                dobjname.c_str(), dobj::names::get(dobjtype));
        delete dob;
        return 0;
    }

    if (!read_ui_dobjitems(dob, (nameless ? "]" : dob->get_username()))) {
        // FIXME: include name of type in error message, and if it is nameless
        // or not
        wc_err("In %s data object %s, %s.",
               dobj::names::get(dob->get_object_type()),
               (nameless ? "[nameless]" : dob->get_username()), wc_err_msg);
        delete dob;
        return 0;
    }

    if (include_dbj(dob->get_username())) {
        if (wcnt::jwm.is_verbose()) cout << "---- validating..." << endl;
        errors::TYPE et = dob->validate();
        if (et != errors::NO_ERROR) {
            wc_err("In %s data object %s, parameter %s %s %s",
                   dobj::names::get(dob->get_object_type()),
                   (nameless ? "[nameless]" : dob->get_username()),
                   dobj::base::get_error_msg(),
                   errors::stock::get_prefix_msg(et),
                   errors::stock::get(et));
            delete dob;
            return 0;
        }
        if (wcnt::jwm.is_verbose()) cout << "Ok." << endl;
    }
    com = read_command();
    if (strcmp(com, (nameless == 0 ? dob->get_username() : "]" )) != 0) {
        wc_err("In data object %s expected definition termination %s, got "
               "%s instead.", dob->get_username(), dob->get_username(), com);
        delete dob;
        return 0;
    }
    return dob;
}

string*  synthfilereader::read_string_list_param (const char* enda,
                                                  const char* endb)
{
    D_UIITEM("%s %s %s %s\n",
             (enda) ? "enda = " : "", (enda) ? enda : "",
             (endb) ? "endb = " : "", (endb) ? endb : "");
    if (enda == 0 && endb == 0) {
        wc_err("%s read_string_list_param(char*, char*) called with"
                            " NULL arguements.", errors::stock::major);
        invalidate();
        return 0;
    }
    string strlist;
    bool ready_to_finish = false;
    while(true) {
        const char* com = read_command();
        if (!com) {
            wc_err("%s", "Unexpected EOF. Data object missing editlist "
                                                            "terminator.");
            invalidate();
            return 0;
        }

        D_UIITEM("got %s\n", com);
        if (enda) {
            if (strcmp(com, enda) == 0) {
                if (!ready_to_finish) {
                    wc_err("Malformed %s in %s.", enda, endb);
                    invalidate();
                    return 0;
                }
                // tell read_command() the next command it should
                // return has already been read and it is enda:
                command = new string(enda);
                D_UIITEM("command set to %s\n returning %s\n", enda, strlist.c_str());
                return new string(strlist);
            }
        }
        if (endb) {
            if (strcmp(com, endb) == 0) {
                if (!ready_to_finish) {
                    wc_err("Unexpected termination of %s.", endb);
                    invalidate();
                    return 0;
                }
                command = new string(endb);
                D_UIITEM("command set to %s\n returning %s\n", endb, strlist.c_str());
                return new string(strlist);
            }
        }
        strlist += com;
        strlist += " ";
        ready_to_finish = true;
    }
    return 0;
}


bool synthfilereader::read_ui_moditems(synthmod::base* sm)
{
    ui::moditem_list* items = sm->get_ui_items();

    if (!items)
        return true;

    if (wcnt::jwm.is_verbose())
        cout << "--------" << endl;

    items->match_begin(sm, sm->get_username());

    ui::moditem* item = 0;
    bool reading = true;

    while (reading) {
        const char* str = read_command();
        D_BUG("()()() read command gave %s ()()()\n", str);
        item = items->match_item(str);

        switch(item->get_item_type()) {
          case ui::UI_ERROR:
            D_BUG("***** ERROR ERROR ERROR *****\n");
            wc_err("%s", item->get_descr());
            return false;
          case ui::UI_USERNAME:
            D_BUG("***** UI_USERNAME\n");
            command = new string(str);
            reading = false;
            break;
          case ui::UI_COMMENT:
            D_BUG("match_item didn't skip comment - wierd\n");
            break;
          case ui::UI_PARAM: {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            #ifdef BAHDEBUG
            mp->dump();
            #endif
            if (!read_ui_modparam(sm, mp->get_param_type()))
                return false;
            break;
          }
          case ui::UI_INPUT: {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            #ifdef BAHDEBUG
            mi->dump();
            #endif
            if (!read_ui_modinput(sm, mi->get_input_type()))
                return false;
            break;
          }
          case ui::UI_DOBJ: {
            ui::moddobj* md = static_cast<ui::moddobj*>(item);
            if (!read_ui_moddobj(sm, md->get_dobj_parent(),
                                     md->get_dobj_child()))
                return false;
            break;
          }
          default:
            wc_err("%s invalid ui element.", errors::stock::bad);
            return false;
        }
    }

    if ((item = items->validate_matches())) {
        wc_err("%s", item->get_descr());
        return false;
    }

    return true;
}


bool synthfilereader::read_ui_dobjitems(dobj::base* dob, const char* parent)
{
    ui::dobjitem_list* items = dob->get_ui_items();

    if (!items)
        return true;

    D_BUG("parent: '%s'\n", (parent ? parent : ""));

    if (wcnt::jwm.is_verbose())
        cout << "--------" << endl;

    items->match_begin(dob, parent);

    const char* dobjname = dobj::names::get(dob->get_object_type());
    ui::dobjitem* item = 0;
    bool reading = true;

    while (reading) {
        const char* str = read_command();
        D_BUG("eeep reading... str: '%s', comparing with parent '%s'\n",
            str, (parent ? parent : ""));
        D_BUG("...comparing with dob: '%s'\n", dobjname);
        if (strcmp(str, dobjname) == 0) {
            command = new string(str);
            break;
        }

        item = items->match_item(str);

        #ifdef DEBUG
        if (!item) {
            wc_err("%s match_item returned null item.", errors::stock::major);
            return false;
        }
        D_BUG("about to switch..\n");
        #endif


        switch (item->get_item_type()) {
          case ui::UI_ERROR:
            D_BUG("***** ERROR ERROR ERROR *****\n");
            wc_err("%s", item->get_descr());
            return false;
          case ui::UI_USERNAME:
            D_BUG("ui::UI_USERNAME!\n");
            command = new string(str);
            reading = false;
            break;
          case ui::UI_COMMENT:
            break;
          case ui::UI_PARAM: {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            param::TYPE pt = dp->get_param_type();
            if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {
                command = new string(str);
                D_BUG("putting string '%s' back...\n", str);
                if (strcmp(str, parent) == 0)
                    return true;
            }
            if (!read_ui_dobjparam(dob, dp->get_param_type(),  parent))
                return false;
            break;
          }
          case ui::UI_DOBJ: {
            ui::dobjdobj* dd = static_cast<ui::dobjdobj*>(item);
            if (!read_ui_dobjdobj(dob, dd->get_dobj_parent(),
                                       dd->get_dobj_child()))
                return false;
            break;
          }
          default:
            wc_err("%s invalid ui element.", errors::stock::bad);
            return false;
        }
    }

    if ((item = items->validate_matches())) {
        wc_err("%s", item->get_descr());
        return false;
    }

    return true;
}

bool
synthfilereader::read_ui_modinput(synthmod::base* sm, input::TYPE inptype)
{
    const char* inpname = input::names::get(inptype);

    string outmod;
    *synthfile >> outmod;

    if (outmod == "off") {
        if (include_mod(sm->get_username())) {
            wcnt::get_connectlist()->add_connector_off(sm, inptype);
            if (wcnt::jwm.is_verbose())
                cout << "added connector " << inpname << "\toff" << endl;
        }
        return true;
    }

    string outname;
    *synthfile >> outname;
    output::TYPE outtype = output::names::type(outname.c_str());

    if (outtype == output::ERR_TYPE) {
        wc_err("Input %s set with unknown output type %s.", inpname,
                                                        outname.c_str());
        return false;
    }

    if (input::names::category(inptype) != output::names::category(outtype))
    {
        wc_err("Input %s set with wrong category of output %s.", inpname,
                                                        outname.c_str());
        return false;
    }

    if (include_mod(sm->get_username())) {
        // FIXME: error detection should be handled by connectorlist
        // - would require connector list to have error message functionality
        connector* c = wcnt::get_connectlist()
                        ->add_connector(sm, inptype, outmod.c_str(), outtype);
        if (autogroup)
            c->set_group_pending();
        if (wcnt::jwm.is_verbose()) {
            cout << "added connector " << inpname << "\t"
                 << outmod << "\t" << outname;
            if (c->is_group_pending())
                cout << "\t(G)";
            cout << endl;

        }
    }
    return true;
}


bool
synthfilereader::read_ui_modparam(synthmod::base* sm, param::TYPE partype)
{
    ostringstream conv;
    string datastr;
    *synthfile >> datastr;

    if (include_mod(sm->get_username())) {
        if (!setpar::set_param(sm, partype, datastr.c_str(), &conv))
        {
            wc_err("%s", setpar::get_error_msg());
            return false;
        }
    }

    if (wcnt::jwm.is_verbose()) {
        cout << "parameter ";
        cout << param::names::get(partype) << "\t" << conv.str() << endl;
    }

   return true;
}


bool
synthfilereader::read_ui_dobjparam(dobj::base* dob, param::TYPE partype,
                                                    const char* parent)
{
    ostringstream conv;
    string* datastr = 0;

    if (partype == param::STR_LIST) {
        D_BUG("*** stringlist parent: '%s'\n", parent);
        const char* dobjname = dobj::names::get(dob->get_object_type());
        datastr = read_string_list_param(dobjname, parent);
        if (!datastr)
            return false;
    }
    else {
        if (command) {
            datastr = command;
            command = 0;
        }
        else {
            datastr = new string;
            *synthfile >> *datastr;
        }
    }
    D_BUG("datastr: '%s'\n", datastr->c_str());
    if (include_dbj(dob->get_username())) {
        if (!setpar::set_param(dob, partype, datastr->c_str(), &conv))
        {
            wc_err("OOOH %s", setpar::get_error_msg());
            delete datastr;
            return false;
        }
    }
    delete datastr;
    if (wcnt::jwm.is_verbose()) {
        cout << "parameter ";
        cout << param::names::get(partype) << "\t" << conv.str() << endl;
    }

   return true;
}

bool
synthfilereader::read_ui_moddobj(synthmod::base* sm, dobj::TYPE parent,
                                                     dobj::TYPE child)
{
    const char* parentname = dobj::names::get(parent);
    const char* childname = dobj::names::get(child);
    const char* com = read_command();
    // now read the list of items (each item's type is sprogtype)
    while (strcmp(parentname, com) != 0) {
        if (strcmp(com, childname) != 0) {
            // check name of item matches expected
            wc_err("data object %s expected %s got %s instead",
                                    parentname, childname, com);
            return false;
        }

        if (wcnt::jwm.is_verbose())
            cout << "-------- creating " << childname << endl;

        dobj::base* dob = wcnt::get_dobjlist()->create_dobj(child);

        if (!dob) {
            wc_err("%s Could not create data object %s data object %s.",
                            errors::stock::major, childname, parentname);
            return false;
        }

        if (!read_ui_dobjitems(dob, parentname)) {
            wc_err("%s, %s %s", parentname, childname, wc_err_msg);
            delete dob;
            return false;
        }

        if (include_mod(sm->get_username())) {
            errors::TYPE et = dob->validate();
            if (et!= errors::NO_ERROR) {
                wc_err("data object %s %s %s %s.", parentname,
                                        dobj::base::get_error_msg(),
                                        errors::stock::get_prefix_msg(et),
                                        errors::stock::get(et));
                delete dob;
                return false;
            }
        }

        if (include_mod(sm->get_username())) {
            // add to synthmodule, not dobjlist  . . .
            if (!sm->add_dobj(dob)) {
                wc_err("%s Could not add data object %s %s in data"
                        " object %s.", errors::stock::major,
                                       childname, sm->get_error_msg(),
                                                         parentname);
                delete dob;
                return false;
            }
        }
        else
            delete dob;

        if (wcnt::jwm.is_verbose())
            cout << "added data object " << childname << endl;

        com = read_command();
    }
    return true;
}


bool
synthfilereader::read_ui_dobjdobj(dobj::base* dob, dobj::TYPE parent,
                                                   dobj::TYPE child)
{
    const char* parentname = dobj::names::get(parent);
    const char* childname = dobj::names::get(child);
    const char* com = read_command();
    // now read the list of items (each item's type is sprogtype)
    while (strcmp(parentname, com) != 0) {
        if (strcmp(com, childname) != 0) {
            // check name of item matches expected
            wc_err("data object %s expected %s got %s instead",
                                    parentname, childname, com);
            return false;
        }

        if (wcnt::jwm.is_verbose())
            cout << "-------- creating " << childname << endl;

        dobj::base* cdob = wcnt::get_dobjlist()->create_dobj(child);

        if (!cdob) {
            wc_err("%s Could not create data object %s data object %s.",
                            errors::stock::major, childname, parentname);
            return false;
        }

        if (!read_ui_dobjitems(cdob, parentname)) {
            wc_err("%s, %s %s", parentname, childname, wc_err_msg);
            delete cdob;
            return false;
        }

        if (include_dbj(dob->get_username())) {
            errors::TYPE et = cdob->validate();
            if (et!= errors::NO_ERROR) {
                wc_err("data object %s %s %s %s.", parentname,
                                        dobj::base::get_error_msg(),
                                        errors::stock::get_prefix_msg(et),
                                        errors::stock::get(et));
                delete cdob;
                return false;
            }
        }

        if (include_dbj(dob->get_username())) {
            // add to synthmodule, not dobjlist  . . .
            if (!dob->add_dobj(cdob)) {
                wc_err("%s Could not add data object %s %s in data"
                        " object %s.", errors::stock::major,
                                       childname,
                                       dobj::base::get_error_msg(),
                                                         parentname);
                delete cdob;
                return false;
            }
        }
        else
            delete cdob;

        if (wcnt::jwm.is_verbose())
            cout << "added data object " << childname << endl;

        com = read_command();
    }

    return true;
}

synthfilereader::FILE_STATUS synthfilereader::open_file()
{
    synthfile->open(wc_filename);
    if (!synthfile->is_open())
        filestatus = NOT_FOUND;
    else {
        *synthfile >> *synthheader;
        if (wcnt::header_is_compatible(synthheader->c_str()))
            filestatus = FILE_READY;
        else
            filestatus = INVALID_HEADER;
    }
    return filestatus;
}

bool synthfilereader::read_samplerate(const char* com)
{
    if (samplerate_set) {
        return true;
    }

    std::istringstream sr_str(com);
    samp_t samplerate;

    if (!(sr_str >> samplerate)) {
        wc_err("%s", "Expected value for samplerate.");
        return false;
    }

    if (!wcnt::jwm.samplerate(samplerate)) {
        wc_err("Invalid samplerate %s. Valid values between %ld and %ld.", com,
                                    wcnt::samplerate_min, wcnt::samplerate_max);
        return false;
    }

    if (wcnt::jwm.is_verbose()) {
        if (wcnt::jwm.samplerate() == samplerate)
            cout << "Samplerate set at " << samplerate << endl;
        else
            cout << "Samplerate already set. Ignoring samplerate " << samplerate << endl;
    }
    return true;
}

const char*
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
    return buff->c_str();
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

errors::TYPE synthfilereader::validate()
{
    return errors::NO_ERROR;
}

bool synthfilereader::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    { // PAR_ADSRSECT is iocat::CAT_FIX_STR.
        case param::FILENAME:
            set_wc_filename((const char*)data);
            return true;
        case param::MOD_ACTION:
            mod_action = (ACTION)(*(int*)data);
            return true;
        case param::DOBJ_ACTION:
            dobj_action = (ACTION)(*(int*)data);
            return true;
        default:
            return false;
    }
}

const void* synthfilereader::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FILENAME:      return wc_filename;
        case param::MOD_ACTION:    return &mod_action;
        case param::DOBJ_ACTION:   return &dobj_action;
        default: return 0;
    }
}

const dobj::base* synthfilereader::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_DOBJNAME: // dobjnamedobj for data object to include/exclued
        retv = dobjnamelist->add_at_tail((dobjnamedobj*)dbj)->get_data();
        if (!retv)
            dobjerr("Could not add dobjname to %s.", get_username());
        break;
    case dobj::SIN_MODNAME: // modnamedobj for module to include/exclued
        retv = modnamelist->add_at_tail((modnamedobj*)dbj)->get_data();
        if (!retv)
            dobjerr("Could not add modname to %s.", get_username());
        break;
    default:
        dobjerr("%s %s to %s.", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    }
    return retv;
}


void synthfilereader::register_ui()
{
    register_param(param::FILENAME);

    register_param(param::MOD_ACTION, "include_listed|exclude_listed")
                            ->set_flags(ui::UI_GROUP1);
    register_dobj(dobj::LST_MODULES, dobj::SIN_MODNAME)
                            ->set_flags(ui::UI_GROUP1);

    register_param(param::DOBJ_ACTION, "include_listed|exclude_listed")
                            ->set_flags(ui::UI_GROUP2);
    register_dobj(dobj::LST_DOBJS, dobj::SIN_DOBJNAME)
                            ->set_flags(ui::UI_GROUP2);
}

ui::dobjitem_list* synthfilereader::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}
