#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include "synthmodule.h"

/* decay time of 0 means do not decay to zero */

class sample_hold : public synthmod
{
public:
    sample_hold(char const*);
    ~sample_hold();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double decay_time;
    unsigned long decay_samps;
    unsigned long ds;
    double decay_size;
    void create_params();
    static bool done_params;
};

#endif
