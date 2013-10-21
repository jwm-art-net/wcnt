#include "../include/cmdline.h"
#include "../include/globals.h"
#include "../include/conversions.h"
#include "../include/wavfilein.h"
#include "../include/synthmod.h"

#include "../include/synthmodlist.h"
#include "../include/modoutputlist.h"
#include "../include/fxsparamlist.h"

#include "../include/dobjlist.h"

#include "../include/synth.h"

#include "../include/textstuff.h"
#include "../include/ui_dobjitem.h"
#include "../include/ui_moditem.h"

#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iostream>


cmdline::cmdline(int const argc, const char** const argv) :
 opts_count(argc), opts(argv), opts_flags(0), good_opts(false)
{
}

cmdline::~cmdline()
{
}

cmdline::cmd_opts_data cmdline::data[OPTS_COUNT] =
{
{ WC_FILE,
    0,                      0,      "<filename.wc>",                0,0,2,
    "\tGenerate and run jwmsynth specified in filename.wc.",
    0
},
{ VERBOSE,
    "--verbose",            "-v",   "<filename.wc>",                0,0,6,
    "\tDisplay lots of information as modules and data objects are\n"
    "\tcreated.",
    0
},
{ DONT_RUN,
    "--dont-run",           0,      "<filename.wc>",                0,0,5,
    "\tCreate synth from filename.wc but do not run it. For testing...",
    0
},
{ NO_TITLE,
    "--no-title",           "-nt",  "<filename.wc>",                0,0,6,
    "\tDon't display titles or flush complete message.",
    0
},
{ NO_PROGRESS,
    "--no-progress",        "-np",  "<filename.wc>",                0,0,6,
    "\tDon't display progress ticker.",
    0
},
{ MOD_HELP,
    "--module-help",        "-mh",  "[module_type]",                0,0,4,
    "\tDisplay a list of all wcnt synth modules, or display a specific\n"
    "\tmodule definition.",
    &cmdline::module_help
},
{ IN_HELP,
    "--input-help",         "-ih",  "[input_type] [filename.wc]",   0,0,5,
    "\tDisplay a list of all input types, or display a list of outputs\n"
    "\tcompatible with specified input. The list of compatible outputs\n"
    "\tcan be restricted to only list the outputs of modules in\n"
    "\tfilename.wc.",
    &cmdline::input_help
},
{ DOBJ_HELP,
    "--data-object-help",   "-dh",  "[data_object_type]",           0,0,4,
    "\tDisplay list of data object types or display a data object's\n"
    "\tdefinition.",
    &cmdline::dobj_help
},
{ SAMPLE_INFO,
    "--sample-info",        "-si",  "<sample.wav>",                 0,0,4,
    "\tDisplay information about a WAV file audio sample. Such\n"
    "\tinformation is useful for setting sampler parameters.",
    &cmdline::sample_info
},
{ NOTE_INFO,
    "--note-info",          "-ni",  "<note_name>",                  0,0,4,
    "\tTranslate a note name (ie c#-1, e2) to a frequency.",
    &cmdline::note_info
},
{ FREQ_INFO,
    "--freq-info",          "-fi",  "<frequency> <samplerate>",     0,0,5,
    "\tTranslate a frequency to degrees-per-sample for given sample\n"
    "\trate. Useful for constant frequency waveform generation etc.",
    &cmdline::freq_info
},
{ HEADER,
    "--header",             0,      0,                              0,0,2,
    "\tShow the wcnt header line which must be at the start and end of\n"
    "\twcnt .wc files.",
    &cmdline::header
},
{ HELP,
    "--help",               "-h",   0,                              0,0,2,
    "\tLists all of wcnt's command line options.",
    &cmdline::help
},
{ LONGHELP,
    "--longhelp",           "-lh",  0,                              0,0,2,
    "\tDisplay help and descriptions for all command line options.",
    &cmdline::help
},
{ ABOUT,
    "--about",              0,      0,                              0,0,2,
    "\tBrief information about wcnt and it's author.",
    &cmdline::about
}
};

