#include "../include/dobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dataobjects.h"

dobj* dobjlist::create_dobj(dobjnames::DOBJ_TYPE dt)
{
    if (dobjnames::check_type(dt) == dobjnames::DOBJ_FIRST)
        return 0;
    dobj* dobjy = 0;
    switch(dt) {
    case dobjnames::SIN_NOTE:
        dobjy = new note_data;
        break;
    case dobjnames::SIN_COORD:
        dobjy = new adsr_coord;
        break;
    case dobjnames::SIN_VERTEX:
        dobjy = new wave_vertex;
        break;
    case dobjnames::SIN_METER:
        dobjy = new meterchange;
        break;
    case dobjnames::SIN_BPM:
        dobjy = new bpmchange;
        break;
    case dobjnames::SIN_RIFFNODE:
        dobjy = new riff_node;
        break;
    case dobjnames::SIN_DVERTEX:
        dobjy = new dynvertex;
        break;
    case dobjnames::SIN_TIME:
        dobjy = new timing;
        break;
    case dobjnames::SIN_MODNAME:
        dobjy = new modnamedobj;
        break;
    case dobjnames::SIN_DOBJNAME:
        dobjy = new dobjnamedobj;
        break;
    case dobjnames::SIN_EDIT_PARAM:
        dobjy = new paramedit;
        break;
    case dobjnames::SIN_EDIT_INPUT:
        dobjy = new inputedit;
        break;
    case dobjnames::SIN_STEP:
        dobjy = new step_data;
        break;
    case dobjnames::DEF_WAVFILEIN:
        dobjy = new wavfilein;
        break;
    case dobjnames::DEF_RIFF:
        dobjy = new riffdata;
        break;
    case dobjnames::DEF_WCFILE:
        dobjy = new synthfilereader;
        break;
    case dobjnames::DEF_PARAMEDITOR:
        dobjy = new parameditor;
        break;
    case dobjnames::DEF_INPUTEDITOR:
        dobjy = new inputeditor;
        break;
    case dobjnames::DEF_RIFFEDITOR:
        dobjy = new riff_editor;
        break;
    case dobjnames::DEF_COPIER:
        dobjy = new copier;
        break;
    case dobjnames::DEF_GROUP:
        dobjy = new group;
        break;
    case dobjnames::DEF_ADSR_SCALER:
        dobjy = new adsr_scaler;
        break;
    case dobjnames::DOBJ_SYNTHMOD:
        dobjy = new dobjmod;
        break;
    default:
        dobjy = 0;
    }
    return dobjy;
}

