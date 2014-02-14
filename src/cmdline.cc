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
#include "../include/ladspa_loader.h"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iostream>


// ID               long                short
cmdline::switches cmdline::swdata[SW_COUNT] = {
 {  VERBOSE,        "verbose",        "v"    },
 {  NO_RUN,         "no-run",         "nr"   },
 {  NO_TITLE,       "no-title",       "nt"   },
 {  NO_PROGRESS,    "no-progress",    "np"   },
 {  MINIMAL,        "minimal",        "m"    }
};


//  ID              long            short   min/max-args,   wc_opt,    valid flags
cmdline::commands cmdline::cmddata[CMD_COUNT] = {
 {  RUN,            0,              0,      1,1,    1,  VERBOSE | NO_RUN | NO_TITLE | NO_PROGRESS },
 {  HELP,           "help",         "h",    0,0,    0,  VERBOSE | NO_TITLE },
 {  MOD_HELP,       "mod-help",     "mh",   0,1,    0,  VERBOSE | NO_TITLE | MINIMAL },
 {  DOBJ_HELP,      "dobj-help",    "dh",   0,1,    0,  VERBOSE | NO_TITLE | MINIMAL },
 {  INPUT_HELP,     "input-help",   "ih",   1,2,    3,  NO_TITLE },
 {  LADSPA_HELP,    "ladspa-help",  "lh",   0,2,    0,  VERBOSE | NO_TITLE | MINIMAL },
 {  SAMPLE_INFO,    "sample-info",  "si",   1,1,    0,  NO_TITLE },
 {  NOTE_INFO,      "note-info",    "ni",   1,2,    0,  NO_TITLE },
 {  FREQ_INFO,      "freq-info",    "fi",   1,1,    0,  NO_TITLE },
 {  HEADER,         "header",       0,      0,0,    0,  NO_TITLE },
 {  ABOUT,          "about",        0,      0,0,    0,  0 }
};

// ID               description\display
cmdline::textuals cmdline::texts[CMD_COUNT] = {
 {  RUN,            "Generate and execute synth specified in file "
                    "(by convention has .wc extension).",
                    "[FILE]"                        },

 {  HELP,           "Display command line help.",
                    0                               },

 {  MOD_HELP,       "List all modules, or show definition of specific module.",
                    "[MODULE NAME]"                 },

 {  DOBJ_HELP,      "Show data-object help.",
                    "[DOBJ NAME]"                   },

 {  INPUT_HELP,     "Show valid outputs for specific input. The outputs "
                    "displayed can be optionally limited to those created "
                    "by [FILE].",
                    "[INPUT NAME] [FILE]"           },

 {  LADSPA_HELP,    "List all LADSPA libraries and plugins, or list plugins "
                    "within specifc library, or show module definition for "
                    "specifc LADSPA plugin.",
                    "[LIBRARY NAME] [PLUGIN LABEL]" },

 {  SAMPLE_INFO,    "Display sample information about [AUDIO FILE]. Works with "
                    "any format supported by libsndfile.",
                    "[AUDIO FILE]." },

 {  NOTE_INFO,      "Display frequency information for [NOTE NAME]. Optionally "
                    "provide [SAMPLE RATE] to display the phase-step value.",
                    "[NOTE NAME] [SAMPLE RATE]"     },

 {  FREQ_INFO,      "Display the phase-step value for given [FREQUENCY] at "
                    "specified [SAMPLE RATE].",
                    "[FREQUENCY] [SAMPLE RATE]"     },

 {  HEADER,         "Display the header that should be in place at the top "
                    "and bottom of every .wc file.",
                    0 },

 {  ABOUT,          "Display contact information about Wav Composer Not Toilet",
                    0 }
};