bool cmdline::scan()
{
#ifdef DEBUG
    std::cout << "\ncmdline::scan()";
    std::cout << "\ncommandline arguements count: " << opts_count;
    for(int i = 0; i < opts_count; i++){
        std::cout << "\n" << i << " = '" << opts[i] << "'";
    }
#endif
    if (opts_count == 1) {
        help();
        return false;
    }
    bool extraneous = false;
    for (int arg = 1; arg < opts_count; arg++){
#ifdef DEBUG
        std::cout << "\nprocessing arg: " << opts[arg];
#endif
        int o;
        for (o = 1; o < OPTS_COUNT; o++){
#ifdef DEBUG
            std::cout << "\n    comparing with: " << data[o].olong;
#endif
            if (((data[o].olong) ?
                    strcmp(opts[arg], data[o].olong) == 0  : false)
                    ||
                ((data[o].oshort) ?
                    strcmp(opts[arg], data[o].oshort) == 0 : false))
            {
#ifdef DEBUG
                std::cout << "\n    adding: " << data[o].type;
#endif
                opts_flags += data[o].type;
#ifdef DEBUG
                std::cout << " to flags ( now: " << opts_flags << " )";
#endif
                switch(data[o].type){
                    case VERBOSE:
                    case DONT_RUN:
                    case NO_TITLE:
                    case NO_PROGRESS:
                    case HEADER:
                    case HELP:
                    case LONGHELP:
                        data[o].par1 = 1;
#ifdef DEBUG
                        std::cout << "\n    arg ";
                        std::cout << data[o].olong;
                        std::cout << " set to 1 : " << data[o].par1;
#endif
                        o = 100;
                        break;
                    default:
                        if (data[o].type == 0)std::cout << "!$*%&(@!";
                        data[o].par1 = (arg + 1 < opts_count)? ++arg : 0;
                        data[o].par2 = (arg + 1 < opts_count)? arg+1 : 0;
#ifdef DEBUG
                        std::cout << "\n    arg may require two params: ";
                        std::cout << data[o].par1 << " and "
                            << data[o].par2;
#endif
                        o = 100;
                }
            }
        }
            if(o == OPTS_COUNT)
            {
#ifdef DEBUG
                std::cout << "\n    comparison failed.";
#endif
                if(data[WC_IX].par1 == 0) {
#ifdef DEBUG
                    std::cout << "\n    using '" << opts[arg] << "'";
                    std::cout << " as .wc file." << std::endl;
#endif
                    data[WC_IX].par1 = arg;
                }
                else
                    extraneous = true;
            }
    }
    if (extraneous){
        invalid_args();
        return false;
    }
    for (int o = 1; o < OPTS_COUNT; o++) {
        if ((data[o].type & opts_flags)) {
            if (opts_count > data[o].max_args) {
                invalid_args();
                return false;
            }
        }
    }

    if (!set_jwm_globals())
        return false;

    return (good_opts = true);
}


bool cmdline::show_help()
{
    for (int o = 1; o < OPTS_COUNT; o++) {
        if ((data[o].type & opts_flags)) {
            if (data[o].helpfunc){
                (this->*data[o].helpfunc)();
                return true;
            }
        }
    }
    return false;
}


bool cmdline::set_jwm_globals()
{
    wcnt::jwm.verbose  = (data[V_IX].par1) ? true : false;
    wcnt::jwm.dont_run = (data[DONT_RUN_IX].par1) ? true : false;
    wcnt::jwm.no_title = (data[NO_TITLE_IX].par1) ? true : false;
    wcnt::jwm.no_progress = (data[NO_PROGRESS_IX].par1) ? true : false;

    if (wcnt::jwm.wc_path || wcnt::jwm.wc_file){
        msg = "\nGlobals (path) being set again... I won't do it.";
        return false;
    }
    const char* filename = 0;
    const char* ptr = 0;
    const char* fnptr;
    char* path = 0;

    if (!(fnptr = ptr = filename = opts[data[WC_IX].par1])) {
        msg = "\nFilename not set.";
        return false;
    }

    while (*ptr != '\0') {
        if (*ptr == '/') fnptr = ptr + 1;
        ptr++;
    }
    if (fnptr != filename) {
        path = new char[fnptr - filename + 1];
        strncpy(path, filename, fnptr - filename);
        path[fnptr - filename] = '\0';
    }
    wcnt::jwm.wc_path  = path;
    wcnt::jwm.wc_file  = new char[strlen(filename) + 1];
    strcpy(wcnt::jwm.wc_file, filename);
    return true;
}

