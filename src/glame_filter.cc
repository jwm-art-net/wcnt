#include "../include/glame_filter.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"

glame_filter::glame_filter(const char* uname) :
 synthmod::base(synthmod::GLAME_FILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_freq_mod1(0), output(0),
 type(LOPASS), cutoff_freq(440.0), freq_mod1size(1.0), stages(1),
 l_descriptor(0), l_inst_handle(0),
 l_cutoff_freq(440.0), l_stages(1),
 l_input(0), l_output(0)
{
    register_output(output::OUT_OUTPUT);
    type_names[0] = "lowpass_iir";
    type_names[1] = "highpass_iir";
    min_cutoff = 0.0001 * wcnt::jwm.samplerate();
    max_cutoff = 0.45 * wcnt::jwm.samplerate();
}

void glame_filter::register_ui()
{
    register_param(param::GLAME_FILTER_TYPE, "lowpass/highpass");
    register_input(input::IN_SIGNAL);
    register_param(param::FREQ);
    register_input(input::IN_FREQ_MOD1);
    register_param(param::FREQ_MOD1SIZE);
    register_param(param::STAGES);
}

ui::moditem_list* glame_filter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

glame_filter::~glame_filter()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_output) delete [] l_output;
}

const void* glame_filter::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void*
glame_filter::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:
            return in_signal = (double*)o;
        case input::IN_FREQ_MOD1:
            return in_freq_mod1 = (double*)o;
        default:
            return 0;
    }
}

const void* glame_filter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:    return in_signal;
        case input::IN_FREQ_MOD1: return in_freq_mod1;
        default: return 0;
    }
}

bool
glame_filter::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::GLAME_FILTER_TYPE:
            type = *(F_TYPE*)data;
            return true;
        case param::FREQ:
            cutoff_freq = *(double*)data;
            return true;
        case param::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case param::STAGES:
            stages = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* glame_filter::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GLAME_FILTER_TYPE: return &type;
        case param::FREQ:              return &cutoff_freq;
        case param::FREQ_MOD1SIZE:     return &freq_mod1size;
        case param::STAGES:            return &stages;
        default: return 0;
    }
}

errors::TYPE glame_filter::validate()
{
    if (cutoff_freq < min_cutoff || cutoff_freq > max_cutoff) {
        sm_err("%s must be within range 0.0001 * samplerate ~ "
               "0.45 * samplerate.",
                param::names::get(param::FREQ));
        invalidate();
        return errors::ERROR;
    }
    if (stages < 1 || stages > 10){
        sm_err("%s must be within range 1 ~ 10",
                    param::names::get(param::STAGES));
        invalidate();
        return errors::ERROR;
    }
    if (!validate_param(param::FREQ_MOD1SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    return errors::NO_ERROR;
}

void glame_filter::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    ladspa_plug* lp = 0;
    if(type == LOPASS)
        lp = ll->get_plugin("lowpass_iir_1891", "lowpass_iir");
    else
        lp = ll->get_plugin("highpass_iir_1890", "highpass_iir");
    if (lp == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
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

#endif // WITH_LADSPA
