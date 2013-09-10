#ifndef RMS_H
#define RMS_H

#include <math.h>

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

// rms
// calculate the root mean square of a given signal
//
// step 1) calculate the square of a sample, and place in array
// step 2) calculate the sum of the array
// step 3) calculate the mean average of the array
// step 4) calculate the square root of mean average

class rms : public synthmod
{
public:
    rms(char const*);
    ~rms();
    void set_input_signal(const double* in){ in_signal = in;}
    const double* get_input_signal(){ return in_signal;}
    const double* get_output_rms(){ return &out_rms;}
    void set_rms_time(double dt){ rms_time = dt;}
    double get_rms_time(){ return rms_time;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

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
    static int rms_count;
    void create_params();
    static bool done_params;
};

#endif
