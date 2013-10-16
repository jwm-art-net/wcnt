#ifndef SEQECHO_H
#define SEQECHO_H

#include "synthmod.h"

class seq_echo : public synthmod::base
{
 public:
    seq_echo(const char*);
    ~seq_echo();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    // inputs
    const STATUS*   in_note_on_trig;
    const STATUS*   in_note_slide_trig;
    const STATUS*   in_note_off_trig;
    const double*   in_freq;
    const double*   in_velocity;
    const STATUS*   in_reset_trig;
    // outputs
    STATUS  out_note_on_trig;
    STATUS  out_note_slide_trig;
    STATUS  out_note_off_trig;
    double  out_freq;
    double  out_velocity;
    wcint_t out_index;
    // params
    double delay_time;
    wcint_t count;
    STATUS send_input_out;
    // working
    wcint_t* on_trigs;
    wcint_t* slide_trigs;
    wcint_t* off_trigs;
    double* freqs;
    double* vels;
    samp_t pastmax;
    samp_t pastpos;
    wcint_t last;
    void register_ui();
};

#endif
