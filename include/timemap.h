#ifndef TIMEMAP_H
#define TIMEMAP_H

#include "synthmodule.h"
#include "conversions.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"

#include "bpmchange.h"
#include "meterchange.h"

/*
    ****** NOTE ******

    Tempo changes are now relative instead of absolute, so bear that
    in mind when reading info below...

    **** END NOTE ****

    timemap
    ======

    maps the bpm, and time signature/meter 

    provides bar position and position in the bar
    for sequencer etc to use.

    makes a linked list of bpm changes storing bar position and bpm.

    ie bpm = 100 @ bar 0,  bpm = 120 @ bar 4, timemap will make a 
    smooth ramp gradually increasing the bpm from 100 to be 120 at
    the beginning of bar 4.

    ie bpm = 100 @ bar 0, bpm = 100 @ bar 4, bpm = 120 @ bar 4
    timemap will maintain bpm of 100 until bar 4, when it will 
    suddenly increase to 120 bpm.

    ie bpm = 100 @ bar 0, bpm = 120 @ bar 4, bpm = 100 @ bar 4
    timemap will gradually increase the bpm until it reaches 120
    at beginning of bar 4, and will then jump back to 100 bpm.

    the above two examples illustrate the importance of adding
    bpm tempo changes in specific order.

    -------------
    Thanks to Tim Goetze on the LAD mailing list for clarifying time 
    signature and bpm calculations.


    Previously on bpmmap
    --------------------
    this module was previously named bpmmap at the start of the
    wcnt 1.1001 update.  It did not exist in prior versions.
    Named it timemap to accommodate time signature change.  I had 
    previously hardcoded a value of 64 for a quarter, and 256 for 
    bar length.  That was fine for 4/4 but bar length would be
    wrong for other time signatures.  Now quarter value is 6720 which
    will give 1/256 septuplets a value of 105. That gives plenty of 
    scope for fine tuning of note positions and lengths

    Previously on sequencer (prior to wcnt 1.1001):
    ===============================================
    the original sequencer module calculates the number of samples
    between each note according to the global (unchanging) bpm.  
    Now using timemap, the new sequencer module only checks the 
    position of the note with time code output by timemap.  There 
    is no need for each sequencer, in order to know when to trigger 
    note on/off etc, to calculate the note's sample positions, and 
    because it no longer works in that way it can handle tempo change
    and time signature changes.

    wcnt header information
    -----------------------
    wcnt file header information contains bpm and time signature.  
    timemap uses this to initialise itself with a bpmchange and 
    meter change at bar 0.  In the current wcnt setup, ie no GUI,
    just a filereader, this is not much use as the user will have
    to define a timemap as no default will be created but in other
    situations not yet implimented it maybe useful.  Or not.

    insertion rules
    ---------------

    BPM
    only one bpmchange should be allowed at bar 0:
        it will not be a ramp change as it will be immediate
    no more than two bpm changes should be allowed at any one
    single bar:
        would otherwise need unneccessary checking code

    METER
    only one timesignature change at any one bar.  inserting two
    or more meter changes at one bar, the last will be used.
    you may change time signature/meter during a bpm ramp. ie
    bpm=120@bar0 bpm=160@bar2 m=4/4@bar0 m=3/4@bar1.  Should work 
    ok I think......

    Outputs
    =======

    out_bpm_change_state is set to ON whenever the bpm changes, ie
    it will be ON for one sample only if the bpm jumps, and it will
    be on for the entire duration of a bpm ramp.

    New Method
    ==========

    bool check_position(short bar, double pos, short quarter_val);

    returns true if the specified position within the bar does not
    conflict with the time signature at that bar.

    returns false if the position exceeds the bar length.

    obviously, it is no use using this method until all time signature
    changes have been created and inserted.  generally used from the
    validate method of some other module.  (validate is called before
    the synth begins it's run().)  it has been created because the
    controller module needs it for creating ramps from one position in
    one bar, to another position in some other bar.  things would get 
    rather messy if a ctrlpoint was not at a valid position.  

    why is it as soon as I've thought about a problem for a while and then
    decide on the solution, and say why I've decided that is the correct
    sollution, that I think of another, perhaps better sollution, which 
    completely goes against what I've just claimed?

    Think of the adsr.  If on note_off the adsr has not yet reached the
    sustain section, then the ramp is created from the current level
    to the release level.  So, if a ctrlpoint is not reached because it's
    position is not within a bar because the time signature creates a
    shorter bar, then the ramp is taken from the current level to the
    next level, and no need for a poxy check. hoorah!  which is why I've
    not yet created the said method.

    ___...---***still thinking this through***---...___	
    
    note: 
        no validate(), nothing to validate.  
        dobj handles validation of the bpm and meter changes.
*/

