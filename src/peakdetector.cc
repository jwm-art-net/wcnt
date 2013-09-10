#ifndef PEAKDETECTOR_H
#include "../include/peakdetector.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

#include <iostream>

peak_detector::peak_detector(char const* uname) :
 synthmod(synthmodnames::PEAKDETECTOR, uname),
 in_signal(0), sig_range_hi(0.0), sig_range_lo(0.0), message(0),
 force_abort(OFF), max_peaks(0), check(true)
{
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

peak_detector::~peak_detector()
{
    if (message)
        delete [] message;
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
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* peak_detector::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

void peak_detector::run()
{
    if (check) {
        if (*in_signal < sig_range_lo) {
            if (peak_count < max_peaks) {
                std::cout << "\n" << message << " ";
                std::cout << *in_signal << " < " << sig_range_lo;
            }
            else if (force_abort == ON) {
                std::cout << "\nToo many peaks detected, abort forced!";
                synthmod::force_abort();
            }
            else
                check = false;
            peak_count++;
        }
        else if (*in_signal > sig_range_hi) {
            if (peak_count < max_peaks) {
                std::cout << "\n" << message << " ";
                std::cout << *in_signal << " > " << sig_range_hi;
            }
            else if (force_abort == ON) {
                std::cout << "\nToo many peaks detected, abort forced!";
                synthmod::force_abort();
            }
            else
                check = false;
            peak_count++;
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
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI:
            sig_range_hi = *(double*)data;
            return true;
        case paramnames::SIG_RANGE_LO:
            sig_range_lo = *(double*)data;
            return true;
        case paramnames::MSG:
            set_message((char*)data);
            return true;
        case paramnames::FORCE_ABORT:
            force_abort = *(STATUS*)data;
            return true;
        case paramnames::MAXPEAKS:
            max_peaks = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* peak_detector::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI:  return &sig_range_hi;
        case paramnames::SIG_RANGE_LO:  return &sig_range_lo;
        case paramnames::MSG:           return message;
        case paramnames::FORCE_ABORT:   return &force_abort;
        case paramnames::MAXPEAKS:      return &max_peaks;
        default: return 0;
    }
}

bool peak_detector::done_params = false;

void peak_detector::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::PEAKDETECTOR, paramnames::SIG_RANGE_HI);
    jwm.get_paramlist()->add_param(
        synthmodnames::PEAKDETECTOR, paramnames::SIG_RANGE_LO);
    jwm.get_paramlist()->add_param(
        synthmodnames::PEAKDETECTOR, paramnames::MSG);
    jwm.get_paramlist()->add_param(
        synthmodnames::PEAKDETECTOR, paramnames::FORCE_ABORT);
    jwm.get_paramlist()->add_param(
        synthmodnames::PEAKDETECTOR, paramnames::MAXPEAKS);
    done_params = true;
}

#endif