cmdline::cmdline(int argc, char** argv) :
 opt_count(0), opts(0), switch_flags(0), ladspa_lib_arg(0), ladspa_label_arg(0),
 wc_file(0)
{
    int opt = 0;;
    opts = new char*[argc];
    for (int arg = 0; arg < argc; ++arg) {
        opts[arg] = 0;
        debug("checking argv[%d] ('%s')...\n", arg, argv[arg]);
        if (*argv[arg] == '-') {
            bool match = false;
            debug("checking argv[%d] ('%s') against switches\n", arg, argv[arg]);
            for (int sw = 0; sw < SW_COUNT; ++sw) {
                if (*(argv[arg] + 1) == '-') {
                    if (strcmp(argv[arg] + 2, swdata[sw]._long) == 0) {
                        switch_flags |= swdata[sw].value;
                        match = true;
                        debug("matched '%s'\n", swdata[sw]._long);
                    }
                }
                else if (strcmp(argv[arg] + 1, swdata[sw]._short) == 0) {
                    switch_flags |= swdata[sw].value;
                    match = true;
                    debug("matched '%s'\n", swdata[sw]._long);
                }
            }
            if (!match) // assume is not switch, process later...
                opts[opt++] = argv[arg];
        }
        else
            opts[opt++] = argv[arg];
    }
    opt_count = opt;
}


cmdline::~cmdline()
{
    if (opts)
        delete [] opts;
}


bool cmdline::scan()
{
    int cmd;
    int match_cmd = RUN; // RUN == 0

    #if DEBUG
    for (int i = 0; i < opt_count; ++i)
        debug("cmd opts[%d] '%s'\n", i, opts[i]);
    #endif

    if (opt_count < 2) {
        help();
        return false;
    }

    if (*opts[1] == '-') {
        match_cmd = -1;
        for (cmd = 1; cmd < CMD_COUNT; ++cmd) {
            if (*(opts[1] + 1) == '-') {
                if (strcmp(opts[1] + 2, cmddata[cmd]._long) == 0) {
                    match_cmd = cmd;
                    break;
                }
            }
            else if (strcmp(opts[1] + 1, cmddata[cmd]._short) == 0) {
                match_cmd = cmd;
                break;
            }
        }

        if (match_cmd == -1) {
            debug("unrecognized argument.\n");
            invalid_args();
            return false;
        }
        #if DEBUG
        else
            debug("matched '%s'\n", cmddata[match_cmd]._long);
        #endif

        if (opt_count - 2 < cmddata[match_cmd].min_args
         || opt_count - 2 > cmddata[match_cmd].max_args) {
            debug("wrong argument count.\n");
            invalid_args();
            return false;
        }
    }

    debug("match_cmd:%d\n",match_cmd);

    int wc_opt = cmddata[match_cmd].wc_opt;

    if (wc_opt && wc_opt < opt_count) {
        wc_file = opts[wc_opt];
        debug(".wc file '%s'\n", wc_file);
    }

    set_jwm_globals();

    switch(match_cmd) {
      case RUN:         return true;
      case MOD_HELP:    mod_help();     break;
      case DOBJ_HELP:   dobj_help();    break;
      case INPUT_HELP:  input_help();   break;
      case LADSPA_HELP: ladspa_help();  break;
      case SAMPLE_INFO: sample_info();  break;
      case NOTE_INFO:   note_info();    break;
      case FREQ_INFO:   freq_info();    break;
      case HEADER:      header();       break;
      case ABOUT:       about();        break;
      default:
        help();
    }

    return false;
}


bool cmdline::set_jwm_globals()
{
    wcnt::jwm.verbose  =    (switch_flags & VERBOSE);
    wcnt::jwm.dont_run =    (switch_flags & NO_RUN);
    wcnt::jwm.no_title =    (switch_flags & NO_TITLE);
    wcnt::jwm.no_progress = (switch_flags & NO_PROGRESS);

    if (wcnt::jwm.wc_path || wcnt::jwm.wc_file) {
        msg = "\nGlobals (path) being set again... I won't do it.";
        return false;
    }

    if (wc_file) {
        const char* ptr = 0;
        const char* fnptr;
        char* path = 0;

        fnptr = ptr = wc_file;

        while (*ptr != '\0') {
            if (*ptr == '/')
                fnptr = ptr + 1;
            ptr++;
        }

        if (fnptr != wc_file) {
            path = new char[fnptr - wc_file + 1];
            strncpy(path, wc_file, fnptr - wc_file);
            path[fnptr - wc_file] = '\0';
        }

        wcnt::jwm.wc_path  = path;
        wcnt::jwm.wc_file  = new_strdup(wc_file);
    }
    return true;
}


