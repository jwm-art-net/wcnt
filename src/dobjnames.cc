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

const char*
dobjnames::get_descr(dobjnames::DOBJ_TYPE dt)
{
    return data[check_type(dt)].descr;
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
    { DOBJ_FIRST,       "**BAD OBJECT**", 0    },
    { DOBJ_NONE,        "none", 0              },
//------------------------------------------------------
    { DOBJ_LISTS,       "lists_defined_within", 0       },
//------------------------------------------------------
    { LST_ENVELOPE,     "envelope", 0          },
    { LST_WAVEFORM,     "waveform", 0          },
    { LST_METER,        "meter_map", 0         },
    { LST_BPM,          "bpm_map", 0           },
    { LST_TRACK,        "track", 0             },
    { LST_MIX,          "mix", 0               },
    { LST_SIGNALS,      "signals", 0           },
    { LST_DYNAMICS,     "amp_map", 0           },
    { LST_TIMINGS,      "timings", 0           },
    { LST_MODULES,      "modules", 0           },
    { LST_DOBJS,        "data_objects", 0      },
    { LST_NOTES,        "notelist", 0          },
    { LST_EDITS,        "editlist", 0          },
    { LST_TRIGGERS,     "triggers", 0          },
    { LST_STEPS,        "step_shape", 0        },
//------------------------------------------------------
    { DOBJ_SINGLES,     "objects_defined_in_lists", 0    },
//------------------------------------------------------
    { SIN_NOTE,         "note", 0              },
    { SIN_COORD,        "section", 0           },
    { SIN_VERTEX,       "vertex", 0            },
    { SIN_METER,        "meter", 0             },
    { SIN_BPM,          "tempo", 0             },
    { SIN_RIFFNODE,     "riff_node", 0         },
    { SIN_DVERTEX,      "dvertex", 0           },
    { SIN_TIME,         "time", 0              },
    { SIN_MODNAME,      "mod", 0               },
    { SIN_DOBJNAME,     "dobj", 0              },
    { SIN_STEP,         "stepvx", 0            },
//------------------------------------------------------
    { DOBJ_EDITS,       "edit_objects_defined_in_lists", 0 },
//------------------------------------------------------
    { SIN_EDIT_PARAM,   "edit", 0              },
    { SIN_EDIT_INPUT,   "connect", 0           },
//------------------------------------------------------
    { DOBJ_DEFS,        "dobjs_defined_outside_modules", 0 },
//------------------------------------------------------
    { DEF_WAVFILEIN,    "wavfilein",            "Used by a module to specify the audio file it should process." },
    { DEF_RIFF,         "riff",                 "Contains a sequence of notes for a sequencer to process."      },
    { DEF_WCFILE,       "synth_file_reader",    "Allows one .wc file to include the contents specified in another .wc file. Modules and data objects which are listed can either be excluded or the only contents included." },
    { DEF_PARAMEDITOR,  "param_editor",         "Allows editing of module or data object parameters. The parameters are written just as they would be ordinarily." },
    { DEF_INPUTEDITOR,  "input_editor",         "Allows editing of module inputs after module creation. The connections are written just as they would be ordinarily." },
    { DEF_RIFFEDITOR,   "riff_editor",          "Allows editing of notes in a riff. Extra notes can be added or existing notes shortened/lengthened/transposed etc. The syntax is a little quirky." },
    { DEF_COPIER,       "copier",               "Makes an exact duplicate of a module, group of modules, or data object, but with a different name." },
    { DEF_GROUP,        "group",                "Groups a series of modules together allowing the copier to be used more efficiently. Connections are updated where appropriate to use the newly created modules." },
    { DEF_ADSR_SCALER,  "adsr_scaler",          "Allows editing of ADSR envelopes after their creation/duplication." },
//------------------------------------------------------
    { DOBJ_SYNTHMOD,    "synthmod", 0         }
//------------------------------------------------------
};

