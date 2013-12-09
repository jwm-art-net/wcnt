#include "../include/synthfilereader.h"

#include "../include/connectorlist.h"
#include "../include/dobjlist.h"
#include "../include/fxsparamlist.h"
#include "../include/globals.h"
#include "../include/inputeditor.h"
#include "../include/listwork.h"
#include "../include/modoutputlist.h"
#include "../include/parameditor.h"
#include "../include/setparam.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/textstuff.h"
#include "../include/ui_dobjitem.h"
#include "../include/ui_moditem.h"

#include <iostream>
#include <fstream>

using namespace std; // just this once as it's used so much in here...


#ifdef DEBUG
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
 dobj::base(dobj::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(WC_INCLUDE_FILE),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0), inc_current(false)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
    modnamelist = new  linked_list<modnamedobj>;
    dobjnamelist = new linked_list<dobjnamedobj>;
    wc_err_msg[0] = '\0';
}

synthfilereader::synthfilereader(WC_FILE_TYPE ft) :
 dobj::base(dobj::DEF_WCFILE),
 wc_filename(0), mod_action(WC_INCLUDE), dobj_action(WC_INCLUDE),
 modnamelist(0), dobjnamelist(0),
 wc_file_type(ft),
 filestatus(NOT_FOUND), synthfile(0), buff(0), command(0),
 synthheader(0)
{
    synthfile = new ifstream;
    buff = new string;
    synthheader = new string;
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
        case synthfilereader::FILE_OPEN:
            break;
        case synthfilereader::FILE_READY:
            wc_err("File %s inspires premature optimism.", wc_filename);
            return false;
    }
    samp_t srate;

    cout << (wc_file_type == WC_INCLUDE_FILE
                ? "  Including "
                : "Processing ") << wc_filename << endl;

    if (!read_header(&srate)) {
        return false;
    }
    if (wc_file_type == WC_MAIN_FILE)
        wcnt::jwm.samplerate(srate);
    else {
        if (srate != wcnt::jwm.samplerate()) {
            wc_err("Warning! Header data in file %s conflicts with header "
                    "data already in use.", wc_filename);
        }
    }
    const char* com = read_command();
    while (strcmp(com, wcnt::wcnt_id) != 0)
    {
        // test for string being a data object name - there
        // are fewer data objects than modules...
        if (*com == ';')
            print_msg();
        else if (dobj::names::category(com) == dobj::CAT_DEF)
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
    if (wcnt::jwm.is_verbose())
        cout << "end wcnt/jwmsynth: " << wc_filename << endl;
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
        switch(dbj->get_object_type())
        {
            default:
            // nothing done by default
                break;
            case dobj::DEF_WCFILE:
            {
                synthfilereader* wcf = static_cast<synthfilereader*>(dbj);
                if (!(wcf->read_and_create())) { // ooooh
                    wc_err("%s", wcf->get_wc_error_msg());
                    return false;
                }
                break;
            }
            case dobj::DEF_PARAMEDITOR:
            {
                parameditor* pe = static_cast<parameditor*>(dbj);
                if (!pe->do_param_edits()) {
                    wc_err("%s", dobj::base::get_error_msg());
                    return false;
                }
                break;
            }
            case dobj::DEF_INPUTEDITOR:
            {
                inputeditor* ie = static_cast<inputeditor*>(dbj);
                if (!ie->create_connectors()) {
                    wc_err("%s", dobj::base::get_error_msg());
                    return false;
                }
                break;
            }
        }
    }
    else {
        if (wcnt::jwm.is_verbose()) {
            cout << "***** " << dbj->get_username()
                 << " is not being included *****" << endl;
        }
        delete dbj;
    }
    if (dbj->get_object_type() == dobj::DEF_WCFILE)
        cout << "    (back in " << wc_filename << ")" << endl;
    if (wcnt::jwm.is_verbose())
        cout << "end " << dbj->get_username() << endl;
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

