#ifndef OSCCLOCK_H
#define OSCCLOCK_H

#include "synthmod.h"

/*
    (v.1.128)
    new input - in_play_state
        if *in_play_state == ON use respone time, otherwise don't.

    new parameter - response time
        how many ms to respond to a normal frequency change
        ie for note_on_trig
        (as opposed to note slide trig & portamento time)
*/

class osc_clock : public synthmod::base
{
public:
    osc_clock(const char*);
    ~osc_clock();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
// outputs
    STATUS out_phase_trig;
    double out_phase_step;
    double out_premod_phase_step;
// inputs
    const double* in_freq;
    const STATUS* in_note_on_trig;
    const STATUS* in_note_slide_trig;
    const STATUS* in_play_state;
    const double* in_freq_mod1;
    const double* in_freq_mod2;
// parameters
    wcint_t octave_offset;
    double semitones;
    double freq_mod1size;
    double freq_mod2size;
    double mod1size;
    double mod2size;
    double degs;
    double degsize1;
    double degsize2;
    double portamento;
    double response_time;
// working
    double slide_size;
    double target_phase_step;
    samp_t slidesamples;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
