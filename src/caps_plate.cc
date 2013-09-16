#include "../include/caps_plate.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

caps_plate::caps_plate(char const* uname) :
 synthmod(synthmodnames::CAPS_PLATE, uname, SM_HAS_STEREO_OUTPUT),
 in_signal(0), out_left(0), out_right(0),
 bandwidth(0.502), tail(0.3745), damping(0.250375), blend(0.25),
 l_descriptor(0), l_inst_handle(0),
 l_input(0), l_bandwidth(0), l_tail(0), l_damping(0), l_blend(0),
 l_out_left(0), l_out_right(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

caps_plate::~caps_plate()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input) delete [] l_input;
    if (l_out_left) delete [] l_out_left;
    if (l_out_right) delete [] l_out_right;
}

void const* caps_plate::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return &out_left;
        case outputnames::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

void const*
caps_plate::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        default:
            return 0;
    }
}

void const* caps_plate::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool
caps_plate::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::BANDWIDTH:
            bandwidth = *(double*)data;
            return true;
        case paramnames::TAIL:
            tail = *(double*)data;
            return true;
        case paramnames::DAMPING:
            damping = *(double*)data;
            return true;
        case paramnames::WETDRY:
            blend = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* caps_plate::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::BANDWIDTH: return &bandwidth;
        case paramnames::TAIL:      return &tail;
        case paramnames::DAMPING:   return &damping;
        case paramnames::WETDRY:    return &blend;
        default: return 0;
    }
}

stockerrs::ERR_TYPE caps_plate::validate()
{
    if (bandwidth < 0.005 || bandwidth > 0.999) {
        *err_msg +=
            jwm.get_paramnames()->get_name(paramnames::BANDWIDTH);
        *err_msg += " must be within range 0.005 to 0.999";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (tail < 0.0 || tail > 0.749) {
        *err_msg +=
            jwm.get_paramnames()->get_name(paramnames::TAIL);
        *err_msg += " must be within range 0.0 to 0.749";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (damping < 0.0005 || damping > 1.0) {
        *err_msg +=
            jwm.get_paramnames()->get_name(paramnames::DAMPING);
        *err_msg += " must be within range 0.0005 to 1.0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (blend < 0.0 || blend > 1.0) {
        *err_msg +=
            jwm.get_paramnames()->get_name(paramnames::WETDRY);
        *err_msg += " must be within range 0.0 to 1.0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void caps_plate::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("caps", "Plate");
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
    l_out_left = new LADSPA_Data[1];
    l_out_right = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, l_input);
    l_descriptor->connect_port(l_inst_handle, 1, &l_bandwidth);
    l_descriptor->connect_port(l_inst_handle, 2, &l_tail);
    l_descriptor->connect_port(l_inst_handle, 3, &l_damping);
    l_descriptor->connect_port(l_inst_handle, 4, &l_blend);
    l_descriptor->connect_port(l_inst_handle, 5, l_out_left);
    l_descriptor->connect_port(l_inst_handle, 6, l_out_right);
    l_descriptor->activate(l_inst_handle);
    l_bandwidth = bandwidth;
    l_tail = tail;
    l_damping = damping;
    l_blend = blend;
}

void caps_plate::run()
{
    *l_input = *in_signal;
    l_descriptor->run(l_inst_handle, 1);
    out_left = *l_out_left;
    out_right = *l_out_right;
}

bool caps_plate::done_params = false;

void caps_plate::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(synthmodnames::CAPS_PLATE,
                               paramnames::BANDWIDTH);
    jwm.get_paramlist()->add_param(synthmodnames::CAPS_PLATE,
                               paramnames::TAIL);
    jwm.get_paramlist()->add_param(synthmodnames::CAPS_PLATE,
                               paramnames::DAMPING);
    jwm.get_paramlist()->add_param(synthmodnames::CAPS_PLATE,
                               paramnames::WETDRY);
    done_params = true;
}

#endif // WITH_LADSPA
