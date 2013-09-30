#include "../include/bode_freq_shifter.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

bode_freq_shifter::bode_freq_shifter(const char* uname) :
 synthmod(module::BODE_FREQ_SHIFTER, uname, SM_DEFAULT),
 in_signal(0), in_shift_mod(0), out_up(0), out_down(0),
 freq_shift(0), shift_modsize(0.0),
 l_descriptor(0), l_inst_handle(0),
 l_freq_shift(0.0), l_input(0), l_out_down(0), l_out_up(0),
 l_out_latency(0)
{
    register_input(input::IN_SIGNAL);
    register_input(input::IN_SHIFT_MOD);
    register_output(output::OUT_UP);
    register_output(output::OUT_DOWN);
    init_first();
}

bode_freq_shifter::~bode_freq_shifter()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_out_down) delete [] l_out_down;
    if (l_out_up) delete [] l_out_up;
    if (l_out_latency) delete [] l_out_latency;
}

const void* bode_freq_shifter::get_out(output::TYPE ot) const
{
    switch(ot) {
        case output::OUT_UP: return &out_up;
        case output::OUT_DOWN: return &out_down;
        default: return 0;
    }
}

const void*
bode_freq_shifter::set_in(input::TYPE it, const void* o)
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

const void* bode_freq_shifter::get_in(input::TYPE it) const
{
    switch(it) {
    case input::IN_SIGNAL:    return in_signal;
    case input::IN_SHIFT_MOD: return in_shift_mod;
    default: return 0;
    }
}

bool bode_freq_shifter::set_param(param::TYPE pt, const void* data)
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

const void* bode_freq_shifter::get_param(param::TYPE pt) const
{
    switch(pt) {
    case param::FREQ_SHIFT:    return &freq_shift;
    case param::SHIFT_MODSIZE: return &shift_modsize;
    default: return 0;
    }
}

errors::TYPE bode_freq_shifter::validate()
{
    if (freq_shift < 0 || freq_shift > 5000) {
        sm_err("%s must be within range 0 ~ 5000.",
                param::names::get(param::FREQ_SHIFT));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

void bode_freq_shifter::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("bode_shifter_1431", "bodeShifter");
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
    l_out_down = new LADSPA_Data[1];
    l_out_up = new LADSPA_Data[1];
    l_out_latency = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, &l_freq_shift);
    l_descriptor->connect_port(l_inst_handle, 1, l_input);
    l_descriptor->connect_port(l_inst_handle, 2, l_out_down);
    l_descriptor->connect_port(l_inst_handle, 3, l_out_up);
    l_descriptor->connect_port(l_inst_handle, 4, l_out_latency);
}

void bode_freq_shifter::run()
{
    *l_input = *in_signal;
    l_freq_shift = freq_shift + *in_shift_mod * shift_modsize;
    if (l_freq_shift < 0)
        l_freq_shift = 0;
    else if
       (l_freq_shift > 5000)
        l_freq_shift = 5000;
    l_descriptor->run(l_inst_handle, 1);
    out_up = *l_out_up;
    out_down = *l_out_down;
}

void bode_freq_shifter::init_first()
{
    if (done_first())
        return;
    register_param(param::FREQ_SHIFT);
    register_param(param::SHIFT_MODSIZE);
}

#endif // WITH_LADSPA
