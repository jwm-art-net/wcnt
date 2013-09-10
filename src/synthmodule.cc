#ifndef SYNTHMODULE_H
#include "../include/synthmodule.h"
#include "../include/connectorlist.h"
#include "../include/modparamlist.h"
#include "../include/groupnames.h"

/* Default audio settings */
short audio_channels = 2;
unsigned long audio_samplerate = 44100ul;
short audio_bitrate = 16;
short audio_ysize = 32767;

/* Default settings for music tempo */
double sm_beats_per_minute = 160;
short sm_beats_per_measure = 4;
short sm_beat_value = 4;

synthmod::synthmod(
 synthmodnames::SYNTH_MOD_TYPE mt, short id, const char* uname) :
 module_type(mt), number_id(id), username(0), valid(true)
{
    username = new char[strlen(uname) + 1];
    strcpy(username, uname);
    #ifdef SHOW_MOD_COUNT
    mods_created_count++;
    mods_count++;
    mods_max_count = (mods_count > mods_max_count)
                        ? mods_count : mods_max_count;
    #endif
}

synthmod::~synthmod()
{
    delete [] username;
    #ifdef SHOW_MOD_COUNT
    mods_destroyed_count++;
    mods_count--;
    #endif
}


char* synthmod::get_group_name()
{
    return get_groupname(username);
}

void synthmod::set_group_name(const char* gname)
{
    char* oldname = username;
    username = set_groupname(gname, username);
    delete [] oldname;
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

const void* synthmod::get_in(inputnames::IN_TYPE)
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to get input when none exist\n";
    *err_msg += "\nPlease email james@jwm-art.net";
    *err_msg += " with the .wc file(s) responsible so the issue";
    *err_msg += " can be resolved. Thankyou.";
    return 0;
}

const void* synthmod::get_out(outputnames::OUT_TYPE)
{
    *err_msg = "\n*** Program Error! ***\nmodule ";
    *err_msg += get_username();
    *err_msg += "\nattempt made to get output when none exist\n";
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

const void* synthmod::get_param(paramnames::PAR_TYPE)
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

synthmod* synthmod::duplicate_module(const char* uname, DUP_IO iocon)
{
    synthmod* dup = modlist->create_module(module_type, uname);
    if (iocon == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    return dup;
}

void synthmod::duplicate_inputs_to(synthmod* to_mod)
{
    connectorlist* conlist =
        connectlist->duplicate_connections_for_module(this, to_mod);
    if (conlist) {
        connector* con = conlist->goto_first();
        while(con) {
            connectlist->add_connector(con);
            con = conlist->goto_next();
        }
        delete conlist;
    }
}

void synthmod::duplicate_params_to(synthmod* to_mod)
{
    modparamlist* mplist = paramlist->get_paramlist_for_moduletype(
                                            to_mod->get_module_type());
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
    modinputlist* modinps = inputslist->get_inputlist_for_module(this);
    modinput* modinp = modinps->goto_first();
    while(modinp) {
        if (!get_in(modinp->get_inputtype())) {
            *err_msg = "\n*** Program Error! ***\nmodule ";
            *err_msg += get_username();
            *err_msg += " has not had its input ";
            *err_msg += innames->get_name(modinp->get_inputtype());
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

string* synthmod::err_msg = 0;
char* synthmod::path = 0;
bool synthmod::verbose = false;
synthmodnames* synthmod::modnames = 0;
iocat_names* synthmod::iocatnames = 0;
inputnames* synthmod::innames = 0;
outputnames* synthmod::outnames = 0;
paramnames* synthmod::parnames = 0;
synthmodlist* synthmod::modlist = 0;
modinputlist* synthmod::inputslist = 0;
modoutputlist* synthmod::outputslist = 0;
modparamlist* synthmod::paramlist = 0;
connectorlist* synthmod::connectlist = 0;
moddobjlist* synthmod::mdobjlist = 0;
fxsparamlist* synthmod::fxsparlist = 0;
STATUS synthmod::abort_status = OFF; // don't force an abort just yet.
#ifdef SHOW_MOD_COUNT
long synthmod::mods_created_count = 0;
long synthmod::mods_destroyed_count = 0;
long synthmod::mods_count = 0;
long synthmod::mods_max_count = 0;
#endif

#endif