const char* cmdline::get_message() const
{
    return msg.c_str();
}


void cmdline::invalid_args()
{
    msg = "\nInvalid command line arguements:\n    ";
    for(int i = 0; i < opt_count; i++) {
        msg += opts[i];
        msg += " ";
    }
}


void cmdline::module_help_list_all()
{
    if (opt_count) {
        if (opts[2] != 0) {
            msg = "\nUnknown synth module: ";
            msg += opts[1];
            msg += "\n";
        }
    }
    msg += "\nAvailable module types are:\n";

    if (wcnt::jwm.is_verbose()) {
        for (int i = synthmod::ERR_TYPE + 2; i < synthmod::LAST_TYPE; ++i) {
            synthmod::TYPE smt = (synthmod::TYPE)i;
            std::string mhv = synthmod::names::get(smt);
            const char* descr = synthmod::names::descr(smt);
            mhv += " - ";
            std::string* d = justify(descr, 60, ' ', "\n    ",
                                                        mhv.c_str());
            msg += *d;
            msg += "\n\n";
            delete d;
        }
    }
    else {
        int modcount = synthmod::LAST_TYPE - 2;
        const char** modnames = new const char*[modcount];
        for (int i = synthmod::ERR_TYPE + 2;
                 i < synthmod::LAST_TYPE; i++) {
            modnames[i - 2] = synthmod::names::get((synthmod::TYPE)i);
        }
        std::string* str = collumnize(modnames, modcount, 26, 3, true);
        msg += *str;
        delete str;
        delete [] modnames;
    }
}


