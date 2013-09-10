#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include <iostream>

#include "synthmodnames.h"
#include "stockerrs.h"
#include "inputnames.h"
#include "outputnames.h"
#include "paramnames.h"
#include "moddobjlist.h"
#include "fxsparamlist.h"

// Default settings for audio output
extern short audio_channels;
extern unsigned long audio_samplerate;
extern short audio_bitrate;
extern short audio_ysize;

// Default settings for music tempo
#ifndef MIN_BPM
#define MIN_BPM 20
#define MAX_BPM 1000
#endif

extern double sm_beats_per_minute;
extern short  sm_beats_per_measure;
extern short  sm_beat_value;

// forward definitions
class synthmodlist;
class modinputlist;
class modoutputlist;
class modparamlist;
class connectorlist;

// although many synthmodules do not need access to much of the
// register_xxx stuff, you can use synthmod to access them. Anything that
// needs access to synthmodlist or connector list will need access to
// synthmod anyhow.  it makes life easier for passing stuff around the
// user interface code.

// =--=--=--=
//  wcnt-1.25
// =--=--=--=
// added get_group_name() method to return the name of the group, or null,
// if it is not part of a group. the group name is part of the username,
// seperated by a dot '.'. set_group_name will change the group name if
// one is already set, or will add a group name if not. the group name
// must be delete[]ed after you're done with it.

// new method bool check_inputs() checks that inputs are set. that is, it
// checks that the _actual_ inputs of a module are set, without relying on // information contained in connectlist. only problem is I've now got
// to create the virtual function get_in in every module which is a
// minor nuisance no brainer copy and paste and edit affair.

class synthmod
{
public:
    enum DUP_IO{AUTO_CONNECT, NO_CONNECT};
    synthmod(synthmodnames::SYNTH_MOD_TYPE, short id, char const* uname);
    virtual ~synthmod();
    synthmodnames::SYNTH_MOD_TYPE get_module_type(){ return module_type;}
    short get_number_id() { return number_id;}
    char const* get_username(){ return username;}
    void set_group_name(const char*);
    char* get_group_name();
    // virtuals
    virtual void run() = 0;// 1st choice method for pure abstract virtual.
    virtual void init(){}; // run() is 99.99999% always needed...
    // validation
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    void invalidate(){valid = false;}
    bool is_valid() { return valid;}
    // more virtuals
    virtual void const* set_in(inputnames::IN_TYPE, void const*);
    virtual void const* get_in(inputnames::IN_TYPE);
    virtual void const* get_out(outputnames::OUT_TYPE);
    virtual bool set_param(paramnames::PAR_TYPE, void const*);
    virtual void const* get_param(paramnames::PAR_TYPE);
    virtual synthmod* duplicate_module(const char* uname, DUP_IO);
// definition just to trip up old style functions still kicking about:
    virtual void* duplicate_module(const char* uname){return 0;}
    virtual dobj* add_dobj(dobj*);
    bool check_inputs();
    // statics - created and registered from jwmsynth constructor
    static void set_verbose(){verbose = true;}
    static bool get_verbose(){return verbose;}
    static void clear_error_msg(){ *err_msg = "";}
    static void register_error_msg(string* e){ err_msg = e;}
    static void register_path(char* p){ path = p;}
    static void register_iocatnames(iocat_names* in){iocatnames = in;}
    static void register_modnames(synthmodnames* s) {modnames = s;}
    static void register_inputnames(inputnames* i) {innames = i;}
    static void register_outputnames(outputnames* o) {outnames = o;}
    static void register_paramnames(paramnames* p) {parnames = p;}
    static void register_modlist(synthmodlist* s){ modlist = s;}
    static void register_inputlist(modinputlist* m) {inputslist = m;}
    static void register_outputlist(modoutputlist* m) {outputslist = m;}
    static void register_paramlist(modparamlist* m){paramlist = m;}
    static void register_connectlist(connectorlist* c){ connectlist = c;}
    static void register_moddobjlist(moddobjlist* m){ mdobjlist = m;}
    static void register_fxsparamlist(fxsparamlist* fl){fxsparlist = fl;}
    static string const* get_error_msg(){ return err_msg;}
    static char const* get_path(){ return path;}
    static iocat_names* get_iocatnames(){ return iocatnames;}
    static synthmodnames* get_modnames(){ return modnames;}
    static inputnames* get_inputnames(){ return innames;}
    static outputnames* get_outputnames(){ return outnames;}
    static paramnames* get_paramnames(){ return parnames;}
    static synthmodlist* get_modlist(){ return modlist;}
    static modinputlist* get_inputlist(){ return inputslist;}
    static modoutputlist* get_outputlist(){ return outputslist;}
    static modparamlist* get_paramlist(){ return paramlist;}
    static connectorlist* get_connectlist(){ return connectlist;}
    static moddobjlist* get_moddobjlist(){ return mdobjlist;}
    static fxsparamlist* get_fxsparamlist(){ return fxsparlist;}
    static STATUS const* get_abort_status(){ return &abort_status;}
    #ifdef SHOW_MOD_COUNT
    static long get_created_count(){ return mods_created_count;}
    static long get_destroyed_count(){ return mods_destroyed_count;}
    static long get_max_count(){ return mods_max_count;}
    #endif
protected:
    static string* err_msg;
    void force_abort(){ abort_status = ON; } // abort now!
    void duplicate_inputs_to(synthmod*);
    void duplicate_params_to(synthmod*);
private:
    synthmodnames::SYNTH_MOD_TYPE module_type;
    short number_id;
    char* username;
    bool valid;
    static char* path; /* to synthfile.wc from cmdline */
    static synthmodnames* modnames;
    static iocat_names* iocatnames;
    static inputnames* innames;
    static outputnames* outnames;
    static paramnames* parnames;
    static synthmodlist* modlist;
    static modinputlist* inputslist;
    static modoutputlist* outputslist;
    static modparamlist* paramlist;
    static connectorlist* connectlist;
    static moddobjlist* mdobjlist;
    static fxsparamlist* fxsparlist;
    static bool verbose;
    static STATUS abort_status;
    #ifdef SHOW_MOD_COUNT
    static long mods_created_count;
    static long mods_destroyed_count;
    static long mods_count;
    static long mods_max_count;
    #endif

};

#endif
