#ifndef TRIGCOUNTER_H
#define TRIGCOUNTER_H

#include "synthmod.h"

class trigcounter : public synthmod
{
public:
    trigcounter(char const*);
    ~trigcounter();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    // inputs
    const STATUS* in_trig;
    const STATUS* in_reset_trig;
    // outputs
    STATUS out_trig;
    STATUS out_not_trig;
    short  out_pre_count;
    short  out_count;
    STATUS out_play_state;
    // params
    short pre_count;
    short count;
    STATUS wrap;
    void create_params();
    static bool done_params;
};

#endif
