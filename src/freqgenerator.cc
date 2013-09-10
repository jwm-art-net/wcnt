#ifndef FREQGENERATOR_H
#include "../include/freqgenerator.h"

freq_generator::freq_generator(char const* uname) :
 synthmod(synthmodnames::MOD_FREQGEN, freq_generator_count, uname),
 in_signal(0), out_freq(220.00), sig_range_hi(1.00), sig_range_lo(-1.00),
 freq_range_hi(440.00), freq_range_lo(110.00), step_count(24)
{
    get_outputlist()->add_output(this, outputnames::OUT_FREQ);
    get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    freq_generator_count++;
    init();
    create_params();
}

freq_generator::~freq_generator()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* freq_generator::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot) {
    case outputnames::OUT_FREQ:
        o = &out_freq;
        break;
    case outputnames::OUT_DEG_SIZE:
        o = &out_deg_size;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* freq_generator::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it) {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool freq_generator::set_param(paramnames::PAR_TYPE pt, void const* data)
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
    case paramnames::PAR_FREQ_RANGE_HI:
        set_freq_range_hi(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_FREQ_RANGE_LO:
        set_freq_range_lo(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_STEP_COUNT:
        set_step_count(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* freq_generator::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_SIG_RANGE_HI:
        return &sig_range_hi;
    case paramnames::PAR_SIG_RANGE_LO:
        return &sig_range_lo;
    case paramnames::PAR_FREQ_RANGE_HI:
        return &freq_range_hi;
    case paramnames::PAR_FREQ_RANGE_LO:
        return &freq_range_lo;
    case paramnames::PAR_STEP_COUNT:
        return &step_count;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE freq_generator::validate()
{
    if (step_count <= 1) {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_STEP_COUNT);
        *err_msg += "must be of a larger value than 1";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_FREQ_RANGE_HI,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_FREQ_RANGE_HI);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!pl->validate(this, paramnames::PAR_FREQ_RANGE_LO,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_FREQ_RANGE_LO);
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
    out_deg_size = (double)audio_samplerate / out_freq;
}

int freq_generator::freq_generator_count = 0;

bool freq_generator::done_params = false;

void freq_generator::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_FREQGEN, paramnames::PAR_STEP_COUNT);
    get_paramlist()->add_param(
     synthmodnames::MOD_FREQGEN, paramnames::PAR_SIG_RANGE_LO);
    get_paramlist()->add_param(
     synthmodnames::MOD_FREQGEN, paramnames::PAR_SIG_RANGE_HI);
    get_paramlist()->add_param(
     synthmodnames::MOD_FREQGEN, paramnames::PAR_FREQ_RANGE_LO);
    get_paramlist()->add_param(
     synthmodnames::MOD_FREQGEN, paramnames::PAR_FREQ_RANGE_HI);
    done_params = true;
}

#endif
