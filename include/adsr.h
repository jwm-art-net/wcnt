#ifndef ADSR_H
#define ADSR_H

#include "adsrcoord.h"
#include "linkedlist.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#include "dobjparamlist.h"
#include "moddobjlist.h"


/*
ADSR - Attack Decay Sustain Release Envelope
--------------------------------------------
not quite your standard ADSR, yes it has the attack, decay, sustain,
release sections, but any number of sub sections can be added Another
un-standard feature is that each ADSR is really two ADSRs: lower and
upper. Using the velocity input will modulate the output between these
two adsrs. The value of the velocity input is only checked at the 
begining of each section or sub section not constantly. You can also
turn OFF zero re-triggering and the sustain section if you wish.

what's new in wcnt-1.1001?

upper_thresh and lower_thresh which are threshholds by the sound of it.
all values above upper_thresh use the upper envelope, while all values
below lower_thresh use the lower envelope. simple, but nice.

what's new in wcnt-1.128?
    new parameters:
      release_is_ratio
        sets levels in release section to ratios when ON
      min_time
        minimum amount of time before note_off can occur.
      max_sus_time
        maximum execution time of sustain
*/

class adsr : public synthmod
{
public:
    adsr(char const* uname);
    ~adsr();
    adsr_coord* insert_coord(adsr_coord*);
    adsr_coord* insert_coord(
        adsr_coord::SECT, double ut, double ul, double lt, double ll);
    adsr_coord* insert_coord_after(adsr_coord*);
    adsr_coord* insert_coord_after(
        adsr_coord*, double ut, double ul, double lt, double ll);
    void delete_coord(adsr_coord*);
    adsr_coord* goto_section(adsr_coord::SECT);
    adsr_coord* goto_first() {
        return coord = (adsr_coord*)
         (coord_item = env->goto_first())->get_data(); 
    }
    adsr_coord* goto_last() {
        return coord = (adsr_coord*)
        (coord_item = env->goto_last())->get_data();
    }
    adsr_coord* goto_prev() {
        return coord = (adsr_coord*)
         (coord_item = env->goto_prev())->get_data();
    }
    adsr_coord* goto_next() {
        return coord = (adsr_coord*)
         (coord_item = env->goto_next())->get_data();
    }
    void scale_section(adsr_coord::SECT, double ratio);
    // inputs
    void set_input_velocity(double const* v){ in_velocity = v;}
    void set_input_note_on_trig(STATUS const* ot){ in_note_on_trig = ot;}
    void set_input_note_off_trig(STATUS const* ot){in_note_off_trig = ot;}
    double const* get_input_velocity(){ return in_velocity;}
    STATUS const* get_input_note_on_trig(){ return in_note_on_trig;}
    STATUS const* get_input_note_off_trig(){ return in_note_off_trig;}
    // outputs
    double const* get_output() { return &output; }
    STATUS const* get_output_off_trig(){ return &out_off_trig;}
    STATUS const* get_play_state() { return &play_state; }
    // params
    void set_upper_thresh(double ut){ up_thresh = ut;}
    void set_lower_thresh(double lt){ lo_thresh = lt;}
    void set_start_level(double sl){ start_level = sl;}
    void set_min_time(double mt){ min_time = mt;}
    void set_max_sustain_time(double mt){ max_sus_time = mt;}
    void set_sustain_status(STATUS s){ sustain_status = s;}
    void set_release_ratio(STATUS r){ release_ratio = r;}
    void set_zero_retrigger_mode(STATUS zrm) { zero_retrigger = zrm; }
    double get_upper_thresh(){ return up_thresh;}
    double get_lower_thresh(){ return lo_thresh;}
    double get_start_level(){ return start_level;}
    double get_min_time(){ return min_time;}
    double get_max_sustain_time(){ return max_sus_time;}
    STATUS get_sustain_status() { return sustain_status; }
    STATUS get_release_ratio(){return release_ratio;}
    STATUS get_zero_retrigger_mode() { return zero_retrigger; }
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
    dobj* add_dobj(dobj*);

private:
    // inputs
    STATUS const* in_note_on_trig;
    STATUS const* in_note_off_trig;
    double const* in_velocity;
    // outputs
    double output;
    STATUS out_off_trig;
    STATUS play_state;
    // params
    double up_thresh;
    double lo_thresh;
    double start_level;
    double min_time;
    double max_sus_time;
    STATUS sustain_status;
    STATUS release_ratio;
    STATUS zero_retrigger;
    // working
    double thresh_range;
    double end_level;
    STATUS released;
    unsigned long tsamps;
    unsigned long min_samps;
    unsigned long max_samps;
    unsigned long max_sus_samps;
    linkedlist* env;
    short sect;
    unsigned long sectsample;
    unsigned long sectmaxsamples;
    double levelsize;
    ll_item* coord_item;
    adsr_coord* coord;
    void ready_section();
    static short adsr_count;
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
