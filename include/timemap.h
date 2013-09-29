#ifndef TIMEMAP_H
#define TIMEMAP_H

#include "synthmod.h"
#include "linkedlist.h"
#include "bpmchange.h"
#include "meterchange.h"

// (snip overly verbose comments)
//
// timemap
//
//  wcnt no longer desires to read the bpm and time signature withing
//  the wcnt 'header' - just the sample rate.
//
//  which means the timemap no longer initialises to the bpm and
//  time signature set in the header (as it not exist).
//
//  so, the user must create when using the timemap, a bpm change
//  and a meter change. which really speaking, is how it should be.

class timemap : public synthmod
{
public:
    enum { TPQN = 6720 }; // Ticks Per Quarter Note
    timemap(const char*);
    ~timemap();

    bpmchange* add_bpm_change(bpmchange *);
    meterchange* add_meter_change(meterchange*);

    // general purpose method used for checking by other modules,
    // bool check_position(short bar, double pos, short quarter_val);
    // virtual methods
    void run();
    void init(); // init will grab global bpm to start with
    stockerrs::ERR_TYPE validate();
    const void* get_out(output::TYPE) const;
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
    linked_list<bpmchange>*   bpm_map;
    linked_list<meterchange>* meter_map;
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
    unsigned long notelen_to_samples(short) const;
    #ifdef UNUSED
    unsigned long ms_to_samples(double) const;
    double notelen_to_frequency(short) const;
    double notelen_to_ms(short) const;
    #endif
    // synthmod stuff for keeping things cushdy.
    void init_first();
};

#endif
