#include "../include/dobjnames.h"

namespace dobj
{
 const struct names::gn_data names::data[LAST_TYPE] =
 {
    { ERR_TYPE,         "BAD_OBJECT_TYPE",  CAT_ERR_TYPE,"ERROR!"                                   ,0},
    { LST_ENVELOPE,     "envelope",         CAT_LIST,   "ADSR Envelope definition."                 ,0},
    { LST_WAVEFORM,     "waveform",         CAT_LIST,   "User-defined waveform definition."         ,0},
    { LST_METER,        "meter_map",        CAT_LIST,   "Meter/Time Signature change list/map."     ,0},
    { LST_BPM,          "bpm_map",          CAT_LIST,   "Tempo/BPM change list/map."                ,0},
    { LST_TRACK,        "track",            CAT_LIST,   "Sequencer track, list of riff nodes."      ,0},
    { LST_MIX,          "mix",              CAT_LIST,   "Mixer module list."                        ,0},
    { LST_SIGNALS,      "signals",          CAT_LIST,   "List of modules providing signals to operate on."  ,0},
    { LST_DYNAMICS,     "amp_map",          CAT_LIST,   "Amplitude map shape definition."           ,0},
    { LST_TIMINGS,      "timings",          CAT_LIST,   "List of sequential timings."               ,0},
    { LST_MODULES,      "modules",          CAT_LIST,   "List of modules to directly operate."      ,0},
    { LST_DOBJS,        "data_objects",     CAT_LIST,   "List of data objects to directly operate." ,0},
    { LST_NOTES,        "notelist",         CAT_LIST,   "List of note data to orchestrate."         ,0},
    { LST_EDITS,        "editlist",         CAT_LIST,   "List of edits to process."                 ,0},
    { LST_TRIGGERS,     "triggers",         CAT_LIST,   "List of trigger modules to process."       ,0},
    { LST_STEPS,        "step_shape",       CAT_LIST,   "Stepper step-shape definition."            ,0},
    { SIN_NOTE,         "note",             CAT_SINGLE, "Musical note definition."                  ,0},
    { SIN_COORD,        "section",          CAT_SINGLE, "ADSR Envelope section definition."         ,0},
    { SIN_VERTEX,       "vertex",           CAT_SINGLE, "User-defined waveform vertext."            ,0},
    { SIN_METER,        "meter",            CAT_SINGLE, "Time signature/meter."             ,0},
    { SIN_BPM,          "tempo",            CAT_SINGLE, "Tempo/BPM."                        ,0},
    { SIN_RIFFNODE,     "riff_node",        CAT_SINGLE, "Riff insertion point."             ,0},
    { SIN_DVERTEX,      "dvertex",          CAT_SINGLE, "Dynamics amp map vertex."          ,0},
    { SIN_TIME,         "time",             CAT_SINGLE, "Time event."                       ,0},
    { SIN_MODNAME,      "mod",              CAT_SINGLE, "Module name."                      ,0},
    { SIN_DOBJNAME,     "dobj",             CAT_SINGLE, "Data object name."                 ,0},
    { SIN_STEP,         "stepvx",           CAT_SINGLE, "Stepper step coordinate."          ,0},
    { SIN_EDIT_PARAM,   "edit",             CAT_EDIT,   "Module or data object to edit."    ,0},
    { SIN_EDIT_INPUT,   "connect",          CAT_EDIT,   "Module to connect."                ,0},
    { SIN_EDIT,         "edit",             CAT_EDIT,   "Module or data object to edit."    ,0},
    { DEF_WAVFILEIN,    "wavfilein",        CAT_DEF,    "Used by a module to specify the audio file it should process." ,0},
    { DEF_RIFF,         "riff",             CAT_DEF,    "Contains a sequence of notes for a sequencer to process. Notes are specified in the form c#-1 for the C# an octave below middle C. Flat notes, ie Eb, cannot be directly specified. See the riff_editor for what else can be done with them." ,0},
    { DEF_WCFILE,       "synth_file_reader",CAT_DEF,    "Allows one .wc file to include the contents specified in another .wc file. Modules and data objects which are listed can either be excluded or exclusively included." ,0},
    { DEF_PARAMEDITOR,  "param_editor",     CAT_DEF,    "Allows editing of module or data object parameters. The parameters are written just as they would be ordinarily. After the \"edit\" command, the \"<unnamed_item> text_string\" should be substituted with the name of a module or data object. While the \"<unnamed_list> text_string\" should be substituted with a parameter name and its value." ,0},
    { DEF_INPUTEDITOR,  "input_editor",     CAT_DEF,    "Allows editing of module inputs after module creation. The connections are written just as they would be ordinarily. After the \"connect\" command, the \"<unnamed_item> text_string\" should be substituted with the name of a module. While the \"<unnamed_list> text_string\" should be substituted with an input name and then the output module and its output to connect with." ,0},
    { DEF_EDITOR,       "editor",           CAT_DEF,    "Allows editing of data object parameters, and both module parameters and inputs.", 0},
    { DEF_RIFFEDITOR,   "riff_editor",      CAT_DEF,    "Allows editing of notes in a riff. Extra notes can be added or existing notes shortened/lengthened/transposed etc. The syntax is a little quirky." ,0},
    { DEF_COPIER,       "copier",           CAT_DEF,    "Makes an exact duplicate of a module, group of modules, or data object, but with a different name." ,0},
    { DEF_GROUP,        "group",            CAT_DEF,    "Groups a series of modules together allowing the copier to be used more efficiently. Connections referencing modules in the pre-existing group are updated to use modules in new group. Autogrouping supersedes direct usage of the group data object, using the autogroup_create or autogroup_add directives followed by name of the group before creating modules. Once modules for the group are created, the group name again will stop autogrouping (or use autogroup_stop)." ,0},
    { DEF_ADSR_SCALER,  "adsr_scaler",      CAT_DEF,    "Allows editing of ADSR envelopes after their creation/duplication." ,0},
    { DOBJ_SYNTHMOD,    "synthmod",         CAT_SYNTHMOD, "Module to process." ,0}
 };
}; // namespace dobj

