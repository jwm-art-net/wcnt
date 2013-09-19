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
    #ifdef MOD_STATS
    STATS_INC
    #endif
}

synthmod::~synthmod()
{
    jwm.get_outputlist()->delete_module_outputs(this);
    jwm.get_inputlist()->delete_module_inputs(this);
    delete [] username;
    #ifdef MOD_STATS
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
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to get output when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return 0;
}

const void* synthmod::set_in(inputnames::IN_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to set input when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return 0;
}

const void* synthmod::get_in(inputnames::IN_TYPE) const
{
    #ifdef IO_PARANOIA
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to get input when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to set parameter when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return false;
}

const void* synthmod::get_param(paramnames::PAR_TYPE) const
{
    #ifdef IO_PARANOIA
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to get parameter when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return 0;
}

dobj* synthmod::add_dobj(dobj*)
{
    #ifdef IO_PARANOIA
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += username;
    *err_msg += "\nattempt made to add data object to";
    *err_msg += "\nmodule lacking ability to accept them\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    #endif
    return 0;
}

synthmod*
synthmod::duplicate_module(const char* uname, DUP_IO iocon) const
{
    if (flag(SM_UNDUPLICABLE)) {
        *err_msg = "\nmodule ";
        *err_msg += username;
        *err_msg += " of type ";
        *err_msg += jwm.get_modnames()->get_name(module_type);
        *err_msg += " is forbidden from being copied.";
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
    *err_msg = "*** Program Error! ***  Module ";
    *err_msg += username;
    *err_msg += ": call to base method synthmod::init_first. This method "
                "should be implemented in the derived class.";
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
        *err_msg = "Failed to register param ";
        *err_msg += jwm.get_paramnames()->get_name(pt);
        *err_msg += " with module type ";
        *err_msg += jwm.get_modnames()->get_name(module_type);
        *err_msg += ".";
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
        *err_msg = "Failed to register fixed string param ";
        *err_msg += jwm.get_paramnames()->get_name(pt);
        *err_msg += " (";
        *err_msg += fixstr;
        *err_msg += ") with module type ";
        *err_msg += jwm.get_modnames()->get_name(module_type);
        *err_msg += ".";
        invalidate();
    }
}

void synthmod::register_input(inputnames::IN_TYPE t)
{
    if (!(flags & SM_VALID))
        return;
    if (!jwm.get_inputlist()->register_input(this, t))
    {
        *err_msg = "Failed to register input ";
        *err_msg += jwm.get_inputnames()->get_name(t);
        *err_msg += " with module ";
        *err_msg += username;
        *err_msg += ".";
        invalidate();
    }
}

void synthmod::register_output(outputnames::OUT_TYPE t)
{
    if (!(flags & SM_VALID))
        return;
    if (!jwm.get_outputlist()->register_output(this, t))
    {
        *err_msg = "Failed to register output ";
        *err_msg += jwm.get_outputnames()->get_name(t);
        *err_msg += " with module ";
        *err_msg += username;
        *err_msg += ".";
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
        *err_msg = "Failed to register data object ";
        *err_msg += jwm.get_dobjnames()->get_name(parent);
        *err_msg += " (child ";
        *err_msg += jwm.get_dobjnames()->get_name(sprog);
        *err_msg += ") with module type ";
        *err_msg += jwm.get_modnames()->get_name(module_type);
        *err_msg += ".";
        invalidate();
    }
}


#ifdef IO_DEBUG
bool synthmod::check_inputs()
{
    modinputlist::linkedlist* modinps =
        new_list_of_by(jwm.get_inputlist(), this);
    modinput* modinp = modinps->goto_first();
    while(modinp) {
        if (!get_in(modinp->get_inputtype())) {
            *err_msg = "\n*** Program Error! ***\nmodule ";
            *err_msg += get_username();
            *err_msg += " has not had its input ";
            *err_msg +=
                jwm.get_inputnames()->get_name(
                    modinp->get_inputtype());
            *err_msg += " set.\n Please email james@jwm-art.net";
            *err_msg += " with the .wc file(s) responsible so the issue";
            *err_msg += " can be resolved. Thankyou.";
            delete modinps;
            return false;
        }
        modinp = modinps->goto_next();
    }
    delete modinps;
    return true;
}
#endif

std::string* synthmod::err_msg = 0;
STATUS synthmod::abort_status = OFF; // don't force an abort just yet.

#ifdef MOD_STATS
STATS_INIT(synthmod)
#endif


bool synthmod::first_done[synthmodnames::LAST] = { false };
