#ifndef DOBJNAMES_H
#define DOBJNAMES_H

#include "getnames.h"


namespace dobj
{
 enum CAT
 {
    CAT_ERR_TYPE,
    CAT_LIST,
    CAT_SINGLE,
    CAT_EDIT,
    CAT_DEF,
    CAT_SYNTHMOD,
    CAT_LAST_TYPE
 };

 enum TYPE
 {
    ERR_TYPE,
    //---------------------------------------------
    // lists to be defined within a module
    // or data object
    //---------------------------------------------
    LST_ENVELOPE,
    LST_WAVEFORM,
    LST_METER,
    LST_BPM,
    LST_TRACK,
    LST_MIX,
    LST_SIGNALS,
    LST_DYNAMICS,
    LST_TIMINGS,
    LST_MODULES,
    LST_DOBJS,
    LST_NOTES,
    LST_EDITS,
    LST_TRIGGERS,
    LST_STEPS,
    //---------------------------------------------
    // single objects to be inserted in lists
    // dobj inherited
    //---------------------------------------------
    SIN_NOTE,
    SIN_COORD,
    SIN_VERTEX,
    SIN_METER,
    SIN_BPM,
    SIN_RIFFNODE,
    SIN_DVERTEX,
    SIN_TIME,
    SIN_MODNAME,
    SIN_DOBJNAME,
    SIN_STEP,
    //---------------------------------------------
    // dobjs defined in a list of edits - which
    // interpret a list of commands contained in a
    // string (paramnames::PAR_STR_LIST)
    //---------------------------------------------
    SIN_EDIT_PARAM,
    SIN_EDIT_INPUT,
    //---------------------------------------------
    // stand alone dobj's defined outside of
    // a module, and inserted by name, as
    // a parameter into either dobj or module.
    //---------------------------------------------
    DEF_WAVFILEIN,
    DEF_RIFF,
    DEF_WCFILE,
    DEF_PARAMEDITOR,
    DEF_INPUTEDITOR,
    DEF_RIFFEDITOR,
    DEF_COPIER,
    DEF_GROUP,
    DEF_ADSR_SCALER,
    //---------------------------------------------
    // special dobj for inserting synthmodules into
    // list see dobjmod.h for more info.
    //---------------------------------------------
    DOBJ_SYNTHMOD,
    //---------------------------------------------
    LAST_TYPE
    //---------------------------------------------
 };

 class names : public getnames<TYPE, CAT>
 {
  public:
    static void instantiate() { static names paramnames; }

  private:
    names() : getnames(LAST_TYPE, data) {}
    ~names() {}
    static const struct gn_data data[LAST_TYPE];
 };
}; // namespace dobj

#endif
