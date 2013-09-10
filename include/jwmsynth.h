#ifndef JWMSYNTH
#define JWMSYNTH

#include <iostream>

#include "synthmodulelist.h"
#include "modinputslist.h"
#include "modoutputslist.h"
#include "connectorlist.h"
#include "synthfilereader.h"
#include "dobjlist.h"
#include "topdobjlist.h"

class jwmsynth
{
public:
    jwmsynth(int const argc, char **const argv);
    ~jwmsynth();
    const char* get_version(){ return version;}
    // must call these functions  in order stated
    bool is_valid(){ return valid;} // 1
    bool scan_cl_options();         // 2
    bool generate_synth();          // 3
    bool connect_synth();           // 4
    bool execute_synth();           // 5
    string get_error_msg(){ return err_msg;}
private:
    char* version;
    char* path;
    string wcnt_id;
//--- dobj and synthmod static member variables ---//
    string* synthmod_err_msg;
    string* dobj_err_msg;
    iocat_names* iocatnames;
    synthmodnames* modnames;
    inputnames* innames;
    outputnames* outnames;
    paramnames* parnames;
    synthmodlist* synthmodslist;
    modinputlist* inputlist;
    modoutputlist* outputlist;
    modparamlist* paramlist;
    connectorlist* connectlist;
    synthfilereader* synthfile;
    moddobjlist* mdobjlist;
    dobjnames* dobj_names;
    dobjlist* dobj_list;
    dobjparamlist* dobjparam_list;
    topdobjlist* top_dobjlist;
    fxsparamlist* fxsparlist;
//--- validation and error messages ---//
    bool valid;
    string err_msg;
    stockerrs* stock_errs;
//--- when to stop ---//
    short exit_bar;
    const STATUS* in_bar_trig;
    const short* in_bar;
//--- command line processing ---//
    int const opts_count;
    char** const opts;
    int wcfile_opt;
//--- private methods ---//
    void module_help();
    void dobj_help();
    void dobj_help(synthmodnames::SYNTH_MOD_TYPE);
    void input_help();
    void input_help_with_wcfile();
    void sample_info();
    void dobj_help_params(dobjnames::DOBJ_TYPE);
    void note_help();
    void freq_help();
};

#endif
