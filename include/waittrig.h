#ifndef WAITTRIG_H
#define WAITTRIG_H

#include "synthmod.h"

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
    waittrig(const char*);
    ~waittrig();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    STATUS const* in_trig1;
    STATUS const* in_trig2;
    STATUS out_trig;
    STATUS out_wait_state;
    double min_time;
    double max_time;
    wcint_t count;
    long min_samples;
    long max_samples;
    long mins;
    long maxs;
    wcint_t counter;
    void init_first();
    
};

#endif
