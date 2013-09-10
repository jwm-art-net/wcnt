#ifndef DOBJNAMES_H
#define DOBJNAMES_H

#include <string.h>

using namespace std;

// if you want to add new dobj types then MAKE SURE you add them
// to the CORRECT part of the enum.... and be prepared for hassle!

// DOBJ_TYPE check_type(DOBJ_TYPE)
// checks the type passed is a usable type, ie is not one of these types:
//      DOBJ_FIRST DOBJ_NONE DOBJ_INLISTS DOBJ_INSINGLES
//      DOBJ_DEFLISTS DOBJ_DEFSINGLES DOBJ_LAST
// if it's one of them, it returns DOBJ_FIRST, otherwise, if it's a
// usable type it will return the value passed.

class dobjnames
{
public:
    enum DOBJ_TYPE
    {
        //---------------------------------------------
        DOBJ_FIRST = 0, // keep first & error type
        //---------------------------------------------
        DOBJ_NONE,      // hmmm, no dobj inserted?
        //---------------------------------------------
        DOBJ_LISTS,   // seperator
        //---------------------------------------------
        // lists to be defined within the module
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
        DOBJ_SINGLES, // seperator
        //---------------------------------------------
        // single objects to be inserted in lists
        // dobj inherited
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
        DOBJ_EDITS,
        //---------------------------------------------
        // dobjs defined in a list of edits - which 
        // interpret a list of commands contained in a
        // string (paramnames::PAR_STR_LIST)
        //---------------------------------------------
        SIN_EDIT_PARAM,
        SIN_EDIT_INPUT,
        //---------------------------------------------
        DOBJ_DEFS,      // seperator
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
        //---------------------------------------------
        // special dobj for inserting synthmodules into
        // list see dobjmod.h for more info.
        //---------------------------------------------
        DOBJ_SYNTHMOD,
        //---------------------------------------------
        DOBJ_LAST       // keep last
        //---------------------------------------------
    };
    dobjnames();
    ~dobjnames();
    char const* get_name(DOBJ_TYPE);
    DOBJ_TYPE get_type(char const*);
    DOBJ_TYPE check_type(DOBJ_TYPE);
    DOBJ_TYPE get_sub_type(DOBJ_TYPE);
    char const* get_sub_name(DOBJ_TYPE);

private:
    char** dobj_name;
};
#endif