const char* cmdline::get_message() const
{
    return msg.c_str();
}

void cmdline::invalid_args()
{
    msg = "\nInvalid command line arguements:\n    ";
    for(int i = 0; i < opts_count; i++) {
        msg += opts[i];
        msg += " ";
    }
}

void cmdline::module_help()
{
    int n = data[MH_IX].par1;
    synthmod::TYPE smt =
        synthmod::names::type((n != 0 && n < opts_count)
            ? opts[n] : "" );
    if (smt == synthmod::NONEZERO) {
        if (wcnt::jwm.is_verbose())
            msg="\n;The 'none' module is a secretive module used whenever "
                "\n;any input is turned 'off'. It provides a zero or "
                "\n;equivalent value while the module ensures that the"
                "\n;expected 'off' behaviour is exhibited.";
        return;
    }
    if (smt == synthmod::ERR_TYPE) {
        if (data[MH_IX].par1 != 0) {
            msg = "\nUnknown synth module: ";
            if (data[MH_IX].par1 < opts_count)
                msg += opts[data[MH_IX].par1];
            msg += "\n";
        }
        msg += "\nAvailable module types are:\n";

        if (wcnt::jwm.is_verbose()) {
            for (int i = synthmod::ERR_TYPE + 2;
                     i < synthmod::LAST_TYPE; ++i)
            {
                smt = (synthmod::TYPE)i;
                std::string mhv = synthmod::names::get(smt);
                const char* descr = synthmod::names::descr(smt);
                mhv += " - ";
                std::string* d = justify(descr, 60, ' ', "\n    ",
                                                            mhv.c_str());
                msg += *d;
                msg += "\n\n";
                delete d;
            }
            return;
        }
        else
        {
            int modcount = synthmod::LAST_TYPE - 2;
            const char** modnames = new const char*[modcount];
            for (int i = synthmod::ERR_TYPE + 2;
                     i < synthmod::LAST_TYPE; i++) {
                modnames[i - 2] = synthmod::names::get(
                                    (synthmod::TYPE)i);
            }
            std::string* str = collumnize(modnames, modcount, 26, 3, true);
            msg += *str;
            delete str;
            delete [] modnames;
            return;
        }
    }
    synthmod::base* sm = synthmod::list::create_module(smt, "username");
    msg = "\n";
    msg += synthmod::names::get(smt);
    if (sm == 0) {
        msg += " module has not been fully\nincorporated into the ";
        msg += "wcnt user interface.  Oops! \n Send an email to ";
        msg += "james@jwm-art.net to complain.\n";
        return;
    }
    msg += "\n";
    msg += sm->get_username();

    const char* descr = synthmod::names::descr(smt);
    if (descr) {
        msg += "\n// ";
        std::string* d = justify(descr, 60, ' ', "\n// ", 0);
        msg += *d;
        delete d;
        msg += "\n//----";
    }

    ui::moditem_list* items = sm->get_ui_items();

    ui::moditem* item = items->goto_first();
    const char* spc = spaces::get(40);
    const char* outstr = "module_name output_name";

    int mxl1 = 0;
    int mxl2 = 0;

    while(item) {
        int l1 = 0;
        int l2 = 0;
        switch(item->get_item_type()) {
          case ui::UI_PARAM: {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            l1 = strlen(param::names::get(mp->get_param_type()));
            l2 = strlen(iocat::names::get(param::names::category(
                                                mp->get_param_type())));
            break;
          }
          case ui::UI_INPUT: {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            l1 = strlen(input::names::get(mi->get_input_type()));
            l2 = strlen(outstr);
            break;
          }
          default:
            l1 = l2 = 0;
        }
        if (mxl1 < l1)
            mxl1 = l1;
        if (mxl2 < l2)
            mxl2 = l2;
        item = items->goto_next();
    }
    mxl1 += 2;

    item = items->goto_first();
    while(item) {
        switch(item->get_item_type()) {
          case ui::UI_COMMENT: {
            ui::modcomment* mc = static_cast<ui::modcomment*>(item);
            const char* c = mc->get_comment();
            msg += "\n";
            if (c) {
                msg += "  // ";
                msg += c;
            }
            break;
          }
          case ui::UI_PARAM: {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            param::TYPE pt = mp->get_param_type();
            const char* s1 = param::names::get(pt);
            msg += "\n    ";
            msg += s1;
            msg.append(spc, mxl1 - strlen(s1));
            iocat::TYPE ioc = param::names::category(pt);
            const char* s2 = 0;
            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    s2 = fsp->get_string_list();
                else
                    s2 = "fixstringparam problem";
            }
            else
                s2 = iocat::names::get(ioc);
            msg += s2;
            if (wcnt::jwm.is_verbose()) {
                const char* c = item->get_comment();
                const char* descr = (!c ? param::names::descr(pt) : c);
                msg.append(spc, mxl2 - strlen(s2));
                msg += " // ";
                msg += descr;
            }
            break;
          }
          case ui::UI_INPUT: {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            input::TYPE it = mi->get_input_type();
            const char* s1 = input::names::get(it);
            msg += "\n    ";
            msg += s1;
            msg.append(spc, mxl1 - strlen(s1));
            msg += outstr;
            msg.append(spc, mxl2 - strlen(outstr));
            if (wcnt::jwm.is_verbose()) {
                const char* c = item->get_comment();
                const char* descr = (!c ? input::names::descr(it) : c);
                msg += " // ";
                msg += descr;
            }
            break;
          }
          case ui::UI_DOBJ: {
            ui::moddobj* md = static_cast<ui::moddobj*>(item);
            dobj_help(md->get_dobj_parent(), md->get_dobj_child(), 1);
            break;
          }
          default:
            break;
        }
        item = items->goto_next();
    }

    delete items;

    modoutputlist::linkedlist* outlist =
        new_list_of_by(wcnt::get_outputlist(), sm);

    modoutput* output = outlist->goto_first();
    if (output) {
        mxl1 = 0;
        while(output) {
            int l1 = strlen(output::names::get(output->get_outputtype()));
            if (l1 > mxl1)
                mxl1 = l1;
            output = outlist->goto_next();
        }
        mxl1 += 2;
        msg += "\n// outputs for ";
        msg += synthmod::names::get(smt);
        output = outlist->goto_first();
        while(output) {
            msg += "\n// ";
            output::TYPE ot = output->get_outputtype();
            const char* s1 = output::names::get(ot);
            msg += s1;
            output = outlist->goto_next();
            if (wcnt::jwm.is_verbose()) {
                const char* descr = output::names::descr(ot);
                msg.append(spc, mxl1 - strlen(s1));
                msg += "// ";
                msg += descr;
            }
        }
    }
    delete outlist;
    msg += "\n";
    msg += sm->get_username();
    msg += "\n";
    delete sm;
    return;
}

