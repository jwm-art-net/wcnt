#include "../include/glame_filter.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

glame_filter::glame_filter(char const* uname) :
 synthmod(synthmodnames::GLAME_FILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_freq_mod1(0), output(0),
 type(LOPASS), cutoff_freq(440.0), freq_mod1size(1.0), stages(1),
 l_descriptor(0), l_inst_handle(0),
 l_cutoff_freq(440.0), l_stages(1),
 l_input(0), l_output(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_input(inputnames::IN_FREQ_MOD1);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
    type_names[0] = "lowpass_iir";
    type_names[1] = "highpass_iir";
    min_cutoff = 0.0001 * jwm.samplerate();
    max_cutoff = 0.45 * jwm.samplerate();
}

glame_filter::~glame_filter()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
}

void const* glame_filter::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const*
glame_filter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        case inputnames::IN_FREQ_MOD1:
            return in_freq_mod1 = (double*)o;
        default:
            return 0;
    }
}

void const* glame_filter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:    return in_signal;
        case inputnames::IN_FREQ_MOD1: return in_freq_mod1;
        default: return 0;
    }
}

bool
glame_filter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::GLAME_FILTER_TYPE:
            type = *(F_TYPE*)data;
            return true;
        case paramnames::FREQ:
            cutoff_freq = *(double*)data;
            return true;
        case paramnames::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case paramnames::STAGES:
            stages = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* glame_filter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::GLAME_FILTER_TYPE: return &type;
        case paramnames::FREQ:              return &cutoff_freq;
        case paramnames::FREQ_MOD1SIZE:     return &freq_mod1size;
        case paramnames::STAGES:            return &stages;
        default: return 0;
    }
}

stockerrs::ERR_TYPE glame_filter::validate()
{
    if (cutoff_freq < min_cutoff || cutoff_freq > max_cutoff) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::FREQ);
        *err_msg += " must be within range 0.0001 * samplerate "
                    " to 0.45 * samplerate";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (stages < 1 || stages > 10){
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::FREQ);
        *err_msg += " must be within range 1 to 10";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_MOD1SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::FREQ_MOD1SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

void glame_filter::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = 0;
    if(type == LOPASS)
        lp = ll->get_plugin("lowpass_iir_1891", "lowpass_iir");
    else
        lp = ll->get_plugin("highpass_iir_1890", "highpass_iir");
    if (lp == 0) {
        *err_msg = ll->get_error_msg();
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        *err_msg = lp->get_error_msg();
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        *err_msg = lp->get_error_msg();
        invalidate();
        return;
    }
    l_input = new LADSPA_Data[1];
    l_output = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, &l_cutoff_freq);
    l_descriptor->connect_port(l_inst_handle, 1, &l_stages);
    l_descriptor->connect_port(l_inst_handle, 2, l_input);
    l_descriptor->connect_port(l_inst_handle, 3, l_output);
    l_descriptor->activate(l_inst_handle);
    l_stages = stages;
}

void glame_filter::run()
{
    *l_input = *in_signal;

    if (*in_freq_mod1 < 0)
        l_cutoff_freq = cutoff_freq /(1 + freq_mod1size * -*in_freq_mod1);
    else
        l_cutoff_freq = cutoff_freq * (1 + freq_mod1size * *in_freq_mod1);
    if (l_cutoff_freq < min_cutoff)
        l_cutoff_freq = min_cutoff;
    else if
       (l_cutoff_freq > max_cutoff)
        l_cutoff_freq = max_cutoff;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}

void glame_filter::init_first()
{
    if (done_first())
        return;

    register_param(paramnames::GLAME_FILTER_TYPE, "lowpass/highpass");
    register_param(paramnames::FREQ);
    register_param(paramnames::FREQ_MOD1SIZE);
    register_param(paramnames::STAGES);
}

#endif // WITH_LADSPA