synthmod::base* synthfilereader::read_synthmodule(const char* com)
{
    synthmod::TYPE smt = synthmod::names::type(com);
    if (smt == synthmod::ERR_TYPE || smt == synthmod::NONEZERO)
    {
        wc_err("Unrecognised wcnt/jwmsynth module %s.", com);
        return 0;
    }
    string modname;
    *synthfile >> modname;
    if (strcmp(modname.c_str(), "off") == 0) {
        wc_err("Cannot use reserved word off to name module %s.", com);
        return 0;
    }
    if (strcmp(modname.c_str(), dobj::names::get(dobj::LST_EDITS)) == 0)
    {
        wc_err("Cannot use reserved word %s to name module %s.",
                    dobj::names::get(dobj::LST_EDITS), com);
        return 0;
    }
    const char* const grpname = get_groupname(modname.c_str());
    if (grpname) {
        delete [] grpname;
        wc_err("The %s module name %s uses the '.' character which is "
                "reserved for grouped modules only (use the group data "
                "object if you want to add the module to a group.)",
                synthmod::names::get(smt), modname.c_str());
        return 0;
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
    synthmod::base* sm = synthmod::list::create_module(smt,modname.c_str());

    if (!read_ui_moditems(sm)) {
        wc_err("In module %s, %s.", sm->get_username(), wc_err_msg);
        delete sm;
        return 0;
    }

    com = read_command();
    if (strcmp(com, modname.c_str()) != 0) {
        wc_err("In module %s expected definition termination %s, "
                "got %s instead.", sm->get_username(), sm->get_username(),
                                                                    com);
        delete sm;
        return 0;
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
    *synthfile >> dobjname;
    if (strcmp(dobjname.c_str(), "off") == 0) {
        wc_err("Cannot use reserved word off to name %s.", com);
        return 0;
    }
    if (strcmp(dobjname.c_str(), dobj::names::get(dobj::LST_EDITS)) == 0)
    {
        wc_err("Cannot use reserved word %s to name data object %s.",
                            dobj::names::get(dobj::LST_EDITS), com);
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
        if (wcnt::get_dobjlist()->get_dobj_by_name(dobjname.c_str())) {
            wc_err("A data object %s already uses the name %s.",
                                                com, dobjname.c_str());
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
    dob->set_username(dobjname.c_str());

    if (!read_ui_dobjitems(dob, dob->get_username())) {
        wc_err("In data object %s, %s.", dob->get_username(), wc_err_msg);
        delete dob;
        return 0;
    }

    if (include_dbj(dob->get_username())) {
        if (wcnt::jwm.is_verbose()) cout << "---- validating..." << endl;
        errors::TYPE et = dob->validate();
        if (et != errors::NO_ERROR) {
            wc_err("In data object %s, parameter %s %s %s",
                    dob->get_username(), dobj::base::get_error_msg(),
                    errors::stock::get_prefix_msg(et),
                    errors::stock::get(et));
            delete dob;
            return 0;
        }
        if (wcnt::jwm.is_verbose()) cout << "Ok." << endl;
    }
    com = read_command();
    if (strcmp(com, dobjname.c_str()) != 0) {
        wc_err("In data object %s expected definition termination %s, got "
                                        "%s instead.", dob->get_username(),
                                        dob->get_username(), com);
        delete dob;
        return 0;
    }
    return dob;
}

string*  synthfilereader::read_string_list_param (const char* enda,
                                                  const char* endb)
{
    #ifdef STR_DEBUG
    cout << "read_string_list_param:";
    if (enda) cout << " enda = " << enda;
    if (endb) cout << " endb = " << endb;
    cout << endl;
    #endif
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
        #ifdef STR_DEBUG
        cout << "read_string_list_param got " << com << endl;
        #endif
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
                #ifdef STR_DEBUG
                cout << "command set to " << enda << endl;
                cout << "returning : " << strlist << endl;
                #endif
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
                #ifdef STR_DEBUG
                cout << "command set to " << endb << endl;
                cout << "returning : " << strlist << endl;
                #endif
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

    items->match_begin(sm);

    #ifdef DEBUG
    std::cout << "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=" << std::endl;
    std::cout << "_-_-_-_-_-_-_--- read_ui_moditems ---_-_-_-_-_-_-_" << std::endl;
    std::cout << "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=" << std::endl;
    #endif

    ui::moditem* item = 0;

    while (true) {
        const char* str = read_command();

        if (strcmp(str, sm->get_username()) == 0) {
            command = new string(str);
            break;
        }

        item = items->match_item(str);

        switch(item->get_item_type()) {
          case ui::UI_ERROR:
            #ifdef DEBUG
            std::cout << "***** ERROR ERROR ERROR *****" << std::endl;
            #endif
            wc_err("%s", item->get_descr());
            return false;
          case ui::UI_COMMENT:
            #ifdef DEBUG
            std::cout << "match_item didn't skip comment - wierd" << std::endl;
            #endif
            break;
          case ui::UI_PARAM: {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            #ifdef DEBUG
            mp->dump();
            #endif
            if (!read_ui_modparam(sm, mp->get_param_type()))
                return false;
            break;
          }
          case ui::UI_INPUT: {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            #ifdef DEBUG
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

    #ifdef DEBUG
    std::cout << "===========================================" << std::endl;
    std::cout << "read_ui_dobjitems(dob, parent '" << (parent ? parent : "NULL") << "')" << std::endl;
    std::cout << "===========================================" << std::endl;
    #endif


    if (wcnt::jwm.is_verbose())
        cout << "--------" << endl;

    items->match_begin(dob);

    const char* dobjname = dobj::names::get(dob->get_object_type());
    ui::dobjitem* item = 0;

    while (true) {
        const char* str = read_command();
        #ifdef DEBUG
        std::cout << "reading... str: '" << str << "'" << std::endl;
        std::cout << "comparing with parent: '" << (parent ? parent : "NULL") << "'" << std::endl;
        #endif
        if (parent && strcmp(str, parent) == 0) {
            command = new string(str);
            break;
        }

        #ifdef DEBUG
        std::cout << "comparing with dob: '" << dobjname << "'" << std::endl;
        #endif
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
        std::cout << "about to switch.." << std::endl;
        #endif


        switch (item->get_item_type()) {
          case ui::UI_ERROR:
            #ifdef DEBUG
            std::cout << "***** ERROR ERROR ERROR *****" << std::endl;
            #endif
            wc_err("%s", item->get_descr());
            return false;
          case ui::UI_COMMENT:
            break;
          case ui::UI_PARAM: {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            param::TYPE pt = dp->get_param_type();
            if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {
                command = new string(str);
                #ifdef DEBUG
                std::cout << "putting string '" << str << "' back..." << std::endl;
                #endif
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
        wcnt::get_connectlist()->add_connector(sm, inptype,
                                                   outmod.c_str(),
                                                   outtype);
        if (wcnt::jwm.is_verbose()) {
            cout << "added connector " << inpname << "\t"
                 << outmod << "\t" << outname << endl;
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
    #ifdef DEBUG
    std::cout << "datastr: " << *datastr << std::endl;
    #endif
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

synthfilereader::FILE_STATUS
synthfilereader::open_file()
{
    synthfile->open(wc_filename);
    if (!synthfile->is_open())
        filestatus = NOT_FOUND;
    else {
        *synthfile >> *synthheader;
        if (*synthheader == wcnt::wcnt_id)
            filestatus = FILE_OPEN;
        else
            filestatus = INVALID_HEADER;
    }
    return filestatus;
}

bool synthfilereader::read_header(samp_t *samplerate)
{
    if (filestatus != FILE_OPEN) {
        wc_err("%s Attempted read of header without open file.",
                                                errors::stock::major);
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
                if (wcnt::jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
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

// (snip swearing,  wingeing, and excuse making, for what follows ;)

bool synthfilereader::eff_ing_header_bodge(samp_t *samplerate)
{
    ifstream headerfile;
    string hf_name;
    *synthfile >> hf_name;
    // stop reading from that (synthfile) for mo now we got header name.
    const char* path = wcnt::jwm.path();
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
    if (wcnt::jwm.is_verbose())
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
            if (wcnt::jwm.is_verbose() && wc_file_type == WC_MAIN_FILE)
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
    case dobj::SIN_DOBJNAME:
        retv = dobjnamelist->add_at_tail((dobjnamedobj*)dbj)->get_data();
        if (!retv)
            dobjerr("Could not add dobjname to %s.", get_username());
        break;
    case dobj::SIN_MODNAME:
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

    register_param(param::MOD_ACTION, "include/exclude")
                            ->set_flags(ui::UI_OPTIONAL | ui::UI_GROUP1);
    register_dobj(dobj::LST_MODULES, dobj::SIN_MODNAME)
                            ->set_flags(ui::UI_OPTIONAL | ui::UI_GROUP1);

    register_param(param::DOBJ_ACTION, "include/exclude")
                            ->set_flags(ui::UI_OPTIONAL | ui::UI_GROUP2);
    register_dobj(dobj::LST_DOBJS, dobj::SIN_DOBJNAME)
                            ->set_flags(ui::UI_OPTIONAL | ui::UI_GROUP2);
}

ui::dobjitem_list* synthfilereader::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
}