void
cmdline::dobj_help(dobj::TYPE parent, dobj::TYPE child, int indent_level)
{
    const char* spc = spaces::get(40);
    const char* enclosure = dobj::names::get(parent);

    if (indent_level > 4)
        indent_level = 4;

    msg += "\n";
    msg.append(spc, indent_level * 4);
    msg += enclosure;

    dobj_help_items(child, indent_level + 1);

    msg += "\n";
    msg.append(spc, indent_level * 4);
    msg += enclosure;
}

void cmdline::dobj_help_items(dobj::TYPE dt, int indent_level)
{
    dobj::base* dob = dobj::list::create_dobj(dt);
    ui::dobjitem_list* items = dob->get_ui_items();
    const char* spc = spaces::get(40);

    if (indent_level > 4)
        indent_level = 4;

    msg += "\n";
    msg.append(spc, indent_level * 4);
    msg += dobj::names::get(dt);

    ++indent_level;

    ui::dobjitem* item = items->goto_first();

    // get length of longest parameter name.
    int mxl = 0;
    while(item) {
        int l = 0;
        if (item->get_item_type() == ui::UI_PARAM) {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            l = strlen(param::names::get(dp->get_param_type()));
        }
        if (mxl < l)
            mxl = l;
        item = items->goto_next();
    }
    mxl += 2;

    item = items->goto_first();
    while(item) {
        switch(item->get_item_type()) {
          case ui::UI_PARAM: {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            param::TYPE pt = dp->get_param_type();
            const char* s = param::names::get(pt);
            msg += "\n";
            msg.append(spc, indent_level * 4);
            msg += s;
            msg.append(spc, mxl - strlen(s));
            iocat::TYPE ioc = param::names::category(pt);
            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    msg += fsp->get_string_list();
                else
                    msg += "fixstringparam problem";
            }
            else
                msg += iocat::names::get(ioc);
            if (wcnt::jwm.is_verbose()) {
                const char* c = item->get_comment();
                const char* descr = (!c ? param::names::descr(pt) : c);
                msg += " // ";
                msg += descr;
            }
            break;
          }
          case ui::UI_DOBJ: {
            ui::dobjdobj* dd = static_cast<ui::dobjdobj*>(item);
            dobj_help(dd->get_dobj_parent(), dd->get_dobj_child(),
                                                        indent_level + 1);
          }
          default:
            break;
        }
        item = items->goto_next();
    }
    delete items;
}

