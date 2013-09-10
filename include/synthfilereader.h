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

//    void set_module_action(ACTION ma){ mod_action = ma;}
//    void set_dobj_action(ACTION da){ dobj_action = da;}
//    char const* get_wc_file_name(){ return wc_filename;}
//    ACTION get_module_action(){ return mod_action;}
//    ACTION get_dobj_action(){ return dobj_action;}

    modnamedobj* add_modname(modnamedobj*);
    modnamedobj* goto_first_modname() {
        return modname = (modnamedobj*)
            (mname_item = modnamelist->goto_first())->get_data();
    }
    modnamedobj* goto_last_modname() {
        return modname = (modnamedobj*)
            (mname_item = modnamelist->goto_last())->get_data();
    }
    modnamedobj* goto_next_modname() {
        return modname = (modnamedobj*)
            (mname_item = modnamelist->goto_next())->get_data();
    }
    modnamedobj* goto_prev_modname() {
        return modname = (modnamedobj*)
            (mname_item = modnamelist->goto_prev())->get_data();
    }
    dobjnamedobj* add_dobjname(dobjnamedobj*);
    dobjnamedobj* goto_first_dobjname() {
        return dobjname = (dobjnamedobj*)
            (dname_item = dobjnamelist->goto_first())->get_data();
    }
    dobjnamedobj* goto_last_dobjname() {
        return dobjname = (dobjnamedobj*)
            (dname_item = dobjnamelist->goto_last())->get_data();
    }
    dobjnamedobj* goto_next_dobjname() {
        return dobjname = (dobjnamedobj*)
            (dname_item = dobjnamelist->goto_next())->get_data();
    }
    dobjnamedobj* goto_prev_dobjname() {
        return dobjname = (dobjnamedobj*)
            (dname_item = dobjnamelist->goto_prev())->get_data();
    }

    bool read_and_create();

    std::string get_wc_error_msg(){ return *wc_err_msg;}

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
    linkedlist* modnamelist;
    linkedlist* dobjnamelist;
    ll_item* mname_item;
    ll_item* dname_item;
    modnamedobj* modname;
    dobjnamedobj* dobjname;

    // working
    WC_FILE_TYPE wc_file_type;
    FILE_STATUS filestatus;
    std::ifstream* synthfile;
    std::string* buff;
    std::string* command; // used to force a command into buff
    std::string* synthheader;
    std::string* wc_err_msg;
    std::ostringstream* conv;
    bool inc_current;  // include current? - bodgish
//--- private member methods ---//
    FILE_STATUS open_file();
    bool read_header(unsigned long* samplerate, double* bpm, 
     short* beatspermeasure, short* beatvalue);
    std::string const* read_command();
    synthmod* const read_synthmodule(std::string const* command);
    dobj* const read_dobj(std::string const* command);
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
    bool eff_ing_header_bodge(unsigned long *samplerate, double *bpm,
     short *beatspermeasure, short *beatvalue);
    void create_params();
    static bool done_params;
};

#endif
