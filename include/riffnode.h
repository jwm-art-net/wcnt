#ifndef RIFFNODE_H
#define RIFFNODE_H

#include "riffdata.h"
#include "dobjparamlist.h"

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
    contoler module will be added tommorrow...

    *** NOTE ***
    i had intended for a method to calculate the length of the riff to the
    nearest bar (not a count of bars), but this may depend on what time
    signature is being used at the point at which the riff is inserted.

    *** NOTE 2 ***
    have placed formentioned method in riffdata, requires both
    elements of the time signature to be passed through to it.
*/

class riff_node : public dobj
{
public:
    riff_node();
    riff_node(riffdata*, short barpos);
    ~riff_node();
    void set_start_bar(short bp){ start_bar = bp; }
    void set_riff_source(riffdata* rs){ riff_source = rs; }
    void set_transpose(short t){ transpose = t; }
    void set_start_length(double sl){ startlen = sl;}
    void set_end_length(double el){ endlen = el;}
    short get_start_bar(){ return start_bar; }
    riffdata* get_riff_source(){ return riff_source; }
    short get_transpose(){ return transpose; }
    double get_start_length(){ return startlen;}
    double get_end_length(){ return endlen;}
#ifndef BARE_MODULES
    virtual stockerrs::ERR_TYPE validate();
    bool set_dparam(dparamnames::DPAR_TYPE, void*);
    void* get_dparam(dparamnames::DPAR_TYPE pt);
#endif
private:
    short start_bar;
    riffdata* riff_source;
    short transpose;
    double startlen;
    double endlen;
#ifndef BARE_MODULES
    void create_dparams();
    static bool done_dparams;
#endif
};

#endif
