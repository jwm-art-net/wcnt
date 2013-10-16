#include "../include/freqgenerator.h"
#include "../include/globals.h"

freq_generator::freq_generator(const char* uname) :
 synthmod::base(synthmod::FREQGEN, uname, SM_DEFAULT),
 in_signal(0), out_freq(220.00), out_phase_step(0),
 sig_range_hi(1.00), sig_range_lo(-1.00),
 freq_range_hi(440.00), freq_range_lo(110.00), sig_step_size(0),
 freq_step_size(0), step_count(24)
{
    register_output(output::OUT_FREQ);
    register_output(output::OUT_PHASE_STEP);
}

void freq_generator::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::STEP_COUNT);
    register_param(param::SIG_RANGE_LO);
    register_param(param::SIG_RANGE_HI);
    register_param(param::FREQ_RANGE_LO);
    register_param(param::FREQ_RANGE_HI);
}

freq_generator::~freq_generator()
{
}

const void* freq_generator::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_FREQ:     return &out_freq;
        case output::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

const void* freq_generator::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* freq_generator::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool freq_generator::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::SIG_RANGE_HI:
            sig_range_hi = *(double*)data;
            return true;
        case param::SIG_RANGE_LO:
            sig_range_lo = *(double*)data;
            return true;
        case param::FREQ_RANGE_HI:
            freq_range_hi = *(double*)data;
            return true;
        case param::FREQ_RANGE_LO:
            freq_range_lo = *(double*)data;
            return true;
        case param::STEP_COUNT:
            step_count = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* freq_generator::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::SIG_RANGE_HI:  return &sig_range_hi;
        case param::SIG_RANGE_LO:  return &sig_range_lo;
        case param::FREQ_RANGE_HI: return &freq_range_hi;
        case param::FREQ_RANGE_LO: return &freq_range_lo;
        case param::STEP_COUNT:    return &step_count;
        default: return 0;
    }
}

errors::TYPE freq_generator::validate()
{
    if (!validate_param(param::STEP_COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::FREQ_RANGE_HI, errors::RANGE_FREQ))
        return errors::RANGE_FREQ;

    if (!validate_param(param::FREQ_RANGE_LO, errors::RANGE_FREQ))
        return errors::RANGE_FREQ;

    return errors::NO_ERROR;
}

void freq_generator::init()
{
    sig_step_size = (sig_range_hi - sig_range_lo) / step_count;
    freq_step_size = (freq_range_hi - freq_range_lo) / step_count;
}

void freq_generator::run()
{
    out_freq =
        freq_range_lo + ((int)((*in_signal - sig_range_lo)
                               / sig_step_size)) * freq_step_size;
    out_phase_step = wcnt::jwm.samplerate() / out_freq;
}

