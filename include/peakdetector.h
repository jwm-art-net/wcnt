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

class peak_detector : public synthmod::base
{
public:
    peak_detector(const char*);
    ~peak_detector();
    void run();
    void init();
    const void* get_out(output::TYPE) { return 0; }
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    errors::TYPE validate();

private:
    const double* in_signal;
    double sig_range_hi;
    double sig_range_lo;
    char* message;
    STATUS force_abort;
    wcint_t max_peaks;
    wcint_t peak_count;
    bool check; // prevent wrap-around of peak_count.
    void set_message(const char* msg);
    void register_ui();
};

#endif
