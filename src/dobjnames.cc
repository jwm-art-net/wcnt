#include "../include/dobjnames.h"

namespace dataobj
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,         "BAD_OBJECT_TYPE",  CAT_ERR_TYPE,"ERROR!"                                   },
    { LST_ENVELOPE,     "envelope",         CAT_LIST,   "ADSR Envelope definition."                 },
    { LST_WAVEFORM,     "waveform",         CAT_LIST,   "User-defined waveform definition."         },
    { LST_METER,        "meter_map",        CAT_LIST,   "Meter/Time Signature change list/map."     },
    { LST_BPM,          "bpm_map",          CAT_LIST,   "Tempo/BPM change list/map."                },
    { LST_TRACK,        "track",            CAT_LIST,   "Sequencer track, list of riff nodes."      },
    { LST_MIX,          "mix",              CAT_LIST,   "Mixer module list."                        },
    { LST_SIGNALS,      "signals",          CAT_LIST,   "List of modules providing signals to operate on."  },
    { LST_DYNAMICS,     "amp_map",          CAT_LIST,   "Amplitude map shape definition."           },
    { LST_TIMINGS,      "timings",          CAT_LIST,   "List of sequential timings."               },
    { LST_MODULES,      "modules",          CAT_LIST,   "List of modules to directly operate."      },
    { LST_DOBJS,        "data_objects",     CAT_LIST,   "List of data objects to directly operate." },
    { LST_NOTES,        "notelist",         CAT_LIST,   "List of note data to orchestrate."         },
    { LST_EDITS,        "editlist",         CAT_LIST,   "List of edits to process."                 },
    { LST_TRIGGERS,     "triggers",         CAT_LIST,   "List of trigger modules to process."       },
    { LST_STEPS,        "step_shape",       CAT_LIST,   "Stepper step-shape definition."            },
    { SIN_NOTE,         "note",             CAT_SINGLE, "Musical note definition."                  },
    { SIN_COORD,        "section",          CAT_SINGLE, "ADSR Envelope section definition."         },
    { SIN_VERTEX,       "vertex",           CAT_SINGLE, "User-defined waveform vertext."            },
    { SIN_METER,        "meter",            CAT_SINGLE, "Time signature/meter."             },
    { SIN_BPM,          "tempo",            CAT_SINGLE, "Tempo/BPM."                        },
    { SIN_RIFFNODE,     "riff_node",        CAT_SINGLE, "Riff insertion point."             },
    { SIN_DVERTEX,      "dvertex",          CAT_SINGLE, "Dynamics amp map vertex."          },
    { SIN_TIME,         "time",             CAT_SINGLE, "Time event."                       },
    { SIN_MODNAME,      "mod",              CAT_SINGLE, "Module name."                      },
    { SIN_DOBJNAME,     "dobj",             CAT_SINGLE, "Data object name."                 },
    { SIN_STEP,         "stepvx",           CAT_SINGLE, "Stepper step coordinate."          },
    { SIN_EDIT_PARAM,   "edit",             CAT_EDIT,   "Module or data object to edit."    },
    { SIN_EDIT_INPUT,   "connect",          CAT_EDIT,   "Module to connect."                },
    { DEF_WAVFILEIN,    "wavfilein",        CAT_DEF,    "Used by a module to specify the audio file it should process." },
    { DEF_RIFF,         "riff",             CAT_DEF,    "Contains a sequence of notes for a sequencer to process. Notes are specified in the form c#-1 for the C# an octave below middle C. Flat notes, ie Eb, cannot be directly specified. See the riff_editor for what else can be done with them." },
    { DEF_WCFILE,       "synth_file_reader",CAT_DEF,    "Allows one .wc file to include the contents specified in another .wc file. Modules and data objects which are listed can either be excluded or exclusively included." },
    { DEF_PARAMEDITOR,  "param_editor",     CAT_DEF,    "Allows editing of module or data object parameters. The parameters are written just as they would be ordinarily. After the \"edit\" command, the \"<unnamed_item> text_string\" should be substituted with the name of a module or data object. While the \"<unnamed_list> text_string\" should be substituted with a parameter name and its value." },
    { DEF_INPUTEDITOR,  "input_editor",     CAT_DEF,    "Allows editing of module inputs after module creation. The connections are written just as they would be ordinarily. After the \"connect\" command, the \"<unnamed_item> text_string\" should be substituted with the name of a module. While the \"<unnamed_list> text_string\" should be substituted with an input name and then the output module and its output to connect with." },
    { DEF_RIFFEDITOR,   "riff_editor",      CAT_DEF,    "Allows editing of notes in a riff. Extra notes can be added or existing notes shortened/lengthened/transposed etc. The syntax is a little quirky." },
    { DEF_COPIER,       "copier",           CAT_DEF,    "Makes an exact duplicate of a module, group of modules, or data object, but with a different name." },
    { DEF_GROUP,        "group",            CAT_DEF,    "Groups a series of modules together allowing the copier to be used more efficiently. Connections referencing modules in the pre-existing group are updated to use modules in new group." },
    { DEF_ADSR_SCALER,  "adsr_scaler",      CAT_DEF,    "Allows editing of ADSR envelopes after their creation/duplication." },
    { DOBJ_SYNTHMOD,    "synthmod",         CAT_SYNTHMOD, "Module to process." }
 };
}; // namespace dataobj

