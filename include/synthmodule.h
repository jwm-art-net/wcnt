#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include <string>

#include "types.h"
#include "synthmodnames.h"
#include "outputnames.h"
#include "inputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "groupnames.h"

/*
//  synthmod - pure abstract base class for jwm synth modules.
//  
//  have removed global static data (which was created in jwmsynth)
//  wanted to tidy this class declaration up a bit.
//
//  first attempt to move the static data out resulted in segfaults
//  as modules were destroyed... working on it --jwm8/DEC/2007
//
*/

class dobj;

class synthmod
{
 public:

    synthmod(synthmodnames::SYNTH_MOD_TYPE, char const* const uname);
    virtual ~synthmod();

    synthmodnames::SYNTH_MOD_TYPE get_module_type() const
        { return module_type; }
    const char* const get_username() const
        { return username; }
    const char* const get_group_name() const
        { return get_groupname(username); }

    void set_group_name(const char* const);

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* input/output/param access */
    virtual void const* get_out(outputnames::OUT_TYPE) const;
    virtual void const* set_in(inputnames::IN_TYPE, void const*);
    virtual void const* get_in(inputnames::IN_TYPE) const;
    virtual bool        set_param(paramnames::PAR_TYPE, void const*);
    virtual void const* get_param(paramnames::PAR_TYPE) const;
    virtual dobj*       add_dobj(dobj*);
    enum DUP_IO
    {
        AUTO_CONNECT,
        NO_CONNECT
    };
    virtual synthmod*   duplicate_module(const char* const uname, DUP_IO);

    /* validation */
    virtual stockerrs::ERR_TYPE validate()
        { return stockerrs::ERR_NO_ERROR; }
    void invalidate() { valid = false; }
    bool is_valid() const { return valid;  }

    bool check_inputs();

    // statics - created and registered from jwmsynth constructor
    static void clear_error_msg()                   { *err_msg = "";}
    static void register_error_msg(std::string* e)  { err_msg = e;}
    static std::string const* get_error_msg()       { return err_msg;}

    /* currently only peak_detector initiates an abort: */
    static const STATUS* const get_abort_status()
        { return &abort_status;}

#ifdef MOD_STATS
STATS_FUNCS
#endif

 protected:
    static std::string* err_msg;
    void force_abort() { abort_status = ON; } /* abort now! */
    void duplicate_inputs_to(synthmod*);
    void duplicate_params_to(synthmod*);

 private:
    synthmodnames::SYNTH_MOD_TYPE module_type;
    char* username;
    bool valid;
    static STATUS abort_status;

/*
//  these two constructors should not be used:
*/
    synthmod();
    synthmod(const synthmod &);

#ifdef MOD_STATS
STATS_VARS
#endif

};

#endif
