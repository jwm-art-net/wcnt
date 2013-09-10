#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include <iostream>

#include "synthmodnames.h"
#include "stockerrs.h"

// when you want to test a new module you've written without all the extra
// data like module lists, output lists, input lists, parameter lists, and
// connector lists:  define BARE_MODULES - to get just that, bare modules
// with only enough functionality for them to run(), but you'll have to
// hard code the outputs to inputs, etc, but that's ok when you just want
// to test the thing.

#ifndef BARE_MODULES
#include "inputnames.h"
#include "outputnames.h"
#include "paramnames.h"
#include "moddobjlist.h"
#endif

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

extern short sm_beats_per_minute;
extern short sm_beats_per_measure;
extern short sm_beat_value;

// forward definitions
#ifndef BARE_MODULES
class synthmodlist;
class modinputlist;
class modoutputlist;
class modparamlist;
class connectorlist;
#endif

// although many synthmodules do not need access to much of the
// register_xxx stuff, you can use synthmod to access them. Anything that
// needs access to synthmodlist or connector list will need access to
// synthmod anyhow.  it makes life easier for passing stuff around the
// user interface code.

class synthmod
{
public:
    synthmod(synthmodnames::SYNTH_MOD_TYPE, short id, char const* uname);
    virtual ~synthmod();
    synthmodnames::SYNTH_MOD_TYPE get_module_type(){ return module_type;}
    short get_number_id() { return number_id;}
    char const* get_username(){ return username;}
    // virtuals
    virtual void run() = 0;// 1st choice method for pure abstract virtual.
    virtual void init(){}; // run() is 96% always needed...
    // validation
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    void invalidate(){valid = false;}
    bool is_valid() { return valid;}
#ifndef BARE_MODULES
    // more virtuals
    virtual void const* set_in(inputnames::IN_TYPE, void const*);
    virtual void const* get_out(outputnames::OUT_TYPE);
    virtual bool set_param(paramnames::PAR_TYPE, void const*);
    virtual void const* get_param(paramnames::PAR_TYPE);
    virtual dobj* add_dobj(dobj*);
    // statics - created and registered from jwmsynth constructor
    static void register_err_msg(string* e){ err_msg = e;}
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
#endif
protected:
    static string* err_msg;
private:
    synthmodnames::SYNTH_MOD_TYPE module_type;
    short number_id;
    char* username;
    bool valid;
#ifndef BARE_MODULES
    static char* path; /* path (if any) to the synthfile.wc */
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
#endif
};

#endif
