#include "../include/dobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dataobjects.h"

dobj* dobjlist::create_dobj(dataobj::TYPE dt)
{
    dobj* dobjy = 0;
    switch(dt) {
    case dataobj::SIN_NOTE:
        dobjy = new note_data;
        break;
    case dataobj::SIN_COORD:
        dobjy = new adsr_coord;
        break;
    case dataobj::SIN_VERTEX:
        dobjy = new wave_vertex;
        break;
    case dataobj::SIN_METER:
        dobjy = new meterchange;
        break;
    case dataobj::SIN_BPM:
        dobjy = new bpmchange;
        break;
    case dataobj::SIN_RIFFNODE:
        dobjy = new riff_node;
        break;
    case dataobj::SIN_DVERTEX:
        dobjy = new dynvertex;
        break;
    case dataobj::SIN_TIME:
        dobjy = new timing;
        break;
    case dataobj::SIN_MODNAME:
        dobjy = new modnamedobj;
        break;
    case dataobj::SIN_DOBJNAME:
        dobjy = new dobjnamedobj;
        break;
    case dataobj::SIN_EDIT_PARAM:
        dobjy = new paramedit;
        break;
    case dataobj::SIN_EDIT_INPUT:
        dobjy = new inputedit;
        break;
    case dataobj::SIN_STEP:
        dobjy = new step_data;
        break;
    case dataobj::DEF_WAVFILEIN:
        dobjy = new wavfilein;
        break;
    case dataobj::DEF_RIFF:
        dobjy = new riffdata;
        break;
    case dataobj::DEF_WCFILE:
        dobjy = new synthfilereader;
        break;
    case dataobj::DEF_PARAMEDITOR:
        dobjy = new parameditor;
        break;
    case dataobj::DEF_INPUTEDITOR:
        dobjy = new inputeditor;
        break;
    case dataobj::DEF_RIFFEDITOR:
        dobjy = new riff_editor;
        break;
    case dataobj::DEF_COPIER:
        dobjy = new copier;
        break;
    case dataobj::DEF_GROUP:
        dobjy = new group;
        break;
    case dataobj::DEF_ADSR_SCALER:
        dobjy = new adsr_scaler;
        break;
    case dataobj::DOBJ_SYNTHMOD:
        dobjy = new dobjmod;
        break;
    default:
        dobjy = 0;
    }
    return dobjy;
}

