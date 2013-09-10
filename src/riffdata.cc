#ifndef RIFFDATA_HH
#include "../include/riffdata.h"

riffdata::riffdata() :
 dobj(dobjnames::LDEF_RIFF),
 quarter_val(0), note(0), notelist(0), note_item(0)
{
    notelist = 
     new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
#ifndef BARE_MODULES
    create_dparams();
#endif
}

riffdata::~riffdata()
{
    ll_item* tmp = notelist->goto_first();
    while(tmp)
    {
        delete (note_data*)tmp->get_data();
        tmp = notelist->goto_next();
    }
    delete notelist;
}

note_data* riffdata::insert_and_position_note(note_data* newnote)
{
    if (!newnote)
        return 0;
    // removed checking of notename, because it's done by note_data
    // and having it here brings up wrong error message.
    return ordered_insert(notelist, newnote, &note_data::get_position);
}

bool riffdata::delete_note(note_data * nd)
{
    if (!nd)
        return false;
    ll_item* tmp = notelist->find_data(nd);
    if (!tmp)
        return false;
    delete notelist->unlink_item(tmp);
    delete nd;
    return true;
}

double riffdata::calc_riff_length(char beats_per_bar, char beat_value)
{
    if (!goto_last()) return 0;
    double note_off_pos = note->get_position() + note->get_length();
    short bars = 1;
    double beatlen = 0;
    double rifflen = 0;
    while (rifflen < note_off_pos){
        beatlen = (get_quartervalue() * (4.0 / (double)beat_value));
        rifflen = beats_per_bar * beatlen * bars;
        bars++;
    }
    return rifflen;
}

#ifndef BARE_MODULES

bool riffdata::set_dparam(dparamnames::DPAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case dparamnames::DPAR_QUARTERVAL:
        set_quartervalue(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* riffdata::get_dparam(dparamnames::DPAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case dparamnames::DPAR_QUARTERVAL:
        retv = &quarter_val;
        break;
    default:
        retv = 0;
    }
    return retv;
}

dobj* riffdata::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_NOTE:
        if (!(retv = insert_and_position_note((note_data*)dbj)))
            *err_msg = "\ncould not add note change to ";
            *err_msg += *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to ";
        *err_msg += *get_username();
        retv = 0;
    }
    return retv;
}

stockerrs::ERR_TYPE riffdata::validate()
{
    if (!get_dparlist()->validate(
        this, dparamnames::DPAR_QUARTERVAL, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_dparnames()->get_name(dparamnames::DPAR_QUARTERVAL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void riffdata::create_dparams()
{
    if (riffdata::done_dparams == true)	return;
    get_dparlist()->add_dobjparam(
     dobjnames::LDEF_RIFF, dparamnames::DPAR_QUARTERVAL);
    get_dobjdobjlist()->add_dobjdobj(
     dobjnames::LDEF_RIFF, dobjnames::SIN_NOTE);
    riffdata::done_dparams = true;
}

bool riffdata::done_dparams = false;

#endif
#endif