void cmdline::dobj_help()
{
    int n = data[DH_IX].par1;
    std::string dname = (n != 0 && n < opts_count) ? opts[n] : "";
    dobj::TYPE dt = dobj::names::type(dname.c_str());

    dobj::base* dob = wcnt::get_dobjlist()->create_dobj(dt);
    if (!dob) {
        // incorrect dobj name or no name specified
        // (dt will be dobj::ERR_TYPE).
        if (opts_count == 3)
            msg = "\nno data object available named " + dname;
        msg += "\navailable data object types are:\n\n";
        int count = 0;
        const char** dbjnames =
            dobj::names::all_in_category(dobj::CAT_DEF, &count);
        std::string* str = collumnize(dbjnames, count, 20, 2, true);
        msg += *str;
        delete str;
        delete [] dbjnames;
        return;
    }

    msg += "\n";
    msg += dobj::names::get(dt);
    msg += "\nusername";

    const char* descr = dobj::names::descr(dt);
    if (descr) {
        msg += "\n// ";
        std::string* d = justify(descr, 60, ' ', "\n// ", 0);
        msg += *d;
        delete d;
        msg += "\n//----";
    }

    ui::dobjitem_list* items = dob->get_ui_items();
    ui::dobjitem* item = items->goto_first();
    int mxl = 0;
    const char* spc = spaces::get(40);

    while(item) {
        int l = 0;
        switch(item->get_item_type()) {
          case ui::UI_PARAM: {
            ui::dobjparam* mp = static_cast<ui::dobjparam*>(item);
            l = strlen(param::names::get(mp->get_param_type()));
            break;
          }
          default:
            l = 0;
        }
        if (mxl < l)
            mxl = l;
        item = items->goto_next();
    }
    mxl += 2;

    item = items->goto_first();
    while(item) {
        switch(item->get_item_type()) {
          case ui::UI_COMMENT: {
            ui::dobjcomment* dc = static_cast<ui::dobjcomment*>(item);
            const char* c = dc->get_comment();
            msg += "\n";
            if (c) {
                msg += "  // ";
                msg += c;
            }
            break;
          }
          case ui::UI_PARAM: {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            param::TYPE pt = dp->get_param_type();
            const char* s = param::names::get(pt);
            msg += "\n    ";
            msg += s;
            msg.append(spc, mxl - strlen(s));
            iocat::TYPE ioc = param::names::category(pt);
            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    msg += fsp->get_string_list();
                else
                    msg += "fixstringparam problem";
            }
            else
                msg += iocat::names::get(ioc);
            if (wcnt::jwm.is_verbose()) {
                const char* c = item->get_comment();
                const char* descr = (!c ? param::names::descr(pt) : c);
                msg += " // ";
                msg += descr;
            }
            break;
          }
          case ui::UI_DOBJ: {
            ui::dobjdobj* dd = static_cast<ui::dobjdobj*>(item);
            dobj_help(dd->get_dobj_parent(), dd->get_dobj_child(), 1);
            break;
          }
          default:
            break;
        }
        item = items->goto_next();
    }

    delete items;
}


