#ifndef RIFFNODE_H
#define RIFFNODE_H

#include "riffdata.h"
#include "dobj.h"
#include "miscfuncobj.h"

/*
class riff_node 
---------------
to 'ghost' one riff many times without duplication of riffdata.

riffs are n't tied to any particular sequencer.  therefore, sequencer
cannot use the riff's own methods for navigating through the notes. in
theory, one riff could potentially be used by multiple sequencers
simultaneously which could possibly be running at different tempos and/or
time signatures.

    transpose 
    how many semitones to transpose the riff when sequenced

    *** removed start_length and end_length ***
    start_length/end_length
    adjust the length of the notes as riff progresses.  decided it should
    not be a ratio, but an offset to add to each note's length.  NOTE:
    you have two riffs, the first with a quarter_val of 16, the second
    with a quarter_val of 64.  When the riffs are inserted, the 
    start and end lengths of each are set so that 8 is added to 
    the length of each note.  how much longer will the notes become?
    for the first riff (qval 16) each note will become a 8th note (quaver)
    longer, while for the second riff (qval 64) each note will become a 
    32nd note longer (demi-semiquaver),  big difference you see/hear?

    *** removed start velocity and end velocity ***
    I added it and took it away again, there would not be much use for it 
    that could not be better implementated by other ways. such as the soon
    to be added controler module.  When used in combination with a sample
    and hold module, could achieve the same effect.  NOTE soon to be added
    contoler module will be added tommorrow... [24thSept2006 ha ha ha
    tommorrow never comes!!]

    *** NOTE ***
    i had intended for a method to calculate the length of the riff to the
    nearest bar (not a count of bars), but this may depend on what time
    signature is being used at the point at which the riff is inserted.

    *** NOTE 2 ***
    have placed formentioned method in riffdata, requires both
    elements of the time signature to be passed through to it.

    *** new to wcnt-1.25 ***
    ------------------------

    1) adding repeat parameter, to tell the sequencer to repeatedly
       add the riff to it's rifflist.

    2) adding repeat_stripe, when repeatedly adding a riff, do it
       every n bars.

    3) adding duplicate_for_bar method, to duplicate the riff_node
       itself, but changing the bar position, returns the new riff_node.

    4) removal of start_length and end_length parameters, they were a
       little clumsy I'll freely admit, use the riff_editor for a 
       similiar effect. Furthermore, the note editing facilities of
       riff_data handle things in a better way, and mean the removal
       of extraneous code from the sequencer.

*/

class riff_node : public dobj::base
{
public:
    riff_node();
    riff_node(riffdata*, wcint_t barpos);
    ~riff_node();
    riff_node* duplicate_for_bar(wcint_t barpos);
    void set_start_bar(wcint_t bp)        { start_bar = bp; }
    void set_riff_source(riffdata* rs)  { riff_source = rs; }
    void set_transpose(wcint_t t)         { transpose = t; }
    void set_repeat(wcint_t r)            { repeat = r;}
    void set_repeat_stripe(wcint_t rs)    { repeat_stripe = rs;}

    wcint_t get_start_bar() const         { return start_bar; }
    riffdata* get_riff_source() const   { return riff_source; }
    wcint_t get_transpose() const         { return transpose; }
    wcint_t get_repeat() const            { return repeat;}
    wcint_t get_repeat_stripe() const     { return repeat_stripe;}
    virtual errors::TYPE validate();
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE pt) const;

    bool operator()(_start_bar_ & n) {
        return n(start_bar);
    }

private:
    wcint_t start_bar;
    riffdata* riff_source;
    wcint_t transpose;
    wcint_t repeat;
    wcint_t repeat_stripe;
    void register_ui();
};

#endif
