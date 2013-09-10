#ifndef PEAKDETECTOR_H
#include "../include/peakdetector.h"

peak_detector::peak_detector(char const* uname) :
 synthmod(synthmodnames::MOD_PEAKDETECTOR, peak_detector_count, uname),
 in_signal(0), sig_range_hi(0.0), sig_range_lo(0.0), message(0),
 force_abort(OFF), max_peaks(0)
{
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
    peak_detector_count++;
}

peak_detector::~peak_detector()
{
    if (message)
        delete [] message;
    get_inputlist()->delete_module_inputs(this);
}

void peak_detector::set_message(const char* msg)
{
    if (message)
        delete [] message;
    message = new char[strlen(msg) + 1];
    strcpy(message, msg);
}

void const* peak_detector::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
    default:
        return 0;
    }
}

void const* peak_detector::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    default:
        return 0;
    }
}

void peak_detector::run()
{
    if ((insig = *in_signal) < sig_range_lo) {
        if (max_peaks > 0)
            peak_count++;
        if (peak_count <= max_peaks || max_peaks == 0) {
            cout << "\n" << message << " ";
            cout << insig << " < " << sig_range_lo;
        }
        else if (force_abort == ON) {
            cout << "\nToo many peaks detected, abort forced! ";
            synthmod::force_abort();
        }
    }
    else if (insig > sig_range_hi) {
        if (max_peaks > 0)
            peak_count++;
        if (peak_count <= max_peaks || max_peaks == 0) {
            cout << "\n" << message << " ";
            cout << insig << " > " << sig_range_hi;
        }
        else if (force_abort == ON) {
            cout << "\nToo many peaks detected, abort forced! ";
            synthmod::force_abort();
        }
    }
}

void peak_detector::init()
{
    if (sig_range_lo > sig_range_hi) {
        double tmp = sig_range_hi;
        sig_range_hi = sig_range_lo;
        sig_range_lo = tmp;
    }
    if (max_peaks < 0) max_peaks = 0;
}

bool peak_detector::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_SIG_RANGE_HI:
        set_signal_range_hi(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_SIG_RANGE_LO:
        set_signal_range_lo(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MSG:
        set_message((char*)data);
        retv = true;
        break;
    case paramnames::PAR_FORCE_ABORT:
        set_force_abort(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_MAXPEAKS:
        set_max_peaks(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* peak_detector::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_SIG_RANGE_HI:
        return &sig_range_hi;
    case paramnames::PAR_SIG_RANGE_LO:
        return &sig_range_lo;
    case paramnames::PAR_MSG:
        return message;
    case paramnames::PAR_FORCE_ABORT:
        return &force_abort;
    case paramnames::PAR_MAXPEAKS:
        return &max_peaks;
    default:
        return 0;
    }
}

bool peak_detector::done_params = false;

void peak_detector::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_PEAKDETECTOR, paramnames::PAR_SIG_RANGE_HI);
    get_paramlist()->add_param(
     synthmodnames::MOD_PEAKDETECTOR, paramnames::PAR_SIG_RANGE_LO);
    get_paramlist()->add_param(
     synthmodnames::MOD_PEAKDETECTOR, paramnames::PAR_MSG);
    get_paramlist()->add_param(
     synthmodnames::MOD_PEAKDETECTOR, paramnames::PAR_FORCE_ABORT);
    get_paramlist()->add_param(
     synthmodnames::MOD_PEAKDETECTOR, paramnames::PAR_MAXPEAKS);
    done_params = true;
}

int peak_detector::peak_detector_count = 0;

#endif
