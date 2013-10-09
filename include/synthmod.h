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
class synthmod;

class smod
{
 public:
    smod(synthmod* inheritor);

    virtual ~smod(){};

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* input/output/param access */
    virtual const void* set_in(input::TYPE, const void*);
    virtual const void* get_in(input::TYPE) const;
    virtual bool        set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;

    /* duplicate module can't be const due to my linked_list impl. */
    virtual smod* duplicate_smod();

    // validation
    virtual errors::TYPE validate()
        { return errors::NO_ERROR; }

    #ifdef DEBUG
    bool check_inputs();
    #endif

 protected:
    bool sm_done_first();
//    void invalidate()           { flags &=~ synthmod::SM_VALID; }

    /*  first instance initializations (ie parameter and data object
        registration) should be performed in the init_first method of
        derived objects.
     */

    void register_sm_param(param::TYPE);
    void register_sm_param(param::TYPE, const char* fixed_string);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    void register_sm_input(input::TYPE);

    bool validate_sm_param(param::TYPE, errors::TYPE);

    static char err_msg[STRBUFLEN];

 private:
    synthmod* sm;
    int flags;
//  not to be used:
    smod();
    smod(const smod &);
};


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

    friend smod;

    synthmod(module::TYPE, const char* const uname, int _flags_);

    virtual ~synthmod();

    module::TYPE    get_module_type() const { return module_type; }
    const char*     get_username()    const { return username; }

    const char*     get_group_name() const {
            return ((flags & SM_UNGROUPABLE)
                        ? 0 : get_groupname(username));
        }

    void set_group_name(const char* const);

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* input/output/param access */
    virtual const void* set_in(input::TYPE, const void*);
    virtual const void* get_in(input::TYPE) const;
    virtual const void* get_out(output::TYPE) const;
    virtual bool        set_param(param::TYPE, const void*);
    virtual const void* get_param(param::TYPE) const;

    virtual dobj*       add_dobj(dobj*);

    enum DUP_IO
    {
        AUTO_CONNECT,
        NO_CONNECT
    };

    /* duplicate module can't be const due to my linked_list impl. */
    virtual synthmod* duplicate_module(const char* const uname, DUP_IO);

    // validation
    virtual errors::TYPE validate()
        { return errors::NO_ERROR; }

    #ifdef DEBUG
    bool check_inputs();
    #endif

    static const char* get_error_msg()  { return err_msg; }

    // should only be used by jwmsynth::run()
    static const STATUS* get_abort_status()
        { return &abort_status;}

    bool operator()(module::TYPE & smt) const
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

    int       flag(SM_FLAGS _flags) const   { return _flags & flags; }
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
    void register_param(param::TYPE);
    void register_param(param::TYPE, const char* fixed_string);
    void register_moddobj(dataobj::TYPE parent, dataobj::TYPE sprog);

    /*  inputs & outputs OTOH, are unique to each instance, so will need
        registration per instance (ie in derived constructor).
    */
    void register_input(input::TYPE);
    void register_output(output::TYPE);

    bool validate_param(param::TYPE, errors::TYPE);

    static char err_msg[STRBUFLEN];

 private:
    module::TYPE    module_type;
    char*           username;
    int             flags;
    static STATUS   abort_status;

//  not to be used:
    synthmod();
    synthmod(const synthmod &);

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[module::LAST_TYPE];
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
