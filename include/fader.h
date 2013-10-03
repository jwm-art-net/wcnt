#ifndef FADER_H
#define FADER_H

#include "synthmod.h"

class fader: public synthmod
{
 public:
    fader(const char*);
    ~fader();

    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    const STATUS* in_bar_trig;
    const short*  in_bar;

    double out_output;
    STATUS out_bar_trig;
    short out_bar;
    STATUS out_play_state;

    short start_bar;
    short end_bar;
    double fade_in_time;
    double fade_out_time;

    samp_t fade_in_smps;
    samp_t fismp;
    samp_t fade_out_smps;
    samp_t fosmp;

    double fisz;
    double fosz;

    void init_first();
    
};

#endif
