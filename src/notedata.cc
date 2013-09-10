#ifndef NOTEDATA_H
#include "../include/notedata.h"

note_data::note_data() :
 dobj(dobjnames::SIN_NOTE), length(0), position(0), velocity(0)
{
    create_params();
    #ifdef SEQ_NOTE_DEBUG
    cout << "\nnote_data(default) this:" << this;
    #endif
}

note_data::note_data(const char *name, double len, double pos, double vel)
 :
 dobj(dobjnames::SIN_NOTE), length(len), position(pos), velocity(vel)
{
    set_name(name);
    create_params();
    #ifdef SEQ_NOTE_DEBUG
    cout << "\nnote_data(name,len,pos,vel) this:" << this;
    display_note();
    #endif
}

note_data::~note_data()
{
    #ifdef SEQ_NOTE_DEBUG
    cout << "\n~note_data() this:" << this;
    display_note();
    #endif
}

void note_data::set_name(const char *name)
{
    strncpy(notename, name, NOTE_NAME_LEN);
    notename[NOTE_NAME_LEN] = '\0'; // incase len > 4 chars 
}

bool note_data::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        set_name((char*)data); // pass pointer
        retv = true;
        break;
    case paramnames::PAR_NOTE_POS:
        set_position(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_NOTE_LEN:
        set_length(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_NOTE_VEL:
        set_velocity(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* note_data::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_NAME:
        retv = notename;
        break;
    case paramnames::PAR_NOTE_POS:
        retv = &position;
        break;
    case paramnames::PAR_NOTE_LEN:
        retv = &length;
        break;
    case paramnames::PAR_NOTE_VEL:
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
        *err_msg = get_paramnames()->get_name(paramnames::PAR_NAME);
        *err_msg += " ";
        *err_msg += notename;
        invalidate();
        return stockerrs::ERR_NOTENAME;
    }
    dobjparamlist* dpl = get_dparlist();
    if (!dpl->validate(
        this, paramnames::PAR_NOTE_POS, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_NOTE_POS);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!dpl->validate(
        this, paramnames::PAR_NOTE_LEN, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_NOTE_LEN);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void note_data::create_params()
{
    if (note_data::done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, paramnames::PAR_NAME);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, paramnames::PAR_NOTE_POS);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, paramnames::PAR_NOTE_LEN);
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_NOTE, paramnames::PAR_NOTE_VEL);
    note_data::done_params = true;
}

bool note_data::done_params = false;

#ifdef SEQ_NOTE_DEBUG
#include <iostream>
using namespace std;
void note_data::display_note()
{
    cout << "\tname: " << notename;
    cout << " pos: " << position;
    cout << " len: " << length;
    cout << " vel: " << velocity;
}
#endif

#endif
