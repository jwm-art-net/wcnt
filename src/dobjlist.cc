#include "../include/dobjlist.h"
#include "../include/globals.h"
#include "../include/dataobjects.h"

namespace dobj
{
 dobj::base* list::create_dobj(int dt)
 {
    dobj::base* dobjy = 0;
    switch(dt) {
    case dobj::SIN_NOTE:
        dobjy = new note_data;
        break;
    case dobj::SIN_COORD:
        dobjy = new adsr_coord;
        break;
    case dobj::SIN_VERTEX:
        dobjy = new wave_vertex;
        break;
    case dobj::SIN_METER:
        dobjy = new meterchange;
        break;
    case dobj::SIN_BPM:
        dobjy = new bpmchange;
        break;
    case dobj::SIN_RIFFNODE:
        dobjy = new riff_node;
        break;
    case dobj::SIN_DVERTEX:
        dobjy = new dynvertex;
        break;
    case dobj::SIN_TIME:
        dobjy = new timing;
        break;
    case dobj::SIN_MODNAME:
        dobjy = new modnamedobj;
        break;
    case dobj::SIN_DOBJNAME:
        dobjy = new dobjnamedobj;
        break;
    case dobj::SIN_EDIT_PARAM:
        dobjy = new paramedit;
        break;
    case dobj::SIN_EDIT_INPUT:
        dobjy = new inputedit;
        break;
    case dobj::SIN_STEP:
        dobjy = new step_data;
        break;
    case dobj::DEF_WAVFILEIN:
        dobjy = new wavfilein;
        break;
    case dobj::DEF_RIFF:
        dobjy = new riffdata;
        break;
    case dobj::DEF_WCFILE:
        dobjy = new synthfilereader;
        break;
    case dobj::DEF_PARAMEDITOR:
        dobjy = new parameditor;
        break;
    case dobj::DEF_INPUTEDITOR:
        dobjy = new inputeditor;
        break;
    case dobj::DEF_RIFFEDITOR:
        dobjy = new riff_editor;
        break;
    case dobj::DEF_COPIER:
        dobjy = new copier;
        break;
    case dobj::DEF_GROUP:
        dobjy = new group;
        break;
    case dobj::DEF_ADSR_SCALER:
        dobjy = new adsr_scaler;
        break;
    case dobj::DOBJ_SYNTHMOD:
        dobjy = new dobjmod;
        break;
    default:
        dobjy = 0;
    }
    if (dobjy)
        dobjy->ui_register();
    return dobjy;
 }
}; // namespace dobj
