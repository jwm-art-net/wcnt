#ifndef RMS_H
#define RMS_H

#include "synthmodule.h"

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
    rms(char const*);
    ~rms();
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
    void create_params();
    static bool done_params;
};

#endif
