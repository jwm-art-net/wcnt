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

    void set_wc_filename(const char* filename);

    bool read_and_create();

    const char* get_wc_error_msg() const { return wc_err_msg; }

    virtual errors::TYPE validate();
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;
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

    char wc_err_msg[STRBUFLEN];


    bool inc_current;  // include current? - bodgish

//--- private member methods ---//

    FILE_STATUS open_file();
    bool        read_header(samp_t* samplerate);
    const char* read_command();
    void        print_msg();
    synthmod*   read_synthmodule(const char* command);
    dobj*       read_dobj(const char* command);

    bool include_mod(const char* name);
    bool include_dbj(const char* name);
    bool read_and_create_synthmod(const char* com);
    bool read_and_create_dobj(const char* com);
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
    bool eff_ing_header_bodge(samp_t *samplerate);
    void init_first();
    
};

#endif
