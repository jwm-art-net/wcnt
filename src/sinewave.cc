#ifndef SINEWAVE_H
#include "../include/sinewave.h"

sine_wave::sine_wave(char const* uname) :
 synthmod(synthmodnames::MOD_SINEWAVE, sine_wave_count, uname),
 output(0.00), play_state(OFF), in_phase_trig(NULL), in_deg_size(NULL),
 degs(360.00), recycle(OFF), zero_deg(OFF), cycles(1.00), maxdegs(360)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE);
#endif
    sine_wave_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}
sine_wave::~sine_wave()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
void const* sine_wave::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &output;
        break;
    case outputnames::OUT_PLAY_STATE:
        o = &play_state;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* sine_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_PHASE_TRIG:
        i = in_phase_trig = (STATUS*)o;
        break;
    case inputnames::IN_DEG_SIZE:
        i = in_deg_size = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool sine_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_RECYCLE_MODE:
        set_recycle_mode(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_TRIG_ZERO_DEG:
        set_on_trig_zero_deg(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_CYCLES:
        set_cycles(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* sine_wave::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_RECYCLE_MODE:
        return &recycle;
    case paramnames::PAR_TRIG_ZERO_DEG:
        return &zero_deg;
    case paramnames::PAR_CYCLES:
        return &cycles;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE sine_wave::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_CYCLES,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_CYCLES);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

#endif // BARE_MODULES

void sine_wave::init()
{
    maxdegs = cycles * 360.00;
}

void sine_wave::run()
{
    if (*in_phase_trig == ON) {
        play_state = ON;
        if (zero_deg == ON) degs = 0.00;
    }
    if (play_state == ON)
        output = sin(degs * DTR);
    degs += *in_deg_size;
    if (degs >= maxdegs) degs -= maxdegs;
    if (play_state == ON)
        if (degs + *in_deg_size >= maxdegs)
            if (recycle == OFF) play_state = OFF;
}

int sine_wave::sine_wave_count = 0;

#ifndef BARE_MODULES
bool sine_wave::done_params = false;

void sine_wave::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SINEWAVE, paramnames::PAR_RECYCLE_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SINEWAVE, paramnames::PAR_TRIG_ZERO_DEG);
    get_paramlist()->add_param(
     synthmodnames::MOD_SINEWAVE, paramnames::PAR_CYCLES);
    done_params = true;
}
#endif
#endif
