#ifndef SYNTHFILEREADER_H
#define SYNTHFILEREADER_H

#include <sstream>

#include "dobj.h"
#include "linkedlist.h"
#include "modnamedobj.h"
#include "dobjnamedobj.h"

/*
//  synthfilereader(FILE_TYPE) constructor should be only called
//  by jwmsynth with synthfilereader(WC_MAIN_FILE).
//  a user created synthfilereader should call default constructor
//
//  there are three member methods commented out.  they are used
//  to transmit excluded modules and data objects to a synthfilereader
//  read by this one. I'm unsure of it's usefullness, and think it may
//  just confuse matters...  note that the transmit_.. method was not
//  actually used anywhere.
*/

class synthmod;

class synthfilereader : public dobj
{
public:
    enum FILE_STATUS
    {
        NOT_FOUND = -1,
        INVALID_HEADER = 0,
        FILE_OPEN = 1,
        FILE_READY = 2
    };
    enum WC_FILE_TYPE
    {
        WC_MAIN_FILE,
        WC_INCLUDE_FILE
    };
    enum ACTION
    {
        WC_INCLUDE, WC_EXCLUDE
    };

    synthfilereader();
    synthfilereader(WC_FILE_TYPE);
    ~synthfilereader();

    void set_wc_filename(char const* filename);

    bool read_and_create();

    const char* get_wc_error_msg() { return wc_err_msg; }

    virtual stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE pt) const;
    dobj const* add_dobj(dobj*);

private:

    // params
    char* wc_filename;
    ACTION mod_action;
    ACTION dobj_action;

    // lists
    linked_list<modnamedobj>*  modnamelist;
    linked_list<dobjnamedobj>* dobjnamelist;

    // working
    WC_FILE_TYPE wc_file_type;
    FILE_STATUS filestatus;
    std::ifstream* synthfile;
    std::string* buff;
    std::string* command; // used to force a command into buff
    std::string* synthheader;

    /*std::string* wc_err_msg;*/
    char wc_err_msg[STRBUFLEN];


    bool inc_current;  // include current? - bodgish

//--- private member methods ---//

    FILE_STATUS open_file();
    bool read_header(unsigned long* samplerate);
    std::string const* read_command();
    synthmod*   read_synthmodule(std::string const* command);
    dobj*       read_dobj(std::string const* command);
    bool include_mod(const char* name);
    bool include_dbj(const char* name);
    bool read_and_create_synthmod(std::string const* com);
    bool read_and_create_dobj(std::string const* com);
    bool skip_remarks();
    // methods to read various parts of a module
    bool read_dobjs(synthmod*);
    bool read_inputs(synthmod*);
    bool read_params(synthmod*);
    // method to read sub-parts of standalone dobjs
    bool read_dobjs(dobj*);
    // method to read parameters of dobjs
    bool read_dobj_params(dobj*, const char* endterm);
    const std::string*
        read_string_list_param(const char* enda, const char* endb);
    // method for ......
    bool eff_ing_header_bodge(unsigned long *samplerate);
    void create_params();
    static bool done_params;
};

#endif