void cmdline::input_help()
{ 
// decide by the number of options passed on command line whether to 
// load a wcnt file or to create all possible modules in order to 
// access outputs.
    input::TYPE intype = input::names::type(opts[data[IH_IX].par1]);
    if (intype == input::ERR_TYPE) {
        msg = "";
        if (data[IH_IX].par1) {
            msg = "\nUnknown input type: ";
            msg += opts[data[IH_IX].par1];
        }
        msg += "\nAvailable input types are:\n";
        int incount = input::LAST_TYPE - 1;
        const char** innames = new const char*[incount];
        size_t mxl = 0;
        for (int i = input::ERR_TYPE + 1; i < input::LAST_TYPE; i++) {
            innames[i - 1] = input::names::get((input::TYPE)i);
            size_t l = strlen(innames[i - 1]);
            if (mxl < l)
                mxl = l;
        }
        if (wcnt::jwm.is_verbose()) {
            const char* spc = spaces::get(40);
            for (int i = input::ERR_TYPE + 1; i < input::LAST_TYPE; ++i) {
                msg += "\n";
                msg += innames[i - 1];
                msg.append(spc, mxl - strlen(innames[i - 1]));
                msg += "// ";
                msg += input::names::descr((input::TYPE)i);
            }
        }
        else {
            int cc = 80 / (mxl + 2);
            int cw = 80 / cc;
            std::string* str = collumnize(innames, incount, cw, cc, true);
            msg += *str;
            delete str;
        }
        delete [] innames;
        return;
    }
    msg = "";
    iocat::TYPE incat = input::names::category(intype);
    if (data[WC_IX].par1 > 0) {
        // must create all modules to gain access to compatible outputs
        wcnt::synth jwm_synth;
        if (!jwm_synth.is_valid() || !jwm_synth.generate_synth()) {
            msg =
                "\nproblems reading synthfile:\n" +
                msg + "\nproceeding anyway";
        }
    }
    else {
        // only create modules within a .wc file to access outputs
        synthmod::base* sm;
        int i;
        for (i = synthmod::ERR_TYPE + 1; i < synthmod::LAST_TYPE; i++)
        {
            if (i != synthmod::NONEZERO) {
                sm = synthmod::list::create_module((synthmod::TYPE)i,
                                synthmod::names::get((synthmod::TYPE)i));
                if (!sm) {
                    msg += "\nnot enough memory to process request to"
                                            "create synthmodule type: ";
                    msg += synthmod::names::get((synthmod::TYPE)i);
                    return;
                }
                wcnt::jwm.get_modlist()->add_module(sm);
            }
        }
    }
    msg += "\nOutputs matching category of input ";
    msg += input::names::get(intype);
    msg += " are:\n";
    // now get the outputs matching category of input
    modoutputlist* outputs =
                        wcnt::get_outputlist()->list_of_category(incat);
    modoutput* output = outputs->goto_first();
    std::string mn;
    int mxl = 0;
    while(output) {
        mn = output->get_module()->get_username();
        int l = mn.length();
        if (l > mxl) mxl = l;
        output = outputs->goto_next();
    }
    mxl += 2;
    output = outputs->goto_first();
    const char* spc = spaces::get(40);
    while(output)
    { // this if statement prevents from displaying off off
        // now it only displays one off - less confusing.
        if (output->get_moduletype() != synthmod::NONEZERO) {
            mn = output->get_module()->get_username();
            msg += mn;
            msg.append(spc, mxl - mn.length());
        }
        msg += output::names::get(output->get_outputtype());
        msg += "\n";
        output = outputs->goto_next();
    }
    delete outputs; // delete list we created
    return;
}

