#ifndef SYNTHMODULE_H
#include "../include/synthmodule.h"
#include "../include/synthmodulelist.h"
#include "../include/jwm_globals.h"
#include "../include/connectorlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/groupnames.h"

#include <iostream>

//------------------------------------------------------------------------
// public member methods
//------------------------------------------------------------------------

synthmod::synthmod(
 synthmodnames::SYNTH_MOD_TYPE mt, const char* const uname) :
 module_type(mt), username(0), valid(true)
{
    username = new char[strlen(uname) + 1];
    strcpy(username, uname);

#ifdef MOD_STATS
STATS_INC
#endif

}

synthmod::~synthmod()
{
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
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to get output when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

const void* synthmod::set_in(inputnames::IN_TYPE, const void*)
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to set input when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

const void* synthmod::get_in(inputnames::IN_TYPE) const
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to get input when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

bool synthmod::set_param(paramnames::PAR_TYPE, const void*)
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to set parameter when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return false;
}

const void* synthmod::get_param(paramnames::PAR_TYPE) const
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to get parameter when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

dobj* synthmod::add_dobj(dobj*)
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to add data object to";
    *err_msg += "\nmodule lacking ability to accept them\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

synthmod*
synthmod::duplicate_module(const char* uname, DUP_IO iocon)
{
    synthmod* dup = jwm.get_modlist().create_module(module_type, uname);
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
    connectorlist* conlist =
        jwm.get_connectlist().duplicate_connections_for_module
            (this, to_mod);
    if (conlist) {
        connector* con = conlist->goto_first();
        while(con) {
            jwm.get_connectlist().add_connector(con);
            con = conlist->goto_next();
        }
        delete conlist;
    }
}

void synthmod::duplicate_params_to(synthmod* to_mod)
{
    modparamlist* mplist =
        jwm.get_paramlist().get_paramlist_for_moduletype
            (to_mod->get_module_type());
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

bool synthmod::check_inputs()
{
    modinputlist* modinps =
        jwm.get_inputlist().get_inputlist_for_module(this);
    modinput* modinp = modinps->goto_first();
    while(modinp) {
        if (!get_in(modinp->get_inputtype())) {
            *err_msg = "\n*** Program Error! ***\nmodule ";
            *err_msg += get_username();
            *err_msg += " has not had its input ";
            *err_msg += jwm.get_inputnames().get_name(
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

std::string* synthmod::err_msg = 0;
STATUS synthmod::abort_status = OFF; // don't force an abort just yet.

#ifdef MOD_STATS
STATS_INIT(synthmod)
#endif

#endif
