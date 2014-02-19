#include "../include/glame_filter.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"


ladspa_plug* glame_filter::lp_hipass = 0;
ladspa_plug* glame_filter::lp_lopass = 0;


glame_filter::glame_filter(const char* uname) :
 synthmod::base(synthmod::GLAME_FILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_freq_mod1(0), output(0),
 type(LOPASS), cutoff_freq(440.0), freq_mod1size(1.0), stages(1),
 l_descriptor(0), l_handle(0),
 l_cutoff_freq(440.0), l_stages(1),
 l_input(0), l_output(0), lp(0)
{
    register_output(output::OUT_OUTPUT);
    type_names[0] = "lowpass_iir";
    type_names[1] = "highpass_iir";
}


void glame_filter::register_ui()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    char* path = ll->find_lib_path("lowpass_iir_1891");

    if (path) {
        lp_lopass = ll->get_plugin(path, "lowpass_iir");
        delete [] path;
    }

    if ((path = ll->find_lib_path("highpass_iir_1890"))) {
        lp_hipass = ll->get_plugin(path, "highpass_iir");
        delete [] path;
    }

    register_param(param::GLAME_FILTER_TYPE, "lowpass|highpass");
    register_input(input::IN_SIGNAL);
    register_param(param::FREQ);
    register_input(input::IN_FREQ_MOD1)->set_flags(ui::UI_GROUP1);
    register_param(param::FREQ_MOD1SIZE)->set_flags(ui::UI_GROUP1);
    register_param(param::STAGES);
}


ui::moditem_list* glame_filter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

glame_filter::~glame_filter()
{
    if (l_descriptor)
        l_descriptor->cleanup(l_handle);
}


const void* glame_filter::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}


const void*
glame_filter::set_in(int it, const void* o)
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


const void* glame_filter::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:    return in_signal;
        case input::IN_FREQ_MOD1: return in_freq_mod1;
        default: return 0;
    }
}


bool
glame_filter::set_param(int pt, const void* data)
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


const void* glame_filter::get_param(int pt) const
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
    char* err = 0;

    if (!(lp = (type == LOPASS ? lp_lopass : lp_hipass))) {
        sm_err("%s", "Failed to load LADSPA plugin.");
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Cutoff Frequency", &l_cutoff_freq))) {
        sm_err("%s %s.", param::names::get(param::FREQ), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("Stages(2 poles per stage)", &l_stages))) {
        sm_err("%s %s.", param::names::get(param::STAGES), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if (!validate_param(param::FREQ_MOD1SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    return errors::NO_ERROR;
}


void glame_filter::init()
{
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }

    l_descriptor->connect_port(l_handle, 0, &l_cutoff_freq);
    l_descriptor->connect_port(l_handle, 1, &l_stages);
    l_descriptor->connect_port(l_handle, 2, &l_input);
    l_descriptor->connect_port(l_handle, 3, &l_output);
    l_descriptor->activate(l_handle);
    l_stages = stages;
}


void glame_filter::run()
{
    l_input = *in_signal;

    if (*in_freq_mod1 < 0)
        l_cutoff_freq = cutoff_freq /(1 + freq_mod1size * -*in_freq_mod1);
    else
        l_cutoff_freq = cutoff_freq * (1 + freq_mod1size * *in_freq_mod1);

    l_descriptor->run(l_handle, 1);
    output = l_output;
}

#endif // WITH_LADSPA
