#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/jwm_globals.h"
#include "../include/connectorlist.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/groupnames.h"
#include "../include/fxsparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>

//------------------------------------------------------------------------
// public member methods
//------------------------------------------------------------------------

synthmod::synthmod(
 synthmodnames::SYNTH_MOD_TYPE mt, const char* const uname, int _flags_) :
 module_type(mt), username(0)
{
    flags = _flags_ | SM_VALID;
    username = new char[strlen(uname) + 1];
    strcpy(username, uname);
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

synthmod::~synthmod()
{
    jwm.get_outputlist()->delete_module_outputs(this);
    jwm.get_inputlist()->delete_module_inputs(this);
    delete [] username;
    #ifdef DATA_STATS
    STATS_DEC
    #endif
}

void synthmod::set_group_name(const char* const gname)
{
    const char* oldname = username;
    username = set_groupname(gname, username);
    delete [] oldname;
}

const void* synthmod::get_out(outputnames::OUT_TYPE) const
{
    #ifdef IO_PARANOIA
    sm_err("%s %s get output where none exist.", stockerrs::major,
                                                 stockerrs::bad, username);
    #endif
    return 0;
}

const void* synthmod::set_in(inputnames::IN_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    sm_err("%s %s set input where none exist.", stockerrs::major,
                                                stockerrs::bad, username);
    #endif
    return 0;
}

const void* synthmod::get_in(inputnames::IN_TYPE) const
{
    #ifdef IO_PARANOIA
    sm_err("%s %s get input where none exist.", stockerrs::major,
                                                stockerrs::bad, username);
    #endif
    return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    sm_err("%s %s set parameter where none exist.", stockerrs::major,
                                                stockerrs::bad, username);
    #endif
    return false;
}

const void* synthmod::get_param(paramnames::PAR_TYPE) const
{
    #ifdef IO_PARANOIA
    sm_err("%s %s get parameter where none exist.", stockerrs::major,
                                                stockerrs::bad, username);
    #endif
    return 0;
}

dobj* synthmod::add_dobj(dobj*)
{
    #ifdef IO_PARANOIA
    sm_err("%s %s module unable to contain data objects.", stockerrs::major,
                                              stockerrs::bad_add, username);
    #endif
    return 0;
}

synthmod*
synthmod::duplicate_module(const char* uname, DUP_IO iocon)
{
    if (flag(SM_UNDUPLICABLE)) {
        sm_err("Duplication of module %s is forbidden due to type %s.",
                username, synthmodnames::get_name(module_type));
        return 0;
    }
    synthmod* dup = jwm.get_modlist()->create_module(module_type, uname);
    if (iocon == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    return dup;
}

//------------------------------------------------------------------------
// protected member methods
//------------------------------------------------------------------------

void synthmod::duplicate_inputs_to(synthmod* to_mod) const
{
    jwm.get_connectlist()->duplicate_connections(this, to_mod);
}

void synthmod::duplicate_params_to(synthmod* to_mod) const
{
    modparamlist::linkedlist* mplist =
        new_list_of_by(jwm.get_paramlist(), to_mod->get_module_type());
    if (mplist) {
        modparam* mp = mplist->goto_first();
        while(mp) {
            paramnames::PAR_TYPE pt = mp->get_paramtype();
            to_mod->set_param(pt, get_param(pt));
            mp = mplist->goto_next();
        }
        delete mplist;
    }
}

void synthmod::init_first()
{
    #ifdef IO_PARANOIA
    sm_err("%s, Module %s: call to base method "
           "synthmod::init_first. This method should be implemented "
           "in the derived class.", stockerrs::major, username);
    #endif
}

bool synthmod::done_first() const
{
    bool r = first_done[module_type];
    first_done[module_type] = true;
    return r;
}

void synthmod::register_param(paramnames::PAR_TYPE pt)
{
    if (!(flags & SM_VALID))
        return;
    if (!jwm.get_paramlist()->add_param(module_type, pt))
    {
        sm_err("Failed to register param %s with module type %s.",
                paramnames::get_name(pt),
                synthmodnames::get_name(module_type));
        invalidate();
    }
}

void synthmod::register_param(paramnames::PAR_TYPE pt, const char* fixstr)
{
    if (!(flags & SM_VALID))
        return;

    modparam* mp = jwm.get_paramlist()->add_param(module_type, pt);
    fixstrparam* fsp = 0;
    if (mp)
        fsp = jwm.get_fxsparamlist()->add_param(fixstr, pt);
    if (!mp || !fsp)
    {
        sm_err("Failed to register fixed string param %s (%s) "
               "with module type %s,",  paramnames::get_name(pt),
                                        fixstr,
                               synthmodnames::get_name(module_type));
        invalidate();
    }
}

void synthmod::register_input(inputnames::IN_TYPE t)
{
    if (!(flags & SM_VALID))
        return;
    if (!jwm.get_inputlist()->register_input(this, t))
    {
        sm_err("Failed to register input %s with module %s.",
                inputnames::get_name(t), username);
        invalidate();
    }
}

void synthmod::register_output(outputnames::OUT_TYPE t)
{
    if (!(flags & SM_VALID))
        return;
    if (!jwm.get_outputlist()->register_output(this, t))
    {
        sm_err("Failed to register output %s with module %s.",
                outputnames::get_name(t), username);
        invalidate();
    }
}

void synthmod::register_moddobj(dobjnames::DOBJ_TYPE parent,
                                                dobjnames::DOBJ_TYPE sprog)
{
    if (!(flags & SM_VALID))
        return;
    moddobj* mdbj;
    dobjdobj* ddbj = 0;
    mdbj = jwm.get_moddobjlist()->register_moddobj(module_type, parent);
    if (mdbj)
        ddbj = mdbj->get_dobjdobjlist()->register_dobjdobj(parent, sprog);
    if (!mdbj || !ddbj)
    {
        sm_err("Failed to register data object %s (child %s) with "
               "module type %s.", dobjnames::get_name(parent),
                                dobjnames::get_name(sprog),
                                synthmodnames::get_name(module_type));
        invalidate();
    }
}


#ifdef DEBUG
bool synthmod::check_inputs()
{
    modinputlist::linkedlist* modinps =
        new_list_of_by(jwm.get_inputlist(), this);
    modinput* modinp = modinps->goto_first();
    while(modinp) {
        if (!get_in(modinp->get_inputtype())) {
            sm_err("%s Module %s does not have its %s "
                    "input set.", stockerrs::major, get_username(), 
                        inputnames::get_name(modinp->get_inputtype()));
            delete modinps;
            return false;
        }
        modinp = modinps->goto_next();
    }
    delete modinps;
    return true;
}
#endif

char synthmod::err_msg[] = "";
STATUS synthmod::abort_status = OFF; // don't force an abort just yet.

#ifdef DATA_STATS
STATS_INIT(synthmod)
#endif


bool synthmod::first_done[synthmodnames::LAST] = { false };
