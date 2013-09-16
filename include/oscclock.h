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

class osc_clock : public synthmod
{
public:
    osc_clock(char const*);
    ~osc_clock();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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
    short octave_offset;
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
    unsigned long slidesamples;
    void create_params();
    
};

#endif
