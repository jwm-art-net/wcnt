#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include "types.h"
#include "synthmodnames.h"
#include "outputnames.h"
#include "inputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "groupnames.h"
#include "boolfuncobj.h"
#include "textstuff.h"

#ifdef DEBUG
#include <cstdio>
#endif

#include "dobjnames.h"

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
                const char* const uname,
                int _flags_);

    virtual ~synthmod();

    synthmodnames::SYNTH_MOD_TYPE get_module_type() const
        { return module_type; }

    const char* get_username() const
        { return username; }

    const char* get_group_name() const {
            return ((flags & SM_UNGROUPABLE)
                        ? 0 : get_groupname(username));
        }

    void set_group_name(const char* const);

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* input/output/param access */
    virtual const void* set_in(inputnames::IN_TYPE, const void*);
    virtual bool        set_param(paramnames::PAR_TYPE, const void*);
    virtual const void* get_out(outputnames::OUT_TYPE) const;
    virtual const void* get_in(inputnames::IN_TYPE) const;
    virtual const void* get_param(paramnames::PAR_TYPE) const;

    virtual dobj*       add_dobj(dobj*);

    enum DUP_IO
    {
        AUTO_CONNECT,
        NO_CONNECT
    };

    /* duplicate module can't be const due to my linked_list impl. */
    virtual synthmod* duplicate_module(const char* const uname, DUP_IO);

    // validation
    virtual stockerrs::ERR_TYPE validate()
        { return stockerrs::ERR_NO_ERROR; }

    #ifdef DEBUG
    bool check_inputs();
    #endif

    static const char* get_error_msg()  { return err_msg; }

    // should only be used by jwmsynth::run()
    static const STATUS* get_abort_status()
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
    int flag(SM_FLAGS _flags) const { return _flags & flags; }
    int operator()(SM_FLAGS & _flags) const { return _flags & flags; }

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 protected:
    void invalidate()           { flags &=~ SM_VALID; }
    static void force_abort()   { abort_status = ON; }
    void duplicate_inputs_to(synthmod*) const;
    void duplicate_params_to(synthmod*) const;

    /*  first instance initializations (ie parameter and data object
        registration) should be performed in the init_first method of
        derived objects.
     */
    virtual void init_first();
    bool done_first() const;
    void register_param(paramnames::PAR_TYPE);
    void register_param(paramnames::PAR_TYPE, const char* fixed_string);
    void register_moddobj(dobjnames::DOBJ_TYPE parent,
                                                dobjnames::DOBJ_TYPE sprog);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    void register_input(inputnames::IN_TYPE);
    void register_output(outputnames::OUT_TYPE);

    static char err_msg[STRBUFLEN];

 private:
    synthmodnames::SYNTH_MOD_TYPE   module_type;
    char*                           username;
    int                             flags;
    static STATUS                   abort_status;

//  not to be used:
    synthmod();
    synthmod(const synthmod &);

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[synthmodnames::LAST];
};

#ifdef DEBUG
#define sm_err(fmt, ... )                              \
{                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(synthmod::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
}
#else
#define sm_err(fmt, ... ) \
    cfmt(synthmod::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
#endif



#endif
