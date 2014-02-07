#ifndef SYNTHMODULE_H
#define SYNTHMODULE_H

#include "boolfuncobj.h"
#include "dobjnames.h"
#include "groupnames.h"
#include "inputnames.h"
#include "outputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "synthmodnames.h"
#include "textstuff.h"
#include "types.h"
#include "ui_moditem.h"

#ifdef DEBUG
#include <cstdio>
#endif


namespace dobj      { class base; }
namespace modpart   { class base; };


/*
//  synthmod - pure abstract base class for jwm synth modules.
*/


namespace synthmod
{

 class base
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

//    friend modpart::base;

    base(TYPE, const char* const uname, int _flags_);
    virtual ~base();

    TYPE        get_module_type() const { return modtype; }
    const char* get_username()  const   { return username; }

    const char* get_group_name() const {
            return ((flags & SM_UNGROUPABLE)
                        ? 0 : get_groupname(username));
        }

    void set_group_name(const char* const);

    /* virtuals */
    virtual void run() = 0;
    virtual void init(){};

    /* UI access to inputs/outputs/params.. */
    virtual const void* set_in(int input_type, const void*);
    virtual const void* get_in(int input_type) const;
    virtual const void* get_out(int output_type) const;
    virtual bool        set_param(int param_type, const void*);
    virtual const void* get_param(int param_type) const;

    virtual dobj::base* add_dobj(dobj::base*);

    enum DUP_IO
    {
        AUTO_CONNECT,
        NO_CONNECT
    };

    /* duplicate module can't be const due to my linked_list impl. */
    virtual base* duplicate_module(const char* const uname, DUP_IO);

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

    bool operator()(int & smt) const    { return modtype == smt; }

    bool operator()(fnobj::name & n) const     { return n(username); }

    bool operator()(fnobj::groupname & n) const{
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

    /*  Has the module registered its params, inputs, outputs, and
        data objects yet? The registration is a per-type initialization
        and not a per-instance initialization.
     */
    bool type_registered() const;
    void ui_register();

    virtual ui::moditem_list* get_ui_items() = 0;

  protected:
    static void force_abort()   { abort_status = ON; }
    void invalidate()           { flags &=~ SM_VALID; }

    friend modpart::base;

    /*  if the module has inputs, parameters, or data objects, they
        should be registered within register_ui by using the register
        methods below, it should register each in turn.
        register_ui itself will be called after object creat
     */
    virtual void register_ui() = 0;

    /*  Methods for registering params, inputs, and data objects with
        the UI. These should be called only once when a call to
        do_registration() returns true.
     */
    ui::modparam* register_param(int param_type);
    ui::modparam* register_param(int param_type, const char* fixed_string);
    ui::modinput* register_input(int input_type);
    ui::moddobj* register_dobj(dobj::TYPE parent, dobj::TYPE sprog);
    ui::modcomment* register_comment(const char* literal);

    /*  Method for output registration, outputs must be registered per
        instance.S
     */
    void register_output(int output_type);

    /*  Method to be called by a module to validate a single parameter.
        To be called only by the validate method.
     */
    bool validate_param(int param_type, errors::TYPE);

    void duplicate_inputs_to(base*) const;
    void duplicate_params_to(base*) const;

    static char err_msg[STRBUFLEN];

  private:
    TYPE            modtype;
    char*           username;
    int             flags;
    static STATUS   abort_status;

//  not to be used:
    base();
    base(const base &);

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    static bool first_done[LAST_TYPE];
 };


 #ifdef DEBUG
 #define sm_err(fmt, ... )                              \
 {                                                       \
    printf("%40s:%5d %-35s\n",                          \
                    __FILE__, __LINE__, __FUNCTION__);  \
    cfmt(synthmod::base::err_msg, STRBUFLEN, fmt, __VA_ARGS__);   \
 }
 #else
 #define sm_err(fmt, ... ) \
    cfmt(synthmod::base::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
 #endif


}; // namespace synthmod


#endif
