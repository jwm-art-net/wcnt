#ifndef RIFFDATA_HH
#include "../include/riffdata.h"

riffdata::riffdata() :
 dobj(dobjnames::DEF_RIFF),
 quarter_val(0), note(0), notelist(0), note_item(0)
{
    notelist = 
     new linkedlist(linkedlist::MULTIREF_OFF,linkedlist::NO_NULLDATA);
    create_params();
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

bool riffdata::set_param(paramnames::PAR_TYPE dt, void* data)
{
    bool retv = false;
    switch(dt)
    {
    case paramnames::PAR_QUARTER_VAL:
        set_quartervalue(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* riffdata::get_param(paramnames::PAR_TYPE dt)
{
    void* retv = 0;
    switch(dt)
    {
    case paramnames::PAR_QUARTER_VAL:
        retv = &quarter_val;
        break;
    default:
        retv = 0;
    }
    return retv;
}

dobj const* riffdata::add_dobj(dobj* dbj)
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
        this, paramnames::PAR_QUARTER_VAL, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_QUARTER_VAL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void riffdata::create_params()
{
    if (riffdata::done_params == true)
        return;
    get_dparlist()->add_dobjparam(
     dobjnames::DEF_RIFF, paramnames::PAR_QUARTER_VAL);
    dobjdobjlist* dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_RIFF, dobjnames::LST_NOTES);
    dbjlist->add_dobjdobj(dobjnames::LST_NOTES, dobjnames::SIN_NOTE);
    riffdata::done_params = true;
}

bool riffdata::done_params = false;

#endif
