#ifndef RIFFEDITOR_H
#include "../include/riffeditor.h"

riff_editor::riff_editor() :
 dobj(dobjnames::DEF_RIFFEDITOR), riff_source(0)
{
    create_params();
}

riff_editor::~riff_editor()
{
}

bool riff_editor::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_RIFFNAME:
        if (((dobj*)data)->get_object_type() == dobjnames::DEF_RIFF) {
            set_riff_source((riffdata*)data);// pass pointer
            retv = true;
        }
        else {
            *err_msg = "\n";
            *err_msg += get_username();
            *err_msg += " is not a riff and cannot be used as one.";
            retv = false;
        }
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* riff_editor::get_param(paramnames::PAR_TYPE pt)
{
    void* retv = 0;
    switch(pt)
    {
    case paramnames::PAR_RIFFNAME:
        retv = riff_source;
        break;
    default:
        retv = 0;
        break;
    }
    return retv;
}

dobj const* riff_editor::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_NOTE:
        retv = riff_source->insert_and_position_note((note_data*)dbj);
        if (!retv)
            *err_msg = "\ncould not add note change to ";
            *err_msg += riff_source->get_username();
            *err_msg += " by way of riff_editor ";
            *err_msg += get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to ";
        *err_msg += get_username();
        retv = 0;
    }
    return retv;
}

stockerrs::ERR_TYPE riff_editor::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

void riff_editor::create_params()
{
    if (done_params == true) return;
    get_dparlist()->
    add_dobjparam(dobjnames::DEF_RIFFEDITOR, paramnames::PAR_RIFFNAME);
    dobjdobjlist* dbjlist = get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_RIFFEDITOR, dobjnames::LST_NOTES);
    dbjlist->add_dobjdobj(dobjnames::LST_NOTES, dobjnames::SIN_NOTE);
    done_params = true;
}

bool riff_editor::done_params = false;

#endif
