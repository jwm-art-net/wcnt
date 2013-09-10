#ifndef NOTEDATA_H
#include "../include/notedata.h"

note_data::note_data() :
 dobj(dobjnames::SIN_NOTE), length(0), position(0), velocity(0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

note_data::note_data(const char *name, double len, double pos, double vel)
 :
 dobj(dobjnames::SIN_NOTE), length(len), position(pos), velocity(vel)
{
    set_name(name);
#ifndef BARE_MODULES
    create_dparams();
#endif
}

note_data::~note_data()
{
}

void note_data::set_name(const char *name)
{
    strncpy(notename, name, NOTE_NAME_LEN);
    notename[NOTE_NAME_LEN] = '\0'; // incase len > 4 chars 
}

#ifndef BARE_MODULES

bool note_data::set_dparam(dparamnames::DPAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case dparamnames::DPAR_NOTENAME:
        set_name((char*)data); // pass pointer
        retv = true;
        break;
    case dparamnames::DPAR_NOTEPOS:
        set_position(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_NOTELEN:
        set_length(*(double*)data);
        retv = true;
        break;
    case dparamnames::DPAR_NOTEVEL:
        set_velocity(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* note_data::get_dparam(dparamnames::DPAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case dparamnames::DPAR_NOTENAME:
        retv = notename;
        break;
    case dparamnames::DPAR_NOTEPOS:
        retv = &position;
        break;
    case dparamnames::DPAR_NOTELEN:
        retv = &length;
        break;
    case dparamnames::DPAR_NOTEVEL:
        retv = &velocity;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

stockerrs::ERR_TYPE note_data::validate()
{
    if (!check_notename(notename)) {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_NOTENAME);
        *err_msg += " ";
        *err_msg += notename;
        invalidate();
        return stockerrs::ERR_NOTENAME;
    }
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, dparamnames::DPAR_NOTEPOS, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_NOTEPOS);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, dparamnames::DPAR_NOTELEN, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_NOTELEN);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void note_data::create_dparams()
{
    if (note_data::done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, dparamnames::DPAR_NOTENAME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, dparamnames::DPAR_NOTEPOS);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, dparamnames::DPAR_NOTELEN);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, dparamnames::DPAR_NOTEVEL);
    note_data::done_dparams = true;
}

bool note_data::done_dparams = false;

#endif
#endif
