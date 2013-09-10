#ifndef NOTEDATA_H
#include "../include/notedata.h"

note_data::note_data() :
 dobj(dobjnames::SIN_NOTE),
 note_type(NOTE_TYPE_ERR),position(0), length(0), velocity(0)
{
    create_params();
    #ifdef SEQ_NOTE_DEBUG
    cout << "\nnote_data(default) this:" << this;
    #endif
    #ifdef SHOW_NOTE_COUNT
    notes_created_count++;
    notes_count++;
    notes_max_count = (notes_count > notes_max_count)
                        ? notes_count : notes_max_count;
    #endif
}

note_data::note_data(const char *name, double pos, double len, double vel)
 :
 dobj(dobjnames::SIN_NOTE),
 note_type(NOTE_TYPE_ERR), position(pos), length(len), velocity(vel)
{
    set_name(name);
    create_params();
    #ifdef SEQ_NOTE_DEBUG
    cout << "\nnote_data(name,pos,len,vel) this:" << this;
    display_note();
    #endif
    #ifdef SHOW_NOTE_COUNT
    notes_created_count++;
    notes_count++;
    notes_max_count = (notes_count > notes_max_count)
                        ? notes_count : notes_max_count;
    #endif
}

note_data::~note_data()
{
    #ifdef SHOW_NOTE_COUNT
    notes_destroyed_count++;
    notes_count--;
    #endif
    #ifdef SEQ_NOTE_DEBUG
    cout << "\n~note_data() this:" << this;
    display_note();
    #endif
}

void note_data::set_name(const char *name)
{
    strncpy(notename, name, NOTE_NAME_LEN);
    notename[NOTE_NAME_LEN] = '\0';
    get_note_type();
}

note_data::NOTE_SEL_OP note_data::get_note_sel_op()
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_SEL_OP_ERR;
    const char* ptr = notename + NOTE_CHR_SEL_OP;
    switch(*ptr)
    {
        case '=':   return NOTE_SEL_OP_EQU;
        case '<':  return NOTE_SEL_OP_LESS;
        case '>':  return NOTE_SEL_OP_MORE;
        case 'I':    return NOTE_SEL_OP_IN;
        case 'O':   return NOTE_SEL_OP_OUT;
        default:
            return NOTE_SEL_OP_ERR;
    };
}

note_data::NOTE_SEL note_data::get_note_sel()
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_SEL_ERR;
    const char* ptr = notename + NOTE_CHR_SEL;
    switch(*ptr)
    {
        case 'N': return NOTE_SEL_NAME;
        case 'P': return NOTE_SEL_POS;
        case 'L': return NOTE_SEL_LEN;
        case 'V': return NOTE_SEL_VEL;
        default:
            return NOTE_SEL_ERR;
    };
}

note_data::NOTE_OP note_data::get_note_op()
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_OP_ERR;
    const char* ptr = notename + NOTE_CHR_OP;
    switch(*ptr)
    {
        case '+': return NOTE_OP_ADD;
        case '-': return NOTE_OP_SUB;
        case '*': return NOTE_OP_MUL;
        case '/': return NOTE_OP_DIV;
        default:
            return NOTE_OP_ERR;
    };
}

note_data::NOTE_PAR note_data::get_note_par()
{
    if (note_type != NOTE_TYPE_EDIT)
        return NOTE_PAR_ERR;
    const char* ptr = notename + NOTE_CHR_PAR;
    switch(*ptr)
    {
        case '!': return NOTE_PAR_DEL;
        case 'N': return NOTE_PAR_NAME;
        case 'P': return NOTE_PAR_POS;
        case 'L': return NOTE_PAR_LEN;
        case 'V': return NOTE_PAR_VEL;
        case 'Q': return NOTE_PAR_QOPY;
        default:
            return NOTE_PAR_ERR;
    };
}

note_data::NOTE_TYPE note_data::get_note_type()
{
    NOTE_TYPE retv = NOTE_TYPE_ERR;
    if (check_notename(notename) == true)
        return (note_type = NOTE_TYPE_NORMAL);
    else {
        retv = note_type = NOTE_TYPE_EDIT;
        if (get_note_sel_op() == NOTE_SEL_OP_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            cout << "\n\tfailed on note_sel_op";
            #endif
        }
        if (get_note_sel() == NOTE_SEL_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            cout << "\n\tfailed on note_sel";
            #endif
        }
        if (get_note_op() == NOTE_OP_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            cout << "\n\tfailed on note_op";
            #endif
        }
        if (get_note_par() == NOTE_PAR_ERR) {
            retv = NOTE_TYPE_ERR;
            #ifdef NOTE_EDIT_DEBUG
            cout << "\n\tfailed on note_par";
            #endif
        }
    }
    return (note_type = retv);
}

const char* note_data::get_note_name()
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    return ptr;
}

double note_data::get_note_number()
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    char oct = extract_octave(ptr);
    double noteno = (60 + oct * 12 + (note_to_noteno(ptr) - 1));
    return noteno;
}

double note_data::get_note_frequency()
{
    const char* ptr = notename;
    if (note_type == NOTE_TYPE_ERR)
        return 0;
    if (note_type==NOTE_TYPE_EDIT) ptr += NOTE_CHR_NAME;
    return note_to_freq(ptr);
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
    if (note_type == NOTE_TYPE_ERR) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_NAME);
        *err_msg += " is problematically set with ";
        *err_msg += notename;
        invalidate();
        return stockerrs::ERR_NOTENAME;
    } else if (note_type == NOTE_TYPE_EDIT)
        return stockerrs::ERR_NO_ERROR;
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

#ifdef SHOW_NOTE_COUNT
long note_data::notes_created_count = 0;
long note_data::notes_destroyed_count = 0;
long note_data::notes_count = 0;
long note_data::notes_max_count = 0;
#endif

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
