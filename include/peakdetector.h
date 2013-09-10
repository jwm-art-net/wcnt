#ifndef PEAKDETECTOR_H
#define PEAKDETECTOR_H

#include "synthmodule.h"
#include "dobj.h"

#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

/*
    peak_detector

    it detects when the input signal goes above sig_range_hi, and
    below sig_range_lo.

    when such a thing happens, it displays a message as chosen by
    the user. It can do this until the cows come home when the user
    says zero times, or, it can stop after so many peaks. it can
    force wcnt to abort when max peaks is reached, unless max peaks
    is set to zero or it is told not to.

    it has no outputs.

*/

class peak_detector : public synthmod
{
public:
    peak_detector(char const*);
    ~peak_detector();
    void set_input_signal(const double* is){ in_signal = is;}
    const double* get_input_signal(){ return in_signal;}

    void set_signal_range_hi(double sh){ sig_range_hi = sh;}
    void set_signal_range_lo(double sl){ sig_range_lo = sl;}
    void set_message(const char* msg);
    void set_force_abort(STATUS fa){ force_abort = fa;}
    void set_max_peaks(short mp){ max_peaks = mp; }
    double get_signal_range_lo(){ return sig_range_hi;}
    double get_signal_range_hi(){ return sig_range_lo;}
    const char* get_message() { return message;}
    STATUS get_force_abort(){ return force_abort;}
    short get_max_peaks(){ return max_peaks;}

    // virtual funcs
    void run();
    void init();
    void const* get_out(outputnames::OUT_TYPE) { return 0; }
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    const double* in_signal;
    double sig_range_hi;
    double sig_range_lo;
    char* message;
    STATUS force_abort;
    short max_peaks;
    short peak_count;
    double insig;
    static int peak_detector_count;
    void create_params();
    static bool done_params;
};

#endif
