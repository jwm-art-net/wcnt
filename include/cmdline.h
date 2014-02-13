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
* nb this is pretty messed up...
*/



class cmdline
{
 public:

    enum SWITCHES {
        VERBOSE =       0x0001,
        NO_RUN =        0x0002,
        NO_TITLE =      0x0004,
        NO_PROGRESS =   0x0008,
        MINIMAL =       0x0010,
        // magic value;-
        SW_COUNT =      5
    };

    enum COMMANDS {
        RUN,
        HELP,
        MOD_HELP,
        DOBJ_HELP,
        INPUT_HELP,
        LADSPA_HELP,
        SAMPLE_INFO,
        NOTE_INFO,
        FREQ_INFO,
        HEADER,
        ABOUT,
        CMD_COUNT
    };

    cmdline(int argc, char** argv);
    ~cmdline();

    bool scan();

    bool show_help();

    bool set_jwm_globals();

    const char* get_message() const;

 private:
    int opt_count;
    char** opts;
    int    switch_flags;

    typedef struct switches {
        int value;
        const char* const _long;
        const char* const _short;
    } switches;

    static switches swdata[SW_COUNT];

    typedef struct commands {
        int id;
        const char* const _long;
        const char* const _short;
        int min_args;
        int max_args;
        int wc_opt;
        int valid_flags;
    } commands;

    static commands cmddata[CMD_COUNT];

    typedef struct textuals {
        int id;
        const char* const _long;
        const char* const _short;
    } textuals;

    static textuals texts[CMD_COUNT];

    int ladspa_lib;
    int ladspa_label;
    const char* wc_file;

    std::string msg;

    void invalid_args();

    void help();
    void mod_help();
    void dobj_help();
    void input_help();
    void ladspa_help();
    void sample_info();
    void note_info();
    void freq_info();
    void header();
    void about();

    void module_help_list_all();
    void input_help_with_wcfile();

    void dobj_help(dobj::TYPE parent, dobj::TYPE child, int indent_level);
    void dobj_help_items(dobj::TYPE, int indent_level);
};

#endif