void cmdline::sample_info()
{
    if (!data[SI_IX].par1) {
        msg = "\nPlease specify an audio sample file to display"
            " information about.";
        return;
    }
    wavfilein wavfile;
    wavfile.set_wav_filename(opts[data[SI_IX].par1]);
    // set root note so that validate() won't complain about it
    wavfile.set_root_note("c0");
    // validate no longer actually opens the file, have to explicitly
    // call wavfilein::open_wav, these days...
    wavfile.validate();
    wavfile.open_wav();
    if (wavfile.get_status() != WAV_STATUS_OPEN) {
        msg = "\nOpening of ";
        msg += opts[data[SI_IX].par1];
        msg += " failed... ";
        msg += *wavfile.get_error_msg();
        return;
    }
    msg = "\nheader info for wav: ";
    msg += opts[data[SI_IX].par1];
    switch(wavfile.get_channel_status())
    {
    case WAV_CH_MONO:
        msg += "\n\tMono";
        break;
    case WAV_CH_STEREO:
        msg += "\n\tStereo";
        break;
    default:
        msg += "\n\tUnsupported number of channels\n";
    }
    msg += "\n\tBitrate supported ;)";
    std::ostringstream conv;
    conv << "\n\t" << wavfile.get_sample_rate() << " hz";
    conv << "\n\tlength: " << wavfile.get_length() << " samples (";
    conv << (double)wavfile.get_length() / wcnt::jwm.samplerate();
    conv << " seconds)\n";
    msg += conv.str();
    return;
}

void cmdline::note_info()
{
    if (!data[NI_IX].par1){
        msg = "\nPlease specify the name of a note to be converted to"
            " a frequency.\n(Note name examples: c#-2  e1  f#0  b-1)";
        return;
    }
    if (!check_notename(opts[data[NI_IX].par1])) {
        msg = "\nInvalid note name";
        return;
    }
    std::ostringstream conv;
    conv << "\nNote name " << opts[data[NI_IX].par1];
    conv << " is frequency " << note_to_freq(opts[data[NI_IX].par1]);
    msg = conv.str();
    return;
}

void cmdline::freq_info()
{
    if (!data[FI_IX].par1){
        msg = "\nPlease specify a frequency and samplerate for "
            "converting to phase_step.";
        return;
    }
    if (!data[FI_IX].par2){
        msg = "\nPlease specify the samplerate which the frequency"
            " will be converted to\nphase_step for.";
        return;
    }
    double frequency = atof(opts[data[FI_IX].par1]);
    wcnt::jwm.samplerate(atol(opts[data[FI_IX].par2]));
    if (frequency == 0) {
        msg = "\nInvalid frequency";
        return;
    }
    if (wcnt::jwm.samplerate() == 0) {
        msg = "\nInvalid sample rate";
        return;
    }
    std::ostringstream conv;
    conv << "\nFrequency " << frequency;
    conv << " at samplerate " << wcnt::jwm.samplerate();
    conv << "\n\tphase_step " << freq_to_step(frequency);
    conv << "\n\tsamples  " << freq_to_samples(frequency);
    msg = conv.str();
    return;
}
void cmdline::header()
{
    msg = "\nwcnt .wc file header (and footer) is:\n\t";
    msg += wcnt::wcnt_id;
}

void cmdline::help()
{
    const char* const wcnt = "\nwcnt ";
    msg = "\nwcnt command line help\n";
    for(int i = WC_FILE; i < OPTS_COUNT; i++){
        msg += wcnt;
        if (data[i].olong && data[i].oshort) {
            msg += data[i].olong;
            msg += " (or ";
            msg += data[i].oshort;
            msg += ") ";
        }
        else {
            if (data[i].olong)
                msg += data[i].olong;
            else if (data[i].oshort)
                msg += data[i].oshort;
            msg += " ";
        }
        if (data[i].odisplay)
            msg += data[i].odisplay;
        if (opts_flags & LONGHELP) {
            if (data[i].helptext) {
                msg += "\n";
                msg += data[i].helptext;
                msg += "\n";
            }
        }
    }
}

void cmdline::about()
{
    msg =
        "\nwcnt - Wav Composer Not Toilet"
        "\n  Programmed by James W. Morris --> http://www.jwm-art.net"
        "\n  GNU GPL'ed Free Libre Open Source Software."
        "\n  wcnt on the web:"
        "\n    http://wcnt.sourceforge.net"
        "\n    http://www.jwm-art.net/o7.php&p=keywords&k=wcnt"
        "\n    https://github.com/jwm-art-net/wcnt"
        "\n  Send bug reports, patches, requests, suggestions,"
        "\n  or if you wish to share your .wc creations to:"
        "\n    james@jwm-art.net";
}

//#include "../data/commandline_help.h"

