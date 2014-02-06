#include "../include/peakdetector.h"

#include <iostream>

peak_detector::peak_detector(const char* uname) :
 synthmod::base(synthmod::PEAKDETECTOR, uname, SM_DEFAULT),
 in_signal(0), sig_range_hi(0.0), sig_range_lo(0.0), message(0),
 force_abort(OFF), max_peaks(0), peak_count(0), check(true)
{
}

void peak_detector::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::SIG_RANGE_HI);
    register_param(param::SIG_RANGE_LO);
    register_param(param::MSG);
    register_param(param::FORCE_ABORT);
    register_param(param::MAXPEAKS);
}

ui::moditem_list* peak_detector::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
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

const void* peak_detector::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* peak_detector::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
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
                synthmod::base::force_abort();
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
                synthmod::base::force_abort();
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

bool peak_detector::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::SIG_RANGE_HI:
            sig_range_hi = *(double*)data;
            return true;
        case param::SIG_RANGE_LO:
            sig_range_lo = *(double*)data;
            return true;
        case param::MSG:
            set_message((const char*)data);
            return true;
        case param::FORCE_ABORT:
            force_abort = *(STATUS*)data;
            return true;
        case param::MAXPEAKS:
            max_peaks = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* peak_detector::get_param(int pt) const
{
    switch(pt)
    {
        case param::SIG_RANGE_HI:  return &sig_range_hi;
        case param::SIG_RANGE_LO:  return &sig_range_lo;
        case param::MSG:           return message;
        case param::FORCE_ABORT:   return &force_abort;
        case param::MAXPEAKS:      return &max_peaks;
        default: return 0;
    }
}

errors::TYPE peak_detector::validate()
{
    if (!validate_param(param::MAXPEAKS, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

