#include "../include/riffeditor.h"
#include "../include/riffdata.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjparamlist.h"
#include "../include/dobjdobjlist.h"

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
    switch(pt)
    {
    case paramnames::RIFFNAME:
        if (((dobj*)data)->get_object_type() == dobjnames::DEF_RIFF) {
            set_riff_source((riffdata*)data);// pass pointer
            return true;
        }
        *err_msg = "\n";
        *err_msg += get_username();
        *err_msg += " is not a riff and cannot be used as one.";
        return false;
    default:
        return false;
    }
}

void const* riff_editor::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RIFFNAME: return riff_source;
        default: return 0;
    }
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
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::DEF_RIFFEDITOR, paramnames::RIFFNAME);
    jwm.get_topdobjlist()->create_dobjdobjlist(
        dobjnames::DEF_RIFFEDITOR, dobjnames::LST_NOTES)->
            add_dobjdobj(dobjnames::LST_NOTES, dobjnames::SIN_NOTE);
    done_params = true;
}

bool riff_editor::done_params = false;