class timemap : public synthmod
{
public:
    enum { QUARTER_VALUE = 6720 };
    timemap(char const*);
    ~timemap();
    // list insertion and navigation methods
    bpmchange* add_bpm_change(short atbar, double bpm);
    bpmchange* add_bpm_change(bpmchange *);
    bpmchange* goto_first_bpm() {
        return currentbpm = (bpmchange*)
         (bpm_item = bpm_map->goto_first())->get_data();
    }
    bpmchange* goto_last_bpm() {
        return currentbpm = (bpmchange*)
         (bpm_item = bpm_map->goto_last())->get_data();
    }
    bpmchange* goto_prev_bpm() {
        return currentbpm = (bpmchange*)
         (bpm_item = bpm_map->goto_prev())->get_data();
    }
    bpmchange* goto_next_bpm() {
        return currentbpm = (bpmchange*)
         (bpm_item = bpm_map->goto_next())->get_data();
    }
    bpmchange* get_next_bpm(){
        return (bpmchange*)(bpm_item->get_next())->get_data();
    }
    bpmchange* get_prev_bpm(){
        return (bpmchange*)(bpm_item->get_prev())->get_data();
    }
    meterchange* add_meter_change(short atbar, char btpb, char btval);
    meterchange* add_meter_change(meterchange*);
    meterchange* goto_first_meter() {
        return currentmeter = (meterchange*)
         (meter_item = meter_map->goto_first())->get_data();
    }
    meterchange* goto_last_meter() {
        return currentmeter = (meterchange*)
         (meter_item = meter_map->goto_last())->get_data();
    }
    meterchange* goto_prev_meter() {
        return currentmeter = (meterchange*)
         (meter_item = meter_map->goto_prev())->get_data();
    }
    meterchange* goto_next_meter() {
        return currentmeter = (meterchange*)
         (meter_item = meter_map->goto_next())->get_data();
    }
    // general purpose method used for checking by other modules,
    //	bool check_position(short bar, double pos, short quarter_val);
    // outputs access:
    short const* get_out_bar(){return &out_bar;}
    unsigned long const* get_out_pos_in_bar(){return &out_pos_in_bar;}
    double const* get_out_pos_step_size(){return &out_pos_step_size;}
    double const* get_out_bpm(){return &out_bpm;}
    unsigned long const* get_out_sample_total(){return &out_sample_total;}
    unsigned long const* get_out_sample_in_bar() {
        return &out_sample_in_bar;
    }
    STATUS const* get_out_bar_trig(){return &out_bar_trig;}
    short const* get_out_beats_per_bar(){ return &out_beats_per_bar;}
    short const* get_out_beat_value(){ return &out_beat_value;}
    STATUS const* get_out_bpm_change_trig(){ return &out_bpm_change_trig;}
    STATUS const* get_out_meter_change_trig() {
        return &out_meter_change_trig;
    }
    STATUS const* get_out_bpm_change_state() {
        return &out_bpm_change_state;
    }
    // virtual methods
    void run();
    void init(); // init will grab global bpm to start with
    void const* get_out(outputnames::OUT_TYPE);
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);

private:
    // outputs
    short out_bar;
    STATUS out_bar_trig;
    unsigned long out_pos_in_bar;
    double out_pos_step_size;
    double out_bpm;
    unsigned long out_sample_total;
    unsigned long out_sample_in_bar;
    short out_beats_per_bar;
    short out_beat_value;
    STATUS out_bpm_change_trig;
    STATUS out_meter_change_trig;
    STATUS out_bpm_change_state;
    // list variables
    ll_item* bpm_item;
    ll_item* meter_item;
    //	bpmchange* bpm_change;
    linkedlist* bpm_map;
    linkedlist* meter_map;
    // working variables
    bpmchange* currentbpm;
    bpmchange* targetbpm;
    meterchange* currentmeter;
    unsigned long bpmsampletot;
    unsigned long bpmchangesamp;
    double bpmchange_pos;
    double bpmrampsize;
    double bpmchange_ratio;
    double targbpm;
    double pos_in_bar;
    unsigned long bpmchange_notelen;
    short bpmchangebar;
    double barlength;
    short beatlength;
    short meterchangebar;
    // working methods
    double p_bpm;
    unsigned long notelen_to_samples(short);
    unsigned long ms_to_samples(double);
    double notelen_to_frequency(short);
    double notelen_to_ms(short);
    // synthmod stuff for keeping things cushdy.
    static short timemap_count;
    static bool done_moddobj;
    void create_moddobj();
};

#endif
