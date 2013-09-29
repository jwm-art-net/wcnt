#ifndef CMDLINE_H
#define CMDLINE_H

#include "synthmodnames.h"
#include "dobjnames.h"

#include <string>

/*
// cmdline
// for processing the command line.
//
// create, and then scan().
// scan() returns true if a .wc file is specified for processing.
//        returns false if incorrect commandline options specified
//                      or commandline help required.
//
// when scan() returns true, set_jwm_globals should be called.
//
// if scan() returns false, calling display_message() will display
// to user what went wrong, or the help requested. set_jwm_globals
// will be called by display_message() (or further down the line)
// if required for dealing with requested help.
*/

namespace dbjdefs
{
const int DOBJ_LAST = dobjnames::DOBJ_SYNTHMOD - dobjnames::DOBJ_DEFS;
dobjnames::DOBJ_TYPE get_dobj_def_type(int i);
}

class cmdline
{
 public:

    enum CMD_OPTS
    {
        WC_FILE         = 0x0000,
        VERBOSE         = 0x0001,
        DONT_RUN        = 0x0002,
        NO_TITLE        = 0x0004,
        MOD_HELP        = 0x0010,
        IN_HELP         = 0x0020,
        DOBJ_HELP       = 0x0040,
        SAMPLE_INFO     = 0x0080,
        NOTE_INFO       = 0x0100,
        FREQ_INFO       = 0x0200,
        HEADER          = 0x0400,
        HELP            = 0x0800,
        LONGHELP        = 0x1000,
        ABOUT           = 0x2000
    };

    cmdline(const int argc, const char **const argv);
    ~cmdline();

    bool scan();

    bool is_good_opts() const { return good_opts; }

    bool set_jwm_globals();

    const char* get_message() const;

 private:
    int const           opts_count;
    const char** const  opts;
    short               opts_flags;
    bool good_opts;
    std::string msg;
    enum
    {
        WC_IX,
        V_IX,
        DONT_RUN_IX,
        NO_TITLE_IX,
        MH_IX,
        IH_IX,
        DH_IX,
        SI_IX,
        NI_IX,
        FI_IX,
        HEADER_IX,
        H_IX,
        LH_IX,
        ABOUT_IX,
        OPTS_COUNT
    };
    struct cmd_opts_data
    {
        CMD_OPTS type;
        const char* const olong;
        const char* const oshort;
        const char* const odisplay;
        short par1;
        short par2;
        short max_args;
        const char* const helptext;
        void (cmdline::*helpfunc)();
    };
    static cmd_opts_data data[OPTS_COUNT];
    struct mod_help_data
    {
        module::TYPE type;
        const char* const help;
    };
    static const mod_help_data modhelp_data[module::LAST_TYPE];

    struct dobj_help_data
    {
        dobjnames::DOBJ_TYPE type;
        const char* const help;
    };
    static const dobj_help_data dobjhelp_data[dbjdefs::DOBJ_LAST];

    void invalid_args();

    void module_help();
    void input_help();
    void input_help_with_wcfile();
    void dobj_help();
    void sample_info();
    void note_info();
    void freq_info();
    void header();
    void help();
    void about();

    void dobj_help(module::TYPE);
    void dobj_help_params(dobjnames::DOBJ_TYPE);
};

#endif
