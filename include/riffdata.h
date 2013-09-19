#ifndef RIFFDATA_HH
#define RIFFDATA_HH

#include "notedata.h"
#include "dobj.h"
#include "linkedlist.h"

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
their requirements...constructor default is 12.

------------------
*** ammendment ***
------------------
added calc_riff_length(...) method to get the length of the riff. 
the value returned is in terms of a note length with a quarter value 
as set for the riff, giving the length of the number of bars the riff
occupies. ie it does not return how many bars the riff occupies, but
the note length of those bars.

the method requires both elements of the time signature to be passed.

***************
   wcnt-1.25
***************

    the note_data* edit_notes(note_data* editnote) method is called
    by insert_and_position_note whenever the note to be inserted and
    positioned is an edit note. editnotes still use note_data but are
    not notes which can be played. They are not stored along with normal
    notes, but instead have their own seperate linkedlist, editlist.

*/

class riffdata : public dobj, public linked_list<note_data>
{
 public:
    riffdata();
    ~riffdata();

    /*
    // needed (kinda, atleast for ease of...)
    */
    void set_quartervalue(short qv) { quarter_val = qv;}
    short get_quartervalue() const  { return quarter_val;}

    note_data* insert_and_position_note(note_data*);
    note_data* add_edit_note(note_data* en) {
        if (en->get_note_type() != note_data::NOTE_TYPE_EDIT)
            return 0;
        return editlist->add_at_tail(en)->get_data();
    };
    note_data* edit_notes(note_data* editnote);

    /*
    // beats per bar grabbed from output of
    // time map module, input into sequencer.
    */
    double calc_riff_length(char beats_per_bar, char beat_value);

    /*
    // virtuals from dobj
    */
    stockerrs::ERR_TYPE validate();
    bool set_param(paramnames::PAR_TYPE, void*);
    void const* get_param(paramnames::PAR_TYPE) const;
    dobj const* add_dobj(dobj*);
    dobj* duplicate_dobj(const char*);

 private:
    short quarter_val;

    linkedlist* editlist;

    double calc_note_param(note_data::NOTE_OP, double, double);
    void init_first();
    
};

#endif // RIFF_H
