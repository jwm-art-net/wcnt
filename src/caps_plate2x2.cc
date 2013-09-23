#include "../include/caps_plate2x2.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

caps_plate2x2::caps_plate2x2(const char* uname) :
 synthmod(synthmodnames::CAPS_PLATE2X2, uname, SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0), out_left(0), out_right(0),
 bandwidth(0.502), tail(0.3745), damping(0.250375), blend(0.25),
 l_descriptor(0), l_inst_handle(0),
 l_in_left(0), l_in_right(0), l_bandwidth(0), l_tail(0), l_damping(0),
 l_blend(0), l_out_left(0), l_out_right(0)
{
    register_input(inputnames::IN_LEFT);
    register_input(inputnames::IN_RIGHT);
    register_output(outputnames::OUT_LEFT);
    register_output(outputnames::OUT_RIGHT);
    init_first();
}

caps_plate2x2::~caps_plate2x2()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_in_left) delete [] l_in_left;
    if (l_in_right) delete [] l_in_right;
    if (l_out_left) delete [] l_out_left;
    if (l_out_right) delete [] l_out_right;
}

const void* caps_plate2x2::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return &out_left;
        case outputnames::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

const void*
caps_plate2x2::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:
            return in_left = (double*)o;
        case inputnames::IN_RIGHT:
            return in_right = (double*)o;
        default:
            return 0;
    }
}

const void* caps_plate2x2::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return in_left;
        case inputnames::IN_RIGHT:  return in_right;
        default: return 0;
    }
}

bool
caps_plate2x2::set_param(paramnames::PAR_TYPE pt, const void* data)
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

const void* caps_plate2x2::get_param(paramnames::PAR_TYPE pt) const
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

stockerrs::ERR_TYPE caps_plate2x2::validate()
{
    if (bandwidth < 0.005 || bandwidth > 0.999) {
        sm_err("%s must be within range 0.005 ~ 0.999,",
                paramnames::get_name(paramnames::BANDWIDTH));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (tail < 0.0 || tail > 0.749) {
        sm_err("%s must be within range 0.0 ~ 0.749.",
                paramnames::get_name(paramnames::TAIL));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (damping < 0.0005 || damping > 1.0) {
        sm_err("%s must be within range 0.0005 ~ 1.0.",
                paramnames::get_name(paramnames::DAMPING));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (blend < 0.0 || blend > 1.0) {
        sm_err("%s must be within range 0.0 ~ 1.0.",
                paramnames::get_name(paramnames::WETDRY));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void caps_plate2x2::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("caps", "Plate2x2");
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
    l_in_left = new LADSPA_Data[1];
    l_in_right = new LADSPA_Data[1];
    l_out_left = new LADSPA_Data[1];
    l_out_right = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, l_in_left);
    l_descriptor->connect_port(l_inst_handle, 1, l_in_right);
    l_descriptor->connect_port(l_inst_handle, 2, &l_bandwidth);
    l_descriptor->connect_port(l_inst_handle, 3, &l_tail);
    l_descriptor->connect_port(l_inst_handle, 4, &l_damping);
    l_descriptor->connect_port(l_inst_handle, 5, &l_blend);
    l_descriptor->connect_port(l_inst_handle, 6, l_out_left);
    l_descriptor->connect_port(l_inst_handle, 7, l_out_right);
    l_descriptor->activate(l_inst_handle);
    l_bandwidth = bandwidth;
    l_tail = tail;
    l_damping = damping;
    l_blend = blend;
}

void caps_plate2x2::run()
{
    *l_in_left = *in_left;
    *l_in_right = *in_right;
    l_descriptor->run(l_inst_handle, 1);
    out_left = *l_out_left;
    out_right = *l_out_right;
}

void caps_plate2x2::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::BANDWIDTH);
    register_param(paramnames::TAIL);
    register_param(paramnames::DAMPING);
    register_param(paramnames::WETDRY);
}

#endif // WITH_LADSPA
