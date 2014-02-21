#include "../include/bode_freq_shifter.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"


ladspa_plug* bode_freq_shifter::lp = 0;


bode_freq_shifter::bode_freq_shifter(const char* uname) :
 synthmod::base(synthmod::BODE_FREQ_SHIFTER, uname, SM_DEFAULT),
 in_signal(0), in_shift_mod(0), out_up(0), out_down(0),
 freq_shift(0), shift_modsize(0.0),
 l_descriptor(0), l_handle(0),
 l_freq_shift(0.0), l_input(0), l_out_down(0), l_out_up(0),
 l_out_latency(0)
{
    register_output(output::OUT_UP);
    register_output(output::OUT_DOWN);
}


void bode_freq_shifter::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::FREQ_SHIFT);
    register_input(input::IN_SHIFT_MOD);
    register_param(param::SHIFT_MODSIZE);

    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    char* path = ll->find_lib_path("bode_shifter_1431");

    if (path) {
        lp = ll->get_plugin(path, "bodeShifter");
        delete [] path;
    }
}


ui::moditem_list* bode_freq_shifter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}


bode_freq_shifter::~bode_freq_shifter()
{
    if (l_descriptor)
        l_descriptor->cleanup(l_handle);
}


const void* bode_freq_shifter::get_out(int ot) const
{
    switch(ot) {
        case output::OUT_UP: return &out_up;
        case output::OUT_DOWN: return &out_down;
        default: return 0;
    }
}


const void* bode_freq_shifter::set_in(int it, const void* o)
{
    switch(it) {
    case input::IN_SIGNAL:
        return in_signal = (double*)o;
    case input::IN_SHIFT_MOD:
        return in_shift_mod = (double*)o;
    default:
        return 0;
    }
}


const void* bode_freq_shifter::get_in(int it) const
{
    switch(it) {
    case input::IN_SIGNAL:    return in_signal;
    case input::IN_SHIFT_MOD: return in_shift_mod;
    default: return 0;
    }
}


bool bode_freq_shifter::set_param(int pt, const void* data)
{
    switch(pt) {
    case param::FREQ_SHIFT:
        freq_shift = *(double*)data;
        return true;
    case param::SHIFT_MODSIZE:
        shift_modsize = *(double*)data;
        return true;
    default:
        return false;
    }
}


const void* bode_freq_shifter::get_param(int pt) const
{
    switch(pt) {
    case param::FREQ_SHIFT:    return &freq_shift;
    case param::SHIFT_MODSIZE: return &shift_modsize;
    default: return 0;
    }
}


errors::TYPE bode_freq_shifter::validate()
{

    if (!lp) {
        sm_err("Failed to load %s.", "LADSPA plugin");
        invalidate();
        return errors::ERROR;
    }

    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return errors::ERROR;
    }
    if ((l_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return errors::ERROR;
    }

    char* err;

    LADSPA_Data fshift = freq_shift;

    if ((err = lp->validate_port("Frequency shift", &fshift))) {
        sm_err("%s %s", param::names::get(param::FREQ_SHIFT), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}


void bode_freq_shifter::init()
{
    if (!lp) {
        sm_err("%s", "Failed to load plugin.");
        invalidate();
        return;
    }

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

    if (lp->connect_port(l_handle, "Frequency shift", &l_freq_shift) < 0
     || lp->connect_port(l_handle, "Input",           &l_input)      < 0
     || lp->connect_port(l_handle, "Down out",        &l_out_down)   < 0
     || lp->connect_port(l_handle, "Up out",          &l_out_up)     < 0
     || lp->connect_port(l_handle, "latency",         &l_out_latency)< 0)
    {
        debug("failed to connect port(s).");
        invalidate();
        return;
    }

    // doesn't have an activate function!
}

void bode_freq_shifter::run()
{
    l_input = *in_signal;
    l_freq_shift = freq_shift + *in_shift_mod * shift_modsize;

    /*
    if (l_freq_shift < shift_min)
        l_freq_shift = shift_min;
    else if
       (l_freq_shift > shift_max)
        l_freq_shift = shift_max;
    */

    l_descriptor->run(l_handle, 1);
    out_up = l_out_up;
    out_down = l_out_down;
}

#endif // WITH_LADSPA
