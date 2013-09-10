#ifndef WAITTRIG_H
#define WAITTRIG_H

#include "synthmodule.h"
#include "conversions.h"

#ifndef BARE_MODULES
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"
#endif

/*
    only outputs trigger one, when it is triggered, but not
    before trigger two triggers.  repeat.

    minimum and maximum wait times can be set also. nb:

        if the minimum time has not expired when trigger two triggers,
        then ignore until trigger two triggers again.

        if the maximum time has expired and trigger two has not 
        triggered, then output trigger at the next chance (ie when
        trigger one next triggers)

        wait times of zero mean wait times ignored.
*/

class waittrig: public synthmod
{
public:
    waittrig(char const*);
    ~waittrig();
    void set_input_trig1(STATUS const* it){ in_trig1 = it; }
    void set_input_trig2(STATUS const* it){ in_trig2 = it; }
    STATUS const* get_input_trig1(){ return in_trig1;}
    STATUS const* get_input_trig2(){ return in_trig2;}
    STATUS const* get_output_trig(){ return &out_trig;}
    void set_min_time(double);
    void set_max_time(double);
    // virtual funcs
    void run();
    stockerrs::ERR_TYPE validate();
#ifndef BARE_MODULES
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);
#endif

private:
    STATUS const* in_trig1;
    STATUS const* in_trig2;
    STATUS out_trig;
    double min_time;
    double max_time;
    long min_samples;
    long max_samples;
    long mins;
    long maxs;
    STATUS trig_pending;
    static int waittrig_count;
#ifndef BARE_MODULES
    void create_params();
    static bool done_params;
#endif
};

#endif
