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
#include "boolfuncobj.h"

/*
//  synthmod - pure abstract base class for jwm synth modules.
*/

class dobj;

class synthmod
{
 public:

    enum SM_FLAGS
    {
        SM_DEFAULT,
        SM_VALID =              0x0001,
        SM_EMPTY_RUN =          0x0002,
        SM_UNGROUPABLE =        0x0004,
        SM_UNDUPLICABLE =       0x0008,
        SM_HAS_OUT_OUTPUT =     0x0010,
        SM_HAS_STEREO_OUTPUT =  0x0020,
        SM_HAS_OUT_TRIG =       0x0040
    };

    synthmod(   synthmodnames::SYNTH_MOD_TYPE,
                char const* const uname,
                int _flags_);

    virtual ~synthmod();

    synthmodnames::SYNTH_MOD_TYPE get_module_type() const
        { return module_type; }
    synthmodnames::SYNTH_MOD_TYPE get_module_type()
        { return module_type; }

    const char* const get_username() const
        { return username; }

    const char* const get_group_name() const
        {
            return
                ((flags & SM_UNGROUPABLE)
                    ? 0
                    : get_groupname(username));
        }

    void set_group_name(const char* const);

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* input/output/param access */
    virtual void const* set_in(inputnames::IN_TYPE, void const*);
    virtual bool        set_param(paramnames::PAR_TYPE, void const*);
    virtual void const* get_out(outputnames::OUT_TYPE) const;
    virtual void const* get_in(inputnames::IN_TYPE) const;
    virtual void const* get_param(paramnames::PAR_TYPE) const;

    virtual dobj*       add_dobj(dobj*);

    enum DUP_IO
    {
        AUTO_CONNECT,
        NO_CONNECT
    };

    virtual synthmod* duplicate_module(const char* const uname, DUP_IO);

    // validation
    virtual stockerrs::ERR_TYPE validate()
        { return stockerrs::ERR_NO_ERROR; }

    #ifdef IO_DEBUG
    bool check_inputs();
    #endif

    // statics - created and registered from jwmsynth constructor
    static void register_error_msg(std::string* e)  { err_msg = e;   }
    static void clear_error_msg()                   { *err_msg = ""; }
    static std::string const* get_error_msg()       { return err_msg;}

    // should only be used by jwmsynth::run()
    static const STATUS* const get_abort_status()
        { return &abort_status;}

    bool operator()(synthmodnames::SYNTH_MOD_TYPE & smt) const
        { return module_type == smt; }

    bool operator()(name & n) const { return n(username); }

    bool operator()(groupname & n) const {
        if (flags & SM_UNGROUPABLE)
            return false;
        const char* grpname = get_groupname(username);
        bool retv = n(grpname);
        delete [] grpname;
        return retv;
    }
    int flag(SM_FLAGS _flags) { return _flags & flags; }
    int operator()(SM_FLAGS & _flags) { return _flags & flags; }

    #ifdef MOD_STATS
    STATS_FUNCS
    #endif

 protected:
    void invalidate()  { flags ^= SM_VALID; }
    void force_abort() { abort_status = ON; }
    void duplicate_inputs_to(synthmod*);
    void duplicate_params_to(synthmod*);

    static std::string* err_msg;

 private:
    synthmodnames::SYNTH_MOD_TYPE   module_type;
    char*                           username;
    int                             flags;
    static STATUS                   abort_status;

//  not to be used:
    synthmod();
    synthmod(const synthmod &);

    #ifdef MOD_STATS
    STATS_VARS
    #endif
};

#endif