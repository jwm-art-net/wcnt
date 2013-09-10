#ifndef RIFFDATA_HH
#define RIFFDATA_HH

#include "notedata.h"
#ifndef BARE_MODULES
#include "dobjparamlist.h"
#include "dobjdobjlist.h"
#endif
/*
RIFFDATA
--------
riffdata holds a linkedlist of notedata. when it comes to sequencing the
notes, riff_node defined in sequencer.h tells the sequencer what bar the
riff will start in.  This allows each riff to be 'ghosted'.  Ie copying
the riff along a sequence without duplicating it's data.  It also allows
in the case of multiple sequencers, for the same riff to be shared among
the sequencers at the same time if neccessary.  If the riff is more than
one bar long then it is possible that each sequencer starts the riff at
different bars.  The problem arises of keeping track of each sequencer's
position in the riff - ie the riff itself is no longer reliable to
maintain each sequencer's position.
this explains the variety of methods in riffdata for getting notes. the
goto_????() methods are intended for used by a UI which can only edit
one instance of a given riff at a time therefore it can rely on the riff's
capacity to maintain the 'current' note.  These return note_data*. Also
returning note_data* are the get_????() methods, but unlike the goto_???()
methods, they do not rely on linkedlist's ability to keep track of the
current item.  The really important ones as far a sequencer is concerned
are the sneak_first() & sneak_last() methods, these return an ll_item.
the sequencer then accesses the linkedlist && ll_items directly so it
keeps track of which note in the riff it is currently working on, and
provides it with a means of getting the next note, without using any
goto_???() methods.

Note that the get_prev() and get_next() methods, unlike the goto_??
equivalents do not iterate through the list, but only return
current->get_next etc.

hope that clears things up?!

What's new in wcnt1.1001?
-------------------------

i've removed riff length calculation code as it is no longer needed.
i've added 1/4 note value so user can specify a value which suits
thier requirements...constructor default is 12.

------------------
*** ammendment ***
------------------
added calc_riff_length(...) method to get the length of the riff. 
the value returned is in terms of a note length with a quarter value 
as set for the riff, giving the length of the number of bars the riff
occupies. ie it does not return how many bars the riff occupies, but
the note length of those bars.

the method requires both elements of the time signature to be passed.
*/

class riffdata : public dobj
{
public:
    riffdata();
    ~riffdata();
    void set_quartervalue(short qv){ quarter_val = qv;}
    short get_quartervalue(){ return quarter_val;}
    note_data* insert_and_position_note(note_data*);
    bool delete_note(note_data*);
    note_data* goto_first() {
        return note = (note_data*)
         (note_item = notelist->goto_first())->get_data();
    }
    note_data* goto_last() {
        return note = (note_data*)
         (note_item = notelist->goto_last())->get_data();
    }
    note_data* goto_prev() {
        return note = (note_data*)
         (note_item = notelist->goto_prev())->get_data();
    }
    note_data* goto_next() {
        return note = (note_data*)
         (note_item = notelist->goto_next())->get_data();
    }
    // these do not update any list pointers
    note_data* get_first() {
        return (note_data*)notelist->sneak_first()->get_data();
    }
    note_data* get_last() {
        return (note_data*)notelist->sneak_last()->get_data();
    }
    note_data* get_prev() {
        return (note_data*)notelist->sneak_prev()->get_data();
    }
    note_data* get_next() {
        return (note_data*)notelist->sneak_next()->get_data();
    }
    // errr, umm
    note_data* get_current() {
        return (note_data*)notelist->sneak_current()->get_data();
    }
    // these give direct access to list
    ll_item* sneak_first(){ return notelist->sneak_first(); }
    ll_item* sneak_last(){ return notelist->sneak_last(); }
    // beats per bar grabbed from output of
    // time map module, input into sequencer.
    double calc_riff_length(char beats_per_bar, char beat_value);
#ifndef BARE_MODULES
    // virtuals from dobj
    stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE);
    dobj* add_dobj(dobj*);
#endif
private:
    short quarter_val;
    note_data* note;
    linkedlist* notelist;
    ll_item* note_item;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif // RIFF_H
