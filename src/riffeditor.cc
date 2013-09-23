#include "../include/riffeditor.h"
#include "../include/riffdata.h"
#include "../include/jwm_globals.h"
#include "../include/topdobjlist.h"
#include "../include/dobjparamlist.h"
#include "../include/dobjdobjlist.h"


riff_editor::riff_editor() :
 dobj(dobjnames::DEF_RIFFEDITOR), riff_source(0)
{
    init_first();
}

riff_editor::~riff_editor()
{
}

bool riff_editor::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt)
    {
    case paramnames::RIFFNAME:
        if (((dobj*)data)->get_object_type() == dobjnames::DEF_RIFF) {
            set_riff_source((riffdata*)data);// pass pointer
            return true;
        }
        dobjerr("%s is not a riff and cannot be used as one.",
                                                get_username());
        return false;
    default:
        return false;
    }
}

const void* riff_editor::get_param(paramnames::PAR_TYPE pt) const
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
            dobjerr("Could not add note change to %s by way of "
                    "riff_editor %s.", riff_source->get_username(),
                                      get_username());
        break;
    default:
        dobjerr("%s %s to %s", stockerrs::major, stockerrs::bad_add,
                                                        get_username());
        retv = 0;
    }
    return retv;
}

stockerrs::ERR_TYPE riff_editor::validate()
{
    return stockerrs::ERR_NO_ERROR;
}

void riff_editor::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::RIFFNAME);
    register_dobjdobj(dobjnames::LST_NOTES, dobjnames::SIN_NOTE);
}


