#include "../include/cmdline.h"
#include "../include/jwm_globals.h"
#include "../include/conversions.h"
#include "../include/wavfilein.h"
#include "../include/synthmod.h"

#include "../include/synthmodlist.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"

#include "../include/fxsparamlist.h"

#include "../include/topdobjlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjparamlist.h"
#include "../include/dobjdobj.h"

#include "../include/jwmsynth.h"

#include "../include/textstuff.h"

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
                    std::cout << " as .wc file.";
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

    for (int o = 1; o < OPTS_COUNT; o++) {
        if ((data[o].type & opts_flags)) {
            if (data[o].helpfunc){
                (this->*data[o].helpfunc)();
                return false;
            }
        }
    }
    return (good_opts = true);
}

bool cmdline::set_jwm_globals()
{
    jwm.verbose  = (data[V_IX].par1) ? true : false;
    jwm.dont_run = (data[DONT_RUN_IX].par1) ? true : false;
    jwm.no_title = (data[NO_TITLE_IX].par1) ? true : false;

    if (jwm.wc_path || jwm.wc_file){
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
    jwm.wc_path  = path;
    jwm.wc_file  = new char[strlen(filename) + 1];
    strcpy(jwm.wc_file, filename);
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
    module::TYPE smt =
        module::names::type((n != 0 && n < opts_count)
            ? opts[n] : "" );
    if (smt == module::NONEZERO) {
        msg="\nThe module definition requested is a secret module which"
            " is used whenever an\ninput is turned 'off'. What did you"
            " expect?";
        return;
    }
    if (smt == module::ERR_TYPE) {
        if (data[MH_IX].par1 != 0) {
            msg = "\nUnknown synth module: ";
            if (data[MH_IX].par1 < opts_count)
                msg += opts[data[MH_IX].par1];
            msg += "\n";
        }
        msg += "\nAvailable module types are:\n";

        if (jwm.is_verbose()) {
            for (int i = module::ERR_TYPE + 2;
                     i < module::LAST_TYPE; ++i)
            {
                smt = (module::TYPE)i;
                std::string mhv = module::names::get(smt);
                const char* descr = module::names::descr(smt);
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
            int modcount = module::LAST_TYPE - 2;
            const char** modnames = new const char*[modcount];
            for (int i = module::ERR_TYPE + 2;
                     i < module::LAST_TYPE; i++) {
                modnames[i - 2] = module::names::get(
                                    (module::TYPE)i);
            }
            std::string* str = collumnize(modnames, modcount, 26, 3, true);
            msg += *str;
            delete str;
            delete [] modnames;
            return;
        }
    }
    synthmod* sm = jwm.get_modlist()->create_module(smt, "username");
    msg = "\n";
    msg += module::names::get(smt);
    if (sm == 0) {
        msg += " module has not been fully\nincorporated into the ";
        msg += "wcnt user interface.  Oops! \n Send an email to ";
        msg += "james@jwm-art.net to complain.\n";
        return;
    }
    msg += "\n";
    msg += sm->get_username();

    const char* descr = module::names::descr(smt);
    if (descr) {
        msg += "\n// ";
        std::string* d = justify(descr, 60, ' ', "\n// ", 0);
        msg += *d;
        delete d;
        msg += "\n//----";
    }
    dobj_help(smt);
    modinputlist::linkedlist* inlist =
        new_list_of_by(jwm.get_inputlist(), sm);
                    //jwm.get_inputlist()->getinputlist_for_module(sm);
    modinput* mi = inlist->goto_first();
    char spaces[20];
    for (int i = 0; i < 20; spaces[i++] = ' ');
    if (mi) {
        msg += "\n// inputs for ";
        msg += module::names::get(smt);
        int mxl = 0;
        std::string in;
        while(mi) { // get max len of input name
            in = input::names::get(mi->get_inputtype());
            int l = in.length();
            if (l > mxl) mxl = l;
            mi = inlist->goto_next();
        }
        mxl += 2;
        mi = inlist->goto_first();
        while(mi) {
            input::TYPE it = mi->get_inputtype();
            std::string in = input::names::get(it);
            msg += "\n    " + in;
            msg.append(spaces, mxl - in.length());
            msg += "modulename outputname";
            mi = inlist->goto_next();
            if (jwm.is_verbose()) {
                const char* descr = input::names::descr(it);
                msg += " // ";
                msg += descr;
            }
        }
    }
    delete inlist;
    modparamlist::linkedlist* parlist =
        new_list_of_by(jwm.get_paramlist(), sm->get_module_type());

    modparam* param = parlist->goto_first();
    if (param) {
        msg += "\n// parameters for ";
        msg += module::names::get(smt);
        int mxl = 0;
        std::string pn;
        while(param) {
            pn = param::names::get(param->get_paramtype());
            int l = pn.length();
            if (l > mxl) mxl = l;
            param = parlist->goto_next();
        }
        mxl += 2;
        param = parlist->goto_first();
        while(param) {
            param::TYPE pt = param->get_paramtype();
            pn = param::names::get(pt);
            msg += "\n    " + pn;
            msg.append(spaces, mxl - pn.length());
            iocat::TYPE ioc = param::names::category(pt);
            if (ioc == iocat::FIX_STR) {
                fixstrparam* fsp;
                fsp = jwm.get_fxsparamlist()->get_fix_str_param(pt);
                if (fsp)
                    msg += fsp->get_string_list();
                else
                    msg += "fixstringparam problem";
            }
            else
                msg += iocat::names::get(ioc);

            if (jwm.is_verbose()) {
                const char* descr = param::names::descr(pt);
                msg += " // ";
                msg += descr;
            }

            param = parlist->goto_next();
        }
    }
    delete parlist;

    modoutputlist::linkedlist* outlist =
        new_list_of_by(jwm.get_outputlist(), sm);

    modoutput* output = outlist->goto_first();
    if (output) {
        msg += "\n// outputs for ";
        msg += module::names::get(smt);
        while(output) {
            msg += "\n// ";
            output::TYPE ot = output->get_outputtype();
            msg += output::names::get(ot);
            output = outlist->goto_next();
            if (jwm.is_verbose()) {
                const char* descr = output::names::descr(ot);
                msg += " // ";
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

void cmdline::dobj_help(module::TYPE smt)
{
    moddobj* mdbj = jwm.get_moddobjlist()->get_first_of_type(smt);
    // forgot that some modules have more than one list.... so we do want
    // to get a moddobjlist and not just a moddobj from moddobjlist.
    if (!mdbj)
        return;
    msg += "\n// data objects for ";
    msg += module::names::get(smt);
    while(mdbj) {
        dobjdobjlist* mod_ddlist = mdbj->get_dobjdobjlist();
        dataobj::TYPE dt = mdbj->get_first_child();
        dobjdobjlist* ddlist =
            mod_ddlist->get_dobjdobjlist_for_dobjtype(dt);
        dobjdobj* dd = ddlist->goto_first();
        msg += "\n    ";
        msg += dataobj::names::get(dt);
        while(dd) {
            dataobj::TYPE sprogtype = dd->get_dobj_sprog();
            msg += "\n        ";
            msg += dataobj::names::get(sprogtype);
            delete jwm.get_dobjlist()->create_dobj(sprogtype);
            dobj_help_params(sprogtype, 2);
            dd = ddlist->goto_next();
        }
        msg += "\n    ";
        msg += dataobj::names::get(dt);
        delete ddlist;
        mdbj = jwm.get_moddobjlist()->get_next_of_type();
    }
    msg += "\n";
    return;
}

void cmdline::dobj_help()
{
    int n = data[DH_IX].par1;
    std::string dname = (n != 0 && n < opts_count) ? opts[n] : "";
    dataobj::TYPE dt = dataobj::names::type(dname.c_str());

    dobj* dbj = jwm.get_dobjlist()->create_dobj(dt);
    if (!dbj) {
        // incorrect dobj name or no name specified
        if (opts_count == 3)
            msg = "\nno data object available named " + dname;
        msg += "\navailable data object types are:\n\n";
        int count = 0;
        const char** dbjnames =
            dataobj::names::all_in_category(dataobj::CAT_DEF, &count);
        std::string* str = collumnize(dbjnames, count, 20, 2, true);
        msg += *str;
        delete str;
        delete [] dbjnames;
        return;
    }

    msg += "\n";
    msg += dataobj::names::get(dt);
    msg += "\nusername";

    const char* descr = dataobj::names::descr(dt);
    if (descr) {
        msg += "\n// ";
        std::string* d = justify(descr, 60, ' ', "\n// ", 0);
        msg += *d;
        delete d;
        msg += "\n//----";
    }

    dobj_help_params(dt, 1);

    dobjdobjlist* ddlist = jwm.get_topdobjlist()->get_first_of_type(dt);

    int p = 10;

    while(ddlist && p) {
        dobjdobj* dd = ddlist->goto_first();
        while(dd) {
            dataobj::TYPE sprogtype = dd->get_dobj_sprog();
            msg += "\n    ";
            msg += dataobj::names::get(sprogtype);

            descr = dataobj::names::descr(sprogtype);
            if (descr) {
                msg += " // ";
                std::string* d = justify(descr, 60, ' ', "\n    // ", 0);
                msg += *d;
                delete d;
            }

            dobjdobjlist::linkedlist* sddlist = 0;
            sddlist = ddlist->get_dobjdobjlist_for_dobjtype(sprogtype);
            dobjdobj* sdd = sddlist->goto_first();
            while(sdd) {
                dataobj::TYPE ssprogtype = sdd->get_dobj_sprog();
                msg += "\n        ";
                msg += dataobj::names::get(ssprogtype);
                delete jwm.get_dobjlist()->create_dobj(ssprogtype);
                dobj_help_params(ssprogtype, 3);
                sdd = sddlist->goto_next();
            }
            delete sddlist;
            msg += "\n    ";
            msg += dataobj::names::get(sprogtype);
            delete jwm.get_dobjlist()->create_dobj(sprogtype);
            dobj_help_params(sprogtype, 2);
            dd = ddlist->goto_next();
        }
        ddlist = jwm.get_topdobjlist()->get_next_of_type();
        p--;
    }
    msg += "\nusername";
    delete ddlist;
    delete dbj;
    return;
}

void cmdline::dobj_help_params(dataobj::TYPE dt, int level)
{
    char spaces[20];
    for (int i = 0; i < 20; spaces[i++] = ' ');

    dobjparamlist::linkedlist*
        dparlist = new_list_of_by(jwm.get_dparlist(), dt);

    if (!dparlist) {
        std::cout << "\nfailed to retrieve dobjparamlist for "
            << dataobj::names::get(dt);
        return;
    }

    // display of strings like "parameters for /dobjname/" is
    // somewhat problematic here, due to the fact many dobj
    // contain other dobj and continually informing the user
    // that here are parameters for them actually makes things
    // more confusing by adding extraneous visual information.

    dobjparam* dparam = dparlist->goto_first();
    std::string dn;
    int mxl = 0;
    while(dparam) {
        dn = param::names::get(dparam->get_partype());
        int l = dn.length();
        if (l > mxl) mxl = l;
        dparam = dparlist->goto_next();
    }
    dparam = dparlist->goto_first();
    while(dparam) {
        iocat::TYPE ioc = param::names::category(dparam->get_partype());
        msg += "\n";
        for (int i = 0; i < level; ++i)
            msg += "    ";
        param::TYPE pt = dparam->get_partype();
        dn = param::names::get(pt);
        msg += dn;
        msg.append(spaces, mxl - dn.length() + 2);
        if (ioc == iocat::FIX_STR) {
            fixstrparam* fsp;
            fsp = jwm.get_fxsparamlist()->get_fix_str_param(pt);
            msg += fsp->get_string_list();
        }
        else
            msg += iocat::names::get(ioc);

        if (jwm.is_verbose()) {
            const char* descr = param::names::descr(pt);
            msg += " // ";
            msg += descr;
        }

        dparam = dparlist->goto_next();
    }
    delete dparlist;
    return;
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
        for (int i = input::ERR_TYPE + 1;
                 i < input::LAST_TYPE; i++)
            innames[i - 1] = input::names::get((input::TYPE)i);
        std::string* str = collumnize(innames, incount, 40, 2, true);
        msg += *str;
        delete str;
        delete [] innames;
        return;
    }
    msg = "";
    iocat::TYPE incat = input::names::category(intype);
    if (data[WC_IX].par1 > 0) {
        // must create all modules to gain access to compatible outputs
        jwmsynth jwm_synth;
        if (!jwm_synth.is_valid() || !jwm_synth.generate_synth()) {
            msg =
                "\nproblems reading synthfile:\n" +
                msg + "\nproceeding anyway";
        }
    }
    else {
        // only create modules within a .wc file to access outputs
        synthmod* sm;
        int i;
        for (i = module::ERR_TYPE + 1;
                i < module::LAST_TYPE; i++)
        {
            if (i != module::NONEZERO) {
                sm = jwm.get_modlist()->create_module(
                    (module::TYPE) i,
                        module::names::get(
                            (module::TYPE)i));
                if (!sm) {
                    msg += "\nnot enough memory to process request to"
                        "create synthmodule type: ";
                    msg += module::names::get(
                            (module::TYPE)i);
                    return;
                }
                jwm.get_modlist()->add_module(sm);
            }
        }
    }
    msg += "\nOutputs matching category of input ";
    msg += input::names::get(intype);
    msg += " are:\n";
    // now get the outputs matching category of input
    modoutputlist* outputs =
        jwm.get_outputlist()->list_of_category(incat);
    modoutput* output = outputs->goto_first();
    std::string mn;
    int mxl = 0;
    while(output) {
        mn = output->get_synthmodule()->get_username();
        int l = mn.length();
        if (l > mxl) mxl = l;
        output = outputs->goto_next();
    }
    mxl += 2;
    output = outputs->goto_first();
    char spaces[20];
    for (int i = 0; i < 20; spaces[i++] = ' ');
    while(output)
    { // this if statement prevents from displaying off off
        // now it only displays one off - less confusing.
        if (output->get_moduletype() != module::NONEZERO) {
            mn = output->get_synthmodule()->get_username();
            msg += mn;
            msg.append(spaces, mxl - mn.length());
        }
        msg += output::names::get(
                                        output->get_outputtype());
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
    conv << (double)wavfile.get_length() / jwm.samplerate();
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
    jwm.samplerate(atol(opts[data[FI_IX].par2]));
    if (frequency == 0) {
        msg = "\nInvalid frequency";
        return;
    }
    if (jwm.samplerate() == 0) {
        msg = "\nInvalid sample rate";
        return;
    }
    std::ostringstream conv;
    conv << "\nFrequency " << frequency;
    conv << " at samplerate " << jwm.samplerate();
    conv << "\n\tphase_step " << freq_to_step(frequency);
    conv << "\n\tsamples  " << freq_to_samples(frequency);
    msg = conv.str();
    return;
}
void cmdline::header()
{
    msg = "\nwcnt .wc file header (and footer) is:\n\t";
    msg += wcnt_id;
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

