#ifndef PEAKDETECTOR_H
#define PEAKDETECTOR_H

#include "synthmod.h"

/*
//  peak_detector
//  it detects when the input signal goes above sig_range_hi, and
//  below sig_range_lo.
//  when such a thing happens, it displays a message as chosen by
//  the user. It can do this until the cows come home when the user
//  says zero times, or, it can stop after so many peaks. it can
//  force wcnt to abort when max peaks is reached, unless max peaks
//  is set to zero or it is told not to.
//  it has no outputs.

*/

class peak_detector : public synthmod
{
public:
    peak_detector(char const*);
    ~peak_detector();
    void run();
    void init();
    void const* get_out(outputnames::OUT_TYPE) { return 0; }
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    const double* in_signal;
    double sig_range_hi;
    double sig_range_lo;
    char* message;
    STATUS force_abort;
    short max_peaks;
    short peak_count;
    bool check; // prevent wrap-around of peak_count.
    void set_message(const char* msg);
    void init_first();
    
};

#endif
