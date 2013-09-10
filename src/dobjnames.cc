#ifndef DOBJNAMES_H
#include "../include/dobjnames.h"

dobjnames::dobjnames()
{
    dobj_name = new char*[DOBJ_LAST];
    dobj_name[DOBJ_FIRST] = "**BAD OBJECT**";
    dobj_name[DOBJ_NONE] = "none";
//------------------------------------------------------
    dobj_name[DOBJ_LISTS] = "lists_defined_within";
//------------------------------------------------------
    dobj_name[LST_ENVELOPE] = "envelope";
    dobj_name[LST_WAVEFORM] = "waveform";
    dobj_name[LST_METER] = "meter_map";
    dobj_name[LST_BPM] = "bpm_map";
    dobj_name[LST_TRACK] = "track";
    dobj_name[LST_MIX] = "mix";
    dobj_name[LST_SIGNALS] = "signals";
    dobj_name[LST_DYNAMICS] = "amp_map";
    dobj_name[LST_TIMINGS] = "timings";
    dobj_name[LST_MODULES] = "modules";
    dobj_name[LST_DOBJS] = "data_objects";
    dobj_name[LST_NOTES] = "notelist";
    dobj_name[LST_EDITS] = "editlist";
    dobj_name[LST_TRIGGERS] = "triggers";
    dobj_name[LST_STEPS] = "step_shape";
//------------------------------------------------------
    dobj_name[DOBJ_SINGLES] = "objects_defined_in_lists";
//------------------------------------------------------
    dobj_name[SIN_NOTE] = "note";
    dobj_name[SIN_COORD] = "section";
    dobj_name[SIN_VERTEX] = "vertex";
    dobj_name[SIN_METER] = "meter";
    dobj_name[SIN_BPM] = "tempo";
    dobj_name[SIN_RIFFNODE] = "riff_node";
    dobj_name[SIN_DVERTEX] = "dvertex";
    dobj_name[SIN_TIME] = "time";
    dobj_name[SIN_MODNAME] = "mod";
    dobj_name[SIN_DOBJNAME] = "dobj";
    dobj_name[SIN_STEP] = "stepvx";
//------------------------------------------------------
    dobj_name[DOBJ_EDITS] = "edit_objects_defined_in_lists";
//------------------------------------------------------
    dobj_name[SIN_EDIT_PARAM] = "edit";
    dobj_name[SIN_EDIT_INPUT] = "connect";
//------------------------------------------------------
    dobj_name[DOBJ_DEFS] = "dobjs_defined_outside_modules";
//------------------------------------------------------
    dobj_name[DEF_WAVFILEIN] = "wavfilein";
    dobj_name[DEF_RIFF] = "riff";
    dobj_name[DEF_WCFILE] = "synth_file_reader";
    dobj_name[DEF_PARAMEDITOR] = "param_editor";
    dobj_name[DEF_INPUTEDITOR] = "input_editor";
    dobj_name[DEF_RIFFEDITOR] = "riff_editor";
    dobj_name[DEF_COPIER] = "copier";
    dobj_name[DEF_GROUP] = "group";
//------------------------------------------------------
    dobj_name[DOBJ_SYNTHMOD] = "synthmod";
//------------------------------------------------------
}

dobjnames::~dobjnames()
{
    delete [] dobj_name;
}

char const* dobjnames::get_name(dobjnames::DOBJ_TYPE dt)
{
    return dobj_name[check_type(dt)];
}

dobjnames::DOBJ_TYPE dobjnames::get_type(char const* dname)
{
    if (dname == 0) return DOBJ_FIRST;
    for (int dt = DOBJ_FIRST + 1; dt < DOBJ_LAST; dt++) {
        if (dt == DOBJ_LISTS || dt == DOBJ_SINGLES || dt == DOBJ_DEFS)
            dt++;
        if (strcmp(dname, dobj_name[dt]) == 0)
            return (DOBJ_TYPE)dt;
    }
    return DOBJ_FIRST;
}

dobjnames::DOBJ_TYPE dobjnames::check_type(dobjnames::DOBJ_TYPE dt)
{
    if (dt <= DOBJ_FIRST ||
        dt == DOBJ_NONE ||
        dt == DOBJ_LISTS ||
        dt == DOBJ_SINGLES ||
        dt == DOBJ_EDITS ||
        dt == DOBJ_DEFS ||
        dt >= DOBJ_LAST) return DOBJ_FIRST;
    return dt;
}

dobjnames::DOBJ_TYPE dobjnames::get_sub_type(dobjnames::DOBJ_TYPE dt)
{
    if (dt >= DOBJ_LAST)     return DOBJ_FIRST;
    if (dt == DOBJ_SYNTHMOD) return DOBJ_SYNTHMOD;
    if (dt >= DOBJ_DEFS)     return DOBJ_DEFS;
    if (dt >= DOBJ_EDITS)    return DOBJ_EDITS;
    if (dt >= DOBJ_SINGLES)  return DOBJ_SINGLES;
    if (dt >= DOBJ_LISTS)    return DOBJ_LISTS;
    return DOBJ_FIRST;
}

char const* dobjnames::get_sub_name(dobjnames::DOBJ_TYPE dt)
{
    return dobj_name[get_sub_type(dt)];
}

#endif