void cmdline::mod_help()
{
    int smt;

    if (ladspa_lib_arg && ladspa_label_arg)
        smt = synthmod::LADSPA;
    else if (opt_count > 2)
        smt = synthmod::names::type(opts[2]);
    else
        smt = synthmod::ERR_TYPE;

    if (smt == synthmod::ERR_TYPE) {
        module_help_list_all();
        return;
    }

    if (smt == synthmod::NONEZERO) {
        if (wcnt::jwm.is_verbose())
            msg="\n;The 'none' module is a secretive module used whenever "
                "\n;any input is turned 'off'. It provides a zero or "
                "\n;equivalent value while the module ensures that the"
                "\n;expected 'off' behaviour is exhibited.";
        return;
    }

    synthmod::base* sm = synthmod::list::create_module(smt, "USERNAME");
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
    ui::moditem* item = (items != 0 ? items->goto_first() : 0);

    const char* outstr = "MODULE OUTPUT";

    int mxl1 = 0;
    int mxl2 = 0;
    bool custom = false;

    if (ladspa_lib_arg && ladspa_label_arg) {
        sm->set_param(param::LADSPA_LIB, opts[ladspa_lib_arg]);
        sm->set_param(param::LADSPA_LABEL, opts[ladspa_label_arg]);
    }

    while(item) {
        int l1 = 0;
        int l2 = 0;
        switch(item->get_item_type()) {
          case ui::UI_PARAM: {
            ui::modparam* mp = static_cast<ui::modparam*>(item);
            int pt = mp->get_param_type();
            iocat::TYPE ioc = param::names::category(pt);

            l1 = strlen(param::names::get(pt));

            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    l2 = strlen(fsp->get_string_list());
                else
                    l2 = strlen("fixstringparam problem");
            }
            else
                l2 = strlen(iocat::names::get(ioc));
            break;
          }
          case ui::UI_INPUT: {
            ui::modinput* mi = static_cast<ui::modinput*>(item);
            l1 = strlen(input::names::get(mi->get_input_type()));
            l2 = strlen(outstr);
            break;
          }
          case ui::UI_CUSTOM: {
            if (sm->create_custom_ui_items()) {
                sm->activate_custom_ui_items();
                items = sm->get_ui_items();
                item = items->first_item();
                custom = true;
                continue;
            }
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

    if (custom) {
        sm->deactivate_custom_ui_items();
        items = sm->get_ui_items();
    }

    item = (items != 0 ? items->first_item() : 0);

    while(item) {
        if (!item->is_dummy()) {
            const char* lead = "\n    ";
            char flags[8] = "";

            ui::TYPE itemtype = item->get_item_type();

            if (wcnt::jwm.is_verbose()) {
                std::string str = items->get_item_header();
                if (str != "") {
                    msg += "\n    ";
                    msg += str;
                }
                item->get_item_flags(flags, 8);
            }
            else if ((item->get_option_id() & ui::UI_OPTION_MASK)
                  && !item->is_ui_opt_duplicate()) {
                // when not verbose, we only want to show the normal items
                // like what happened before any of this multiple choice
                // malarky. skip the items not to be shown by forcing the
                // default in the switch below...
                itemtype = ui::UI_ERROR;
            }

            switch(itemtype) {
              case ui::UI_COMMENT: {
                ui::modcomment* mc = static_cast<ui::modcomment*>(item);
                const char* c = mc->get_descr();
                msg += "\n";
                if (c) {
                    msg += "    // ";
                    msg += c;
                }
                break;
              }
              case ui::UI_PARAM: {
                ui::modparam* mp = static_cast<ui::modparam*>(item);
                int pt = mp->get_param_type();
                const char* s1 = param::names::get(pt);
                msg += lead;
                msg += s1;
                msg.append(spaces::get(mxl1), mxl1 - strlen(s1));
                iocat::TYPE ioc = param::names::category(pt);
                const char* s2 = 0;

                if (ladspa_lib_arg && pt == param::LADSPA_LIB)
                    s2 = opts[ladspa_lib_arg];
                else if (ladspa_label_arg && pt == param::LADSPA_LABEL)
                    s2 = opts[ladspa_label_arg];
                else if (ioc == iocat::FIX_STR) {
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
                    msg.append(spaces::get(mxl2), mxl2 - strlen(s2));
                    msg += " // ";
                    msg += flags;
                    const char* c = item->get_descr();
                    const char* descr = (!c ? param::names::descr(pt) : c);
                    msg += " ";
                    msg += descr;
                }
                break;
              }
              case ui::UI_INPUT: {
                ui::modinput* mi = static_cast<ui::modinput*>(item);
                int it = mi->get_input_type();
                const char* s1 = input::names::get(it);

                msg += lead;
                msg += s1;
                msg.append(spaces::get(mxl1), mxl1 - strlen(s1));
                msg += outstr;
                if (wcnt::jwm.is_verbose()) {
                    msg.append(spaces::get(mxl2), mxl2 - strlen(outstr));
                    msg += " // ";
                    msg += flags;
                    const char* c = item->get_descr();
                    const char* descr = (!c ? input::names::descr(it) : c);
                    msg += " ";
                    msg += descr;
                }
                break;
              }
              case ui::UI_DOBJ: {
                ui::moddobj* md = static_cast<ui::moddobj*>(item);
                dobj_help(md->get_dobj_parent(), md->get_dobj_child(), 1);
                break;
              }
              case ui::UI_CUSTOM: {
                if (sm->activate_custom_ui_items()) {
                    items = sm->get_ui_items();
                    item = items->first_item();
                    continue;
                }
                break;
              }
              default:
                break;
            }
        }
        item = items->next_item();
    }

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
            int ot = output->get_outputtype();
            const char* s1 = output::names::get(ot);
            msg += s1;
            output = outlist->goto_next();
            if (wcnt::jwm.is_verbose()) {
                const char* descr = output::names::descr(ot);
                msg.append(spaces::get(mxl1), mxl1 - strlen(s1));
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
    const char* enclosure = dobj::names::get(parent);

    if (indent_level > 4)
        indent_level = 4;

    msg += "\n";
    msg.append(spaces::get(20), indent_level * 4);
    msg += enclosure;

    dobj_help_items(child, indent_level + 1);

    msg += "\n";
    msg.append(spaces::get(20), indent_level * 4);
    msg += enclosure;
}

void cmdline::dobj_help_items(dobj::TYPE dt, int indent_level)
{
    dobj::base* dob = dobj::list::create_dobj(dt);
    ui::dobjitem_list* items = dob->get_ui_items();

    if (!items) {
        delete dob;
        return;
    }

    if (indent_level > 4)
        indent_level = 4;

    msg += "\n";
    msg.append(spaces::get(20), indent_level * 4);
    msg += dobj::names::get(dt);

    ++indent_level;

    ui::dobjitem* item = items->goto_first();

    // get length of longest parameter name.
    int mxl1 = 0;
    int mxl2 = 0;
    while(item) {
        int l1 = 0;
        int l2 = 0;
        if (item->get_item_type() == ui::UI_PARAM) {
            ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
            int pt = dp->get_param_type();
            iocat::TYPE ioc = param::names::category(pt);
            l1 = strlen(param::names::get(pt));
            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = wcnt::get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    l2 = strlen(fsp->get_string_list());
                else
                    l2 = strlen("fixstringparam problem");
            }
            else
                l2 = strlen(iocat::names::get(ioc));
        }
        if (mxl1 < l1)
            mxl1 = l1;
        if (mxl2 < l2)
            mxl2 = l2;
        item = items->goto_next();
    }
    mxl1 += 2;
    item = (items != 0 ? items->first_item() : 0);

    while(item) {
        if (!item->is_dummy()) {
            char flags[8];
            ui::TYPE itemtype = item->get_item_type();

            if (wcnt::jwm.is_verbose()) {
                std::string str = items->get_item_header();
                if (str != "") {
                    msg += "\n";
                    msg.append(spaces::get(20), indent_level * 4);
                    msg += str;
                }
            }
            else if ((item->get_option_id() & ui::UI_OPTION_MASK)
                  && !item->is_ui_opt_duplicate()) {
                // not an error, just the simplest way of skipping items
                // we only want to show in verbose mode...
                itemtype = ui::UI_ERROR;
            }

            item->get_item_flags(flags, 8);

            switch(itemtype) {
              case ui::UI_COMMENT: {
                ui::dobjcomment* dc = static_cast<ui::dobjcomment*>(item);
                const char* c = dc->get_descr();
                msg += "\n";
                msg.append(spaces::get(20), indent_level * 4);
                if (c) {
                    msg += "// ";
                    msg += c;
                }
                break;
              }
              case ui::UI_PARAM: {
                ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
                int pt = dp->get_param_type();
                const char* s1 = param::names::get(pt);
                msg += "\n";
                msg.append(spaces::get(20), indent_level * 4);
                msg += s1;
                msg.append(spaces::get(mxl1), mxl1 - strlen(s1));
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
                    msg.append(spaces::get(mxl2), mxl2 - strlen(s2));
                    msg += " // ";
                    msg += flags;
                    const char* c = item->get_descr();
                    const char* descr = (!c ? param::names::descr(pt) : c);
                    msg += " ";
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
        }
        item = items->next_item();
    }

    delete dob;
}

void cmdline::dobj_help()
{
    int dt;

    if (opt_count)
        dt = dobj::names::type(opts[1]);
    else
        dt = dobj::ERR_TYPE;

    if (dt == dobj::ERR_TYPE) {
        if (opt_count) {
            msg = "\nno data object available named ";
            msg += opts[1];
        }
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

    dobj::base* dob = wcnt::get_dobjlist()->create_dobj(dt);

    msg += "\n";
    msg += dobj::names::get(dt);
    msg += "\nUSERNAME";

    const char* descr = dobj::names::descr(dt);
    if (descr) {
        msg += "\n// ";
        std::string* d = justify(descr, 60, ' ', "\n// ", 0);
        msg += *d;
        delete d;
        msg += "\n//----";
    }

    ui::dobjitem_list* items = dob->get_ui_items();

    if (!items) {
        msg += "\nUSERNAME";
        return;
    }

    ui::dobjitem* item = items->goto_first();

    int mxl = 0;

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

    item = items->first_item();
    while(item) {
        if (!item->is_dummy()) {
            char flags[8];
            std::string str = items->get_item_header();
            if (str != "") {
                msg += "\n    ";
                msg += str;
            }
            item->get_item_flags(flags, 8);
            switch(item->get_item_type()) {
              case ui::UI_COMMENT: {
                ui::dobjcomment* dc = static_cast<ui::dobjcomment*>(item);
                const char* c = dc->get_descr();
                msg += "\n";
                if (c) {
                    msg += "  // ";
                    msg += c;
                }
                break;
              }
              case ui::UI_PARAM: {
                ui::dobjparam* dp = static_cast<ui::dobjparam*>(item);
                int pt = dp->get_param_type();
                const char* s = param::names::get(pt);
                msg += "\n    ";
                msg += s;
                msg.append(spaces::get(mxl), mxl - strlen(s));
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
                msg += " // ";
                msg += flags;
                if (wcnt::jwm.is_verbose()) {
                    const char* c = item->get_descr();
                    const char* descr = (!c ? param::names::descr(pt) : c);
                    msg += " ";
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
        }
        item = items->next_item();
    }
    msg += "\nUSERNAME";
}


void cmdline::ladspa_help()
{
    if (opt_count > 2) {
        ladspa_lib_arg = 2;
        if (opt_count > 3)
            ladspa_label_arg = 3;
    }

    if (!ladspa_lib_arg || !ladspa_label_arg) {
        msg = "Please specify the LADSPA library name and the LADSPA plugin\n"
              "label to show the ladspa module definition help for.\n\n";
        if (wcnt::jwm.is_verbose()) {
            msg+= "Note: depending on where the LADSPA library you wish to use\n"
                  "      is located, and whether the LADSPA_PATH environment\n"
                  "      variable is set or not, you may not need to specify\n"
                  "      the full path to the plugin. When LADSPA_PATH is not\n"
                  "      set, and the full plugin path is not specified, wcnt\n"
                  "      will use:\n\n    ";
            msg += wcnt::ladspa_path_if_env_not_set;
            msg += "\n";
        }
    }

    if (!ladspa_lib_arg) {
        ladspa_loader* ladspaloader = wcnt::jwm.get_ladspaloader();
        ladspaloader->load_all();

        ladspa_lib* l = ladspaloader->goto_first();
        while(l) {
            const char* libname = strrchr(l->get_path(), '/') + 1;
            msg += libname;
            msg += "\n";
            for (int plug_ix = 0;; plug_ix++) {
                const LADSPA_Descriptor* descr = (l->get_descr_func())(plug_ix);
                if (!descr)
                    break;
                msg += "\t";
                msg += descr->Label;
                msg += "\n";
            }
            msg += "\n";
            l = ladspaloader->goto_next();
        }
        return;
    }

    mod_help();
}



void cmdline::input_help()
{
// decide by the number of options passed on command line whether to
// load a wcnt file or to create all possible modules in order to
// access outputs.
    int intype = input::names::type(opts[2]);
    if (intype == input::ERR_TYPE) {
        msg = "";
        if (opts[2]) {
            msg = "\nUnknown input type: ";
            msg += opts[1];
        }
        msg += "\nAvailable input types are:\n";
        int incount = input::LAST_TYPE - 1;
        const char** innames = new const char*[incount];
        size_t mxl = 0;
        for (int i = input::ERR_TYPE + 1; i < input::LAST_TYPE; i++) {
            innames[i - 1] = input::names::get(i);
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
                msg += input::names::descr(i);
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
    if (wc_file) {
        debug("Reading .wc file '%s'...\n", wc_file);
        // only create modules specified in wc file
        wcnt::synth jwm_synth;
        if (!jwm_synth.is_valid() || !jwm_synth.generate_synth()) {
            msg =
                "\nproblems reading synthfile:\n" +
                msg + "\nproceeding anyway";
        }
    }
    else {
        synthmod::base* sm;
        for (int i = synthmod::ERR_TYPE + 1; i < synthmod::LAST_TYPE; i++)
        {
            if (i != synthmod::NONEZERO) {
                debug("creating synthmod '%s'\n",
                                synthmod::names::get((synthmod::TYPE)i));
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
#ifdef TEDIUM_UNITED
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
#endif
}

void cmdline::note_info()
{
#ifdef TEDIUM_UNITED
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
#endif
}

void cmdline::freq_info()
{
#ifdef TEDIUM_UNITED
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
#endif
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
    for (int i = 0; i < CMD_COUNT; ++i) {
        msg += wcnt;
        if (cmddata[i]._long) {
            msg += "--";
            msg += cmddata[i]._long;
            if (cmddata[i]._short) {
                msg += " (or -";
                msg += cmddata[i]._short;
                msg += ") ";
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
