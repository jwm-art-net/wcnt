#include "../include/caps_plate2x2.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"

caps_plate2x2::caps_plate2x2(const char* uname) :
 synthmod::base(synthmod::CAPS_PLATE2X2, uname, SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0), out_left(0), out_right(0),
 bandwidth(0.502), tail(0.3745), damping(0.250375), blend(0.25),
 l_descriptor(0), l_inst_handle(0),
 l_in_left(0), l_in_right(0), l_bandwidth(0), l_tail(0), l_damping(0),
 l_blend(0), l_out_left(0), l_out_right(0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void caps_plate2x2::register_ui()
{
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT);
    register_param(param::BANDWIDTH);
    register_param(param::TAIL);
    register_param(param::DAMPING);
    register_param(param::WETDRY);
}

ui::moditem_list* caps_plate2x2::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

caps_plate2x2::~caps_plate2x2()
{
    if (l_descriptor)
        l_descriptor->cleanup(l_inst_handle);
    if (l_in_left)
        delete [] l_in_left;
    if (l_in_right)
        delete [] l_in_right;
    if (l_out_left)
        delete [] l_out_left;
    if (l_out_right)
        delete [] l_out_right;
}

const void* caps_plate2x2::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT:  return &out_left;
        case output::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

const void*
caps_plate2x2::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_LEFT:
        return in_left = (double*)o;
    case input::IN_RIGHT:
        return in_right = (double*)o;
    default:
        return 0;
    }
}

const void* caps_plate2x2::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_LEFT:   return in_left;
    case input::IN_RIGHT:  return in_right;
    default: return 0;
    }
}

bool
caps_plate2x2::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::BANDWIDTH:
        bandwidth = *(double*)data;
        return true;
    case param::TAIL:
        tail = *(double*)data;
        return true;
    case param::DAMPING:
        damping = *(double*)data;
        return true;
    case param::WETDRY:
        blend = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* caps_plate2x2::get_param(param::TYPE pt) const
{
    switch(pt)
    {
    case param::BANDWIDTH: return &bandwidth;
    case param::TAIL:      return &tail;
    case param::DAMPING:   return &damping;
    case param::WETDRY:    return &blend;
    default: return 0;
    }
}

errors::TYPE caps_plate2x2::validate()
{
    if (bandwidth < 0.005 || bandwidth > 0.999) {
        sm_err("%s must be within range 0.005 ~ 0.999,",
                param::names::get(param::BANDWIDTH));
        invalidate();
        return errors::ERROR;
    }
    if (tail < 0.0 || tail > 0.749) {
        sm_err("%s must be within range 0.0 ~ 0.749.",
                param::names::get(param::TAIL));
        invalidate();
        return errors::ERROR;
    }
    if (damping < 0.0005 || damping > 1.0) {
        sm_err("%s must be within range 0.0005 ~ 1.0.",
                param::names::get(param::DAMPING));
        invalidate();
        return errors::ERROR;
    }
    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;
    return errors::NO_ERROR;
}

void caps_plate2x2::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
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

#endif // WITH_LADSPA
