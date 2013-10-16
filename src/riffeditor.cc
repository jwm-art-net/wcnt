#include "../include/riffeditor.h"
#include "../include/riffdata.h"
#include "../include/globals.h"


riff_editor::riff_editor() :
 dobj::base(dobj::DEF_RIFFEDITOR), riff_source(0)
{
}

void riff_editor::register_ui()
{
    register_param(param::RIFFNAME);
    register_dobj(dobj::LST_NOTES, dobj::SIN_NOTE);
}

riff_editor::~riff_editor()
{
}

bool riff_editor::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::RIFFNAME:
        if (((dobj::base*)data)->get_object_type() == dobj::DEF_RIFF) {
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

const void* riff_editor::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::RIFFNAME: return riff_source;
        default: return 0;
    }
}

const dobj::base* riff_editor::add_dobj(dobj::base* dbj)
{
    dobj::base* retv = 0;
    dobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobj::SIN_NOTE:
        retv = riff_source->insert_and_position_note((note_data*)dbj);
        if (!retv)
            dobjerr("Could not add note change to %s by way of "
                    "riff_editor %s.", riff_source->get_username(),
                                      get_username());
        break;
    default:
        dobjerr("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                        get_username());
        retv = 0;
    }
    return retv;
}

errors::TYPE riff_editor::validate()
{
    return errors::NO_ERROR;
}



