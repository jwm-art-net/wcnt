#ifndef FADER_H
#define FADER_H

#include "synthmod.h"

class fader: public synthmod
{
 public:
    fader(char const*);
    ~fader();

    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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

    unsigned long fade_in_smps;
    unsigned long fismp;
    unsigned long fade_out_smps;
    unsigned long fosmp;

    short _end_bar;

    double fisz;
    double fosz;

    void create_params();
    static bool done_params;
};

#endif
