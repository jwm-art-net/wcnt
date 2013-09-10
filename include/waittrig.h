#ifndef WAITTRIG_H
#define WAITTRIG_H

#include "synthmodule.h"

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
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    STATUS const* in_trig1;
    STATUS const* in_trig2;
    STATUS out_trig;
    STATUS out_wait_state;
    double min_time;
    double max_time;
    short count;
    long min_samples;
    long max_samples;
    long mins;
    long maxs;
    short counter;
    void create_params();
    static bool done_params;
};

#endif
