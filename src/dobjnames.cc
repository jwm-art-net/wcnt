#include "../include/dobjnames.h"

#include <cstring>

#ifdef DEBUG
#include <iostream>
#endif

dobjnames::dobjnames()
{
#ifdef DEBUG
    for (int i = DOBJ_FIRST; i < DOBJ_LAST; i++){
        if (data[i].type != i) {
            std::cout << "\n***** dobjnames error *****\n";
            std::cout << data[i].name << " is in array index " << i;
            std::cout << " but has type index of " << data[i].type;
            std::cout << "\nthese values should tally!\n";
        }
    }
#endif
}

#ifdef DEBUG
static dobjnames dnames;
#endif

const char*
dobjnames::get_name(dobjnames::DOBJ_TYPE dt)
{
    return data[check_type(dt)].name;
}

dobjnames::DOBJ_TYPE
dobjnames::get_type(const char* const dname)
{
    if (dname == 0) return DOBJ_FIRST;
    for (int dt = DOBJ_FIRST + 1; dt < DOBJ_LAST; dt++) {
        if (dt == DOBJ_LISTS || dt == DOBJ_SINGLES || dt == DOBJ_DEFS)
            dt++;
        if (strcmp(dname, data[dt].name) == 0)
            return (DOBJ_TYPE)dt;
    }
    return DOBJ_FIRST;
}

dobjnames::DOBJ_TYPE
dobjnames::check_type(dobjnames::DOBJ_TYPE dt)
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

dobjnames::DOBJ_TYPE
dobjnames::get_sub_type(dobjnames::DOBJ_TYPE dt)
{
    if (dt >= DOBJ_LAST)     return DOBJ_FIRST;
    if (dt == DOBJ_SYNTHMOD) return DOBJ_SYNTHMOD;
    if (dt >= DOBJ_DEFS)     return DOBJ_DEFS;
    if (dt >= DOBJ_EDITS)    return DOBJ_EDITS;
    if (dt >= DOBJ_SINGLES)  return DOBJ_SINGLES;
    if (dt >= DOBJ_LISTS)    return DOBJ_LISTS;
    return DOBJ_FIRST;
}

#ifdef UNUSED
const char*
dobjnames::get_sub_name(dobjnames::DOBJ_TYPE dt)
{
    return data[get_sub_type(dt)].name;
}
#endif

const dobjnames::dobj_data dobjnames::data[DOBJ_LAST] =
{
    { DOBJ_FIRST,       "**BAD OBJECT**"    },
    { DOBJ_NONE,        "none"              },
//------------------------------------------------------
    { DOBJ_LISTS,       "lists_defined_within"          },
//------------------------------------------------------
    { LST_ENVELOPE,     "envelope"          },
    { LST_WAVEFORM,     "waveform"          },
    { LST_METER,        "meter_map"         },
    { LST_BPM,          "bpm_map"           },
    { LST_TRACK,        "track"             },
    { LST_MIX,          "mix"               },
    { LST_SIGNALS,      "signals"           },
    { LST_DYNAMICS,     "amp_map"           },
    { LST_TIMINGS,      "timings"           },
    { LST_MODULES,      "modules"           },
    { LST_DOBJS,        "data_objects"      },
    { LST_NOTES,        "notelist"          },
    { LST_EDITS,        "editlist"          },
    { LST_TRIGGERS,     "triggers"          },
    { LST_STEPS,        "step_shape"        },
//------------------------------------------------------
    { DOBJ_SINGLES,     "objects_defined_in_lists"      },
//------------------------------------------------------
    { SIN_NOTE,         "note"              },
    { SIN_COORD,        "section"           },
    { SIN_VERTEX,       "vertex"            },
    { SIN_METER,        "meter"             },
    { SIN_BPM,          "tempo"             },
    { SIN_RIFFNODE,     "riff_node"         },
    { SIN_DVERTEX,      "dvertex"           },
    { SIN_TIME,         "time"              },
    { SIN_MODNAME,      "mod"               },
    { SIN_DOBJNAME,     "dobj"              },
    { SIN_STEP,         "stepvx"            },
//------------------------------------------------------
    { DOBJ_EDITS,       "edit_objects_defined_in_lists" },
//------------------------------------------------------
    { SIN_EDIT_PARAM,   "edit"              },
    { SIN_EDIT_INPUT,   "connect"           },
//------------------------------------------------------
    { DOBJ_DEFS,        "dobjs_defined_outside_modules" },
//------------------------------------------------------
    { DEF_WAVFILEIN,    "wavfilein" },
    { DEF_RIFF,         "riff" },
    { DEF_WCFILE,       "synth_file_reader" },
    { DEF_PARAMEDITOR,  "param_editor"      },
    { DEF_INPUTEDITOR,  "input_editor"      },
    { DEF_RIFFEDITOR,   "riff_editor"       },
    { DEF_COPIER,       "copier"            },
    { DEF_GROUP,        "group"             },
    { DEF_ADSR_SCALER,  "adsr_scaler"       },
//------------------------------------------------------
    { DOBJ_SYNTHMOD,    "synthmod"          }
//------------------------------------------------------
};

