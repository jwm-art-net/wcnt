#ifndef RMS_H
#define RMS_H

#include "synthmod.h"

/*
// rms
// calculate the root mean square of a given signal
// step 1) calculate the square of a sample, and place in array
// step 2) calculate the sum of the array
// step 3) calculate the mean average of the array
// step 4) calculate the square root of mean average
*/

class rms : public synthmod
{
public:
    rms(const char*);
    ~rms();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const double* in_signal;
    // outputs
    double out_rms;
    // params
    double rms_time;
    // working
    double output;
    double* rmsarr;//array
    short arraymax;
    short arrpos;
    double sqrsum;
    void init_first();
    
};

#endif
