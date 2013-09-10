#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

// decay time of 0 means do not decay to zero

class sample_hold : public synthmod
{
public:
    sample_hold(char const*);
    ~sample_hold();
    void set_input_trig(const STATUS* trg){ in_trig = trg;}
    void set_input_signal(const double* in){ in_signal = in;}
    const STATUS* get_input_trig(){ return in_trig;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_output(){ return &output;}
    void set_decay_time(double ms){ decay_time = ms;}
    double get_decay_time(){ return decay_time;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif
private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double decay_time;
    unsigned long decay_samps;
    unsigned long ds;
    double decay_size;
    static int sample_hold_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
