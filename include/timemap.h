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
    // bool check_position(wcint_t bar, double pos, wcint_t quarter_val);
    // virtual methods
    void run();
    void init(); // init will grab global bpm to start with
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    synthmod* duplicate_module(const char* uname, DUP_IO);
    dobj* add_dobj(dobj*);

private:
    // outputs
    wcint_t out_bar;
    STATUS out_bar_trig;
    samp_t out_pos_in_bar;
    double out_pos_step_size;
    double out_bpm;
    samp_t out_sample_total;
    samp_t out_sample_in_bar;
    wcint_t out_beats_per_bar;
    wcint_t out_beat_value;
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
    samp_t bpmsampletot;
    samp_t bpmchangesamp;
    double bpmchange_pos;
    double bpmrampsize;
    double bpmchange_ratio;
    double targbpm;
    double pos_in_bar;
    samp_t bpmchange_notelen;
    wcint_t bpmchangebar;
    double barlength;
    wcint_t beatlength;
    wcint_t meterchangebar;
    // working methods
    double p_bpm;
    samp_t notelen_to_samples(wcint_t) const;
    #ifdef UNUSED
    samp_t ms_to_samples(double) const;
    double notelen_to_frequency(wcint_t) const;
    double notelen_to_ms(wcint_t) const;
    #endif
    // synthmod stuff for keeping things cushdy.
    void init_first();
};

#endif
