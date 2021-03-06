#include "../include/caps_plate2x2.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"


static const char* plugin_name = 0;
static char plugin_port_sep = 0;


caps_plate2x2::caps_plate2x2(const char* uname) :
 synthmod::base(synthmod::CAPS_PLATE, uname, SM_HAS_STEREO_OUTPUT),
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
    register_input(input::IN_RIGHT)->set_connect_as(input::IN_LEFT)
                                   ->set_flags(ui::UI_OPTIONAL);
    register_param(param::BANDWIDTH);
    register_param(param::TAIL);
    register_param(param::DAMPING);
    register_param(param::WETDRY);

    ladspa_plug* lp = 0;

    if ((lp = wcnt::jwm.get_ladspaloader()->get_plugin("caps", "Plate2x2")))
        plugin_name = "Plate2x2";
    else if ((lp = wcnt::jwm.get_ladspaloader()->get_plugin("caps", "PlateX2")))
        plugin_name = "PlateX2";

    if (lp->get_port_index("in:l") != -1)
        plugin_port_sep = ':';
    else if (lp->get_port_index("in.l") != -1)
        plugin_port_sep = '.';

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
    if (!plugin_name) {
        sm_err("Failed to load %s.", "LADSPA plugin");
        invalidate();
        return errors::ERROR;
    }

    ladspa_plug* lp = wcnt::jwm.get_ladspaloader()->get_plugin("caps", plugin_name);

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
    if ((l_inst_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return errors::ERROR;
    }

    l_bandwidth = bandwidth;
    l_tail = tail;
    l_damping = damping;
    l_blend = blend;

    char* err;

    if ((err = lp->validate_port("bandwidth", &l_bandwidth))) {
        sm_err("%s %s.", param::names::get(param::BANDWIDTH), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("tail", &l_tail))) {
        sm_err("%s %s.", param::names::get(param::TAIL), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    if ((err = lp->validate_port("damping", &l_damping))) {
        sm_err("%s %s.", param::names::get(param::DAMPING), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }
    if ((err = lp->validate_port("blend", &l_blend))) {
        sm_err("%s %s.", param::names::get(param::WETDRY), err);
        delete [] err;
        invalidate();
        return errors::ERROR;
    }

    return errors::NO_ERROR;
}

void caps_plate2x2::init()
{
    ladspa_plug* lp = wcnt::jwm.get_ladspaloader()->get_plugin("caps", plugin_name);

    l_in_left = new LADSPA_Data[1];
    l_in_right = new LADSPA_Data[1];
    l_out_left = new LADSPA_Data[1];
    l_out_right = new LADSPA_Data[1];

    bool didnotconnect = false;

    switch(plugin_port_sep) {
      case '.':
        if (lp->connect_port(l_inst_handle, "in.l", l_in_left) < 0
         || lp->connect_port(l_inst_handle, "in.r", l_in_right) < 0
         || lp->connect_port(l_inst_handle, "out.l", l_out_left) < 0
         || lp->connect_port(l_inst_handle, "out.r", l_out_right) < 0)
            didnotconnect = true;
        break;
      case ':':
        if (lp->connect_port(l_inst_handle, "in:l", l_in_left) < 0
         || lp->connect_port(l_inst_handle, "in:r", l_in_right) < 0
         || lp->connect_port(l_inst_handle, "out:l", l_out_left) < 0
         || lp->connect_port(l_inst_handle, "out:r", l_out_right) < 0)
            didnotconnect = true;
        break;
      default:
        didnotconnect = true;
    }

    if (didnotconnect
     || lp->connect_port(l_inst_handle, "bandwidth", &l_bandwidth) < 0
     || lp->connect_port(l_inst_handle, "tail",      &l_tail) < 0
     || lp->connect_port(l_inst_handle, "damping",   &l_damping) < 0
     || lp->connect_port(l_inst_handle, "blend",     &l_blend) < 0)
    {
        debug("failed to connect port(s).");
        invalidate();
        return;
    }

    l_descriptor->activate(l_inst_handle);
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
