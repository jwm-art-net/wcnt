#ifndef ADSR_H
#define ADSR_H

#include "synthmodule.h"
#include "adsrcoord.h"
#include "linkedlist.h"

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
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    dobj* add_dobj(dobj*);
    synthmod* duplicate_module(const char* uname, DUP_IO);

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
    static bool done_params;
    void create_params();
    static bool done_moddobj;
    void create_moddobj();
};

#endif
