#ifndef DELAY_H
#include "../include/rms.h"

rms::rms(char const* uname) :
 synthmod(synthmodnames::MOD_RMS, rms_count, uname),
 in_signal(0), out_rms(0), rms_time(0), output(0), rmsarr(0),
 arraymax(0), arrpos(0), sqrsum(0)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_RMS);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
#endif
    rms_count++;
    validate();
#ifndef BARE_MODULES
    create_params();
#endif
}

rms::~rms()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
    delete [] rmsarr;
}

void rms::init()
{
    arraymax = (short)((rms_time * audio_samplerate) / 1000);
    rmsarr = new double[arraymax];
    for (int i = 0; i < arraymax; i++) rmsarr[i] = 0;
    arrpos = arraymax - 1;
}

#ifndef BARE_MODULES
void const* rms::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_RMS:
        o = &out_rms;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* rms::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool rms::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_RMS_TIME:
        set_rms_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* rms::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_RMS_TIME:
        return &rms_time;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE rms::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_RMS_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_RMS_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

#endif // BARE_MODULES

void rms::run()
{
    sqrsum = 0;
    for (int i = arrpos; i < arrpos + arraymax; i++)
        sqrsum += rmsarr[i % arraymax];
    out_rms = sqrt(sqrsum / arraymax);
    rmsarr[arrpos] = *in_signal * *in_signal;
    arrpos--;
    if (arrpos < 0)
        arrpos = arraymax - 1;
}

int rms::rms_count = 0;

#ifndef BARE_MODULES
bool rms::done_params = false;

void rms::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_RMS, paramnames::PAR_RMS_TIME);
    done_params = true;
}
#endif
#endif
