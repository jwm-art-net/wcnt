#include "../include/freqgenerator.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

freq_generator::freq_generator(const char* uname) :
 synthmod(synthmodnames::FREQGEN, uname, SM_DEFAULT),
 in_signal(0), out_freq(220.00), out_phase_step(0),
 sig_range_hi(1.00), sig_range_lo(-1.00),
 freq_range_hi(440.00), freq_range_lo(110.00), sig_step_size(0),
 freq_step_size(0), step_count(24)
{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_FREQ);
    register_output(outputnames::OUT_PHASE_STEP);
    init_first();
}

freq_generator::~freq_generator()
{
}

const void* freq_generator::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_FREQ:     return &out_freq;
        case outputnames::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

const void* freq_generator::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* freq_generator::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool freq_generator::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI:
            sig_range_hi = *(double*)data;
            return true;
        case paramnames::SIG_RANGE_LO:
            sig_range_lo = *(double*)data;
            return true;
        case paramnames::FREQ_RANGE_HI:
            freq_range_hi = *(double*)data;
            return true;
        case paramnames::FREQ_RANGE_LO:
            freq_range_lo = *(double*)data;
            return true;
        case paramnames::STEP_COUNT:
            step_count = *(short*)data;
            return true;
        default:
            return false;
    }
}

const void* freq_generator::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI:  return &sig_range_hi;
        case paramnames::SIG_RANGE_LO:  return &sig_range_lo;
        case paramnames::FREQ_RANGE_HI: return &freq_range_hi;
        case paramnames::FREQ_RANGE_LO: return &freq_range_lo;
        case paramnames::STEP_COUNT:    return &step_count;
        default: return 0;
    }
}

stockerrs::ERR_TYPE freq_generator::validate()
{
    if (step_count <= 1) {
        sm_err("%s must be of a higher count than 1",
                paramnames::get_name(paramnames::STEP_COUNT));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_RANGE_HI,
            stockerrs::ERR_RANGE_FREQ))
    {
        sm_err("%s", paramnames::get_name(
                                            paramnames::FREQ_RANGE_HI));
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_RANGE_LO,
            stockerrs::ERR_RANGE_FREQ))
    {
        sm_err("%s", paramnames::get_name(
                                            paramnames::FREQ_RANGE_LO));
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    return stockerrs::ERR_NO_ERROR;
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
    out_phase_step = jwm.samplerate() / out_freq;
}

void freq_generator::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::STEP_COUNT);
    register_param(paramnames::SIG_RANGE_LO);
    register_param(paramnames::SIG_RANGE_HI);
    register_param(paramnames::FREQ_RANGE_LO);
    register_param(paramnames::FREQ_RANGE_HI);
}

