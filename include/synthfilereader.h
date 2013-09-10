#ifndef SYNTHFILEREADER_H
#define SYNTHFILEREADER_H

#ifndef BARE_MODULES

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "modules.h"
#include "connectorlist.h"

class synthfilereader
{
public:
    enum FILE_STATUS
    {
        NOT_FOUND = -1,
        INVALID_HEADER = 0,
        FILE_OPEN = 1,
        FILE_READY = 2
    };
    enum GEN_STATUS
    {
        MASSIVE_ERROR = -2,
        HALTED = -1,
        OK = 0,
        COMPLETE = 1
    };

    synthfilereader();
    ~synthfilereader();
    FILE_STATUS open_file(char* synthfilename, string wcnt_id);
    bool read_header(unsigned long* samplerate, short* bpm, 
     short* beatspermease, short* beatvalue);
    string const* read_command();
    synthmod* const read_synthmodule(string const* command);
    dobj* const read_dobj(string const* command);
    char* get_filename(){ return filename;}
    GEN_STATUS get_generation_status(){ return genstatus;}
    string get_error_msg(){ return *err_msg;}
    void set_verbose(){ verbose = true;}
    bool is_verbose(){ return (verbose == true);}
    void set_stockerrs(stockerrs* se){ stock_errs = se;}

private:
    char* filename;
    FILE_STATUS filestatus;
    ifstream* synthfile;
    stockerrs* stock_errs;
    string* buff;
    string* synthheader;
    string* err_msg;
    ostringstream* conv;
    GEN_STATUS genstatus;
    bool verbose;
    // file reading methods
    bool skip_remarks();
    // methods to read various parts of a module
    bool read_dobjs(synthmod*);
    bool read_inputs(synthmod*);
    bool read_params(synthmod*);
    // method to read sub-parts of standalone dobjs
    bool read_dobjs(dobj*);
    // method to read parameters of dobjs
    bool read_dobj_params(dobj*);
    // method to read data for all param types
    void* read_iocat_value(IOCAT);
    // method to delete the tempory data created by above
    void destroy_iocat_data(IOCAT, void*);
    // method for ......
    bool eff_ing_header_bodge(unsigned long *samplerate, short *bpm,
     short *beatspermeasure, short *beatvalue);
};

#endif
#endif
