#ifndef ADSR_H
#define ADSR_H

#include "synthmod.h"
#include "adsrcoord.h"
#include "linkedlist.h"
#include "listwork.h"


// ADSR - Attack Decay Sustain Release Envelope
// --------------------------------------------
//  not quite your standard ADSR, yes it has the attack, decay, sustain,
//  release sections, but any number of sub sections can be added Another
//  un-standard feature is that each ADSR is really two ADSRs: lower and
//  upper. Using the velocity input will modulate the output between these
//  two adsrs. The value of the velocity input is only checked at the 
//  begining of each section or sub section not constantly. You can also
//  turn OFF zero re-triggering and the sustain section if you wish.
//  
//  what's new in wcnt-1.1001?
//  
//  upper_thresh and lower_thresh which are threshholds by the sound of
//  it. all values above upper_thresh use the upper envelope, while all
//  values below lower_thresh use the lower envelope. simple, but nice.
//  
//  
//  what's new in wcnt-1.128?
//      new parameters:
//        release_is_ratio
//          sets levels in release section to ratios when ON
//        min_time
//          minimum amount of time before note_off can occur.
//        max_sus_time
//          maximum execution time of sustain

class adsr : public synthmod::base, public linked_list<adsr_coord>
{
public:
    adsr(const char* uname);
    ~adsr();

    adsr_coord* insert_coord(adsr_coord*);
    adsr_coord* insert_coord(
        adsr_coord::SECT, double ut, double ul, double lt, double ll);
    adsr_coord* insert_coord_after(adsr_coord*);
    adsr_coord* insert_coord_after(
        adsr_coord*, double ut, double ul, double lt, double ll);

    #ifdef UNUSED
    void scale_section(adsr_coord::SECT, double ratio);
    #endif

    adsr_coord* goto_section(adsr_coord::SECT sect) {
		llitem* tmp = find_in_data(sneak_first(), sect);
        return tmp ? tmp->get_data() : 0;
    }

    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool        set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    dobj::base* add_dobj(dobj::base*);
    synthmod::base* duplicate_module(const char* uname, DUP_IO);

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
    samp_t tsamps;
    samp_t min_samps;
    samp_t max_samps;
    samp_t max_sus_samps;
    wcint_t sect;
    samp_t sectsample;
    samp_t sectmaxsamples;
    double levelsize;
    adsr_coord** run_coords;
    adsr_coord* coord;
    long coord_ix;
    long decay_ix;
    long sustain_ix;
    long release_ix;
    void ready_section();
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
