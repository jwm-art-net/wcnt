#ifndef CONTRASTER_H
#define CONTRASTER_H

// as if you did n't guess, SND inspired this one. (SND is a sound editor)
//
// it multiplies a given signal by x amount.  if the result is not
// within the range (-1 ~ 1) it mirrors it until it is.
// something like that.
//
// yeah, um, right, my version does not do quite the same thing, but it
// does something to the sound which is enough in my book....

#include "synthmodule.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

class contraster : public synthmod
{
public:
    contraster(char const*);
    ~contraster();
    // inputs
    void set_input_signal(const double* in){ in_signal = in; }
    void set_input_power_mod(const double* pm){ in_power_mod = pm; }
    void set_input_rude_switch_trig(const STATUS* rs) {
        in_rude_switch_trig = rs;
    }
    const double* get_input_signal(){ return in_signal; }
    const double* get_input_power_mod(){ return in_power_mod; }
    const STATUS* get_input_rude_switch_trig() {
        return in_rude_switch_trig;
    }
    // outputs
    const double* get_output(){ return &out_output;}
    // params
    void set_power_min(double pmn){ power_min = pmn;}
    void set_power_max(double pmx){ power_max = pmx;}
    void set_rude_mode(STATUS rm){ rude_mode = rm;}
    void set_wetdry(double wd){ wetdry = wd;}
    double get_power_min(){ return power_min;}
    double get_power_max(){ return power_max;}
    STATUS get_rude_mode(){ return rude_mode;}
    double get_wetdry(){ return wetdry;}
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
    double out_output;
    const double* in_signal;
    const double* in_power_mod;
    const STATUS* in_rude_switch_trig;
    double power_min;
    double power_max;
    STATUS rude_mode;
    double wetdry;
    double power;
    double power_mod;
    double powerrad;
    double output;
    static int contraster_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
