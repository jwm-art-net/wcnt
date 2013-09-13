#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/jwm_globals.h"
#include "../include/connectorlist.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/groupnames.h"

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
    sm_err("*** MAJOR ERROR *** Bad attempt made to get output where \
                                            none exist.", get_username());
    #endif
    return 0;
}

const void* synthmod::set_in(inputnames::IN_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    sm_err("*** MAJOR ERROR *** Bad attempt made to set input where \
                                            none exist.", get_username());
    #endif
    return 0;
}

const void* synthmod::get_in(inputnames::IN_TYPE) const
{
    #ifdef IO_PARANOIA
    sm_err("*** MAJOR ERROR *** Bad attempt made to get input where \
                                            none exist.", get_username());
    #endif
    return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, const void*)
{
    #ifdef IO_PARANOIA
    sm_err("*** MAJOR ERROR *** Bad attempt made to set parameter where \
                                            none exist.", get_username());
    #endif
    return false;
}

const void* synthmod::get_param(paramnames::PAR_TYPE) const
{
    #ifdef IO_PARANOIA
    sm_err("*** MAJOR ERROR *** Bad attempt made to get parameter where \
                                            none exist.", get_username());
    #endif
    return 0;
}

dobj* synthmod::add_dobj(dobj*)
{
    #ifdef IO_PARANOIA
    sm_err("*** MAJOR ERROR *** Bad attempt made to add data object to \
                module unable to contain data objects.", get_username());
    #endif
    return 0;
}

synthmod*
synthmod::duplicate_module(const char* uname, DUP_IO iocon)
{
    if (flag(SM_UNDUPLICABLE)) {
        sm_err("Duplication of module %s is forbidden due to type %s.",
                username, jwm.get_modnames()->get_name(module_type));
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

void synthmod::duplicate_inputs_to(synthmod* to_mod)
{
    jwm.get_connectlist()->duplicate_connections(this, to_mod);
}

void synthmod::duplicate_params_to(synthmod* to_mod)
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

#ifdef IO_DEBUG
bool synthmod::check_inputs()
{
    modinputlist::linkedlist* modinps =
        new_list_of_by(jwm.get_inputlist(), this);
    modinput* modinp = modinps->goto_first();
    while(modinp) {
        if (!get_in(modinp->get_inputtype())) {
            sm_err("*** MAJOR ERROR *** Module %s does not have its %s \
                    input set.", get_username(), 
                        jwm.get_inputnames()->get_name(
                                        modinp->get_inputtype()));
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

#ifdef MOD_STATS
STATS_INIT(synthmod)
#endif

