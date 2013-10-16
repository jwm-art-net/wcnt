#ifndef NOTEDATA_H
#define NOTEDATA_H

#include "dobj.h"
#include "globals.h"

// note_data
// there used to be a whole load of comments with regards to note length
// and position values, but since quarter_value is no longer hard coded,
// and now defined by the user for each riff, it seemed pointless.

/*
    whoah! what's all this then?
    ============================

    the name of a note can be used not only as the name of a note, but
    also as an riff editing command.

    examples of normal note names:
        c0
        e#-1
        g2

    examples of edit commands:
        =P-N    select note at editnote pos and subtract editnote len
                from its name. (ie negative transpose)
        <L*P    select notes with lengths less than editnote len and
                multiply the pos of each by editnote vel.
        >N-Ve0  select notes higher than name and subtract pos from
                their vel.
        IV/L    select notes with vel inside range of editnote pos and
                editnote len, and divide their len by editnote vel.
        OP*N    select notes with pos outside range of editnote pos
                and editnote len and multiply their name by editnote vel.
*/

class note_data : public dobj::base
{
 public:
    enum NOTE_TYPE {
        NOTE_TYPE_ERR = -1,
        NOTE_TYPE_NORMAL = 0,
        NOTE_TYPE_EDIT
    };
    enum EDIT_NOTE_CHR {
        NOTE_CHR_SEL_OP = 0,
        NOTE_CHR_SEL,
        NOTE_CHR_OP,
        NOTE_CHR_PAR,
        NOTE_CHR_NAME
    };
    enum NOTE_SEL_OP {
        NOTE_SEL_OP_ERR = -1,
        NOTE_SEL_OP_EQU = 0,
        NOTE_SEL_OP_LESS,
        NOTE_SEL_OP_MORE,
        NOTE_SEL_OP_IN,
        NOTE_SEL_OP_OUT
    };
    enum NOTE_SEL {
        NOTE_SEL_ERR = -1,
        NOTE_SEL_NAME = 0,
        NOTE_SEL_POS,
        NOTE_SEL_LEN,
        NOTE_SEL_VEL
    };
    enum NOTE_OP {
        NOTE_OP_ERR = -1,
        NOTE_OP_ADD = 0,
        NOTE_OP_SUB,
        NOTE_OP_MUL,
        NOTE_OP_DIV
    };
    enum NOTE_PAR {
        NOTE_PAR_ERR = -1,
        NOTE_PAR_DEL = 0,
        NOTE_PAR_NAME,
        NOTE_PAR_POS,
        NOTE_PAR_LEN,
        NOTE_PAR_VEL,
        NOTE_PAR_QOPY // like POS but does not delete the old notes.
    };
    note_data();
    note_data(const char* name, double pos, double len, double vel);
    ~note_data();
    // set_name and get_name set and return the entire notename string
    void set_name(const char * n);
    void set_length(double l) { length = l; }
    void set_position(double p) { position = p; }
    void set_velocity(double v) { velocity = v; }

    const char* get_name() const{ return (this == NULL) ? NULL : notename; }
    double get_length() const   { return (this == NULL) ? 0 : length; }
    double get_position()const  { return (this == NULL) ? 0 : position; }
    double get_velocity() const { return (this == NULL) ? 0.00 : velocity; }
    // the following only used with edit notes...
    NOTE_SEL_OP get_note_sel_op() const;
    NOTE_SEL    get_note_sel() const;
    NOTE_OP     get_note_op() const;
    NOTE_PAR    get_note_par() const;
    // helpful functions...
    NOTE_TYPE   get_note_type(); /* FIXME: de-tangle & make const */
    NOTE_TYPE   get_note_type(const char* notename) const;
    double      get_note_number() const;

    #ifdef UNUSED
    // get_note_name only returns, if applicable, the note portion.
    const char* get_note_name() const;
    double      get_note_frequency() const;
    #endif

    // virtuals from dobj
    const char* get_username(){ return notename;}
    // virtuals from dobj
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    errors::TYPE validate();

    #ifdef DATA_STATS
    STATS_FUNCS
    #endif

 private:
    NOTE_TYPE note_type;
    char notename[wcnt::note_array_size];
    double position;
    double length;
    double velocity;

    #ifdef DATA_STATS
    STATS_VARS
    #endif

    void register_ui();
};

#endif
