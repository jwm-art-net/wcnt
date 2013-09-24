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
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

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

    short _end_bar;

    double fisz;
    double fosz;

    void init_first();
    
};

#endif
