#include "../include/ladspa_module.h"

#include "../include/globals.h"

#include <climits>


ladspa_module::ladspa_module(const char* uname) :
 synthmod::base(synthmod::LADSPA, uname, SM_DEFAULT),
 filename(0), label(0), plugin_items(0), l_descriptor(0), l_handle(0), lp(0),
 port_count(0), port_mappings(0),
 custom(false)
 #if DEBUG
 , custom_created(false)
 #endif
{
}


void ladspa_module::register_ui()
{
    register_param(param::LADSPA_LIB)->set_flags(ui::UI_NO_EDIT);
    register_param(param::LADSPA_LABEL)->set_flags(ui::UI_NO_EDIT);
    register_custom_ui()->set_flags(ui::UI_NO_EDIT);
}


ui::moditem_list* ladspa_module::get_ui_items()
{
    static ui::moditem_list items;

    if (!custom) {
        debug("NON CUSTOM NON CUSTOM NON CUSTOM NON CUSTOM NON CUSTOM NON CUSTOM\n");
        return &items;
    }
    debug("%%^&%%^&*%%^&*%%^&%%^&*%%^&%%^&%%^&*%%^&%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&\n");
    return plugin_items;
}


ladspa_module::~ladspa_module()
{
    if (l_descriptor && l_handle) {
        if (l_descriptor->deactivate)
            l_descriptor->deactivate(l_handle);
        l_descriptor->cleanup(l_handle);
    }
    if (port_mappings) {
        for (int portix = 0; portix < port_count; ++portix) {
            if (port_mappings[portix].buf)
                delete [] port_mappings[portix].buf;
        }
        delete [] port_mappings;
    }
    if (filename)
        delete [] filename;
    if (label)
        delete [] label;
}


const void* ladspa_module::get_out(int ot) const
{
    return 0;
}

const void* ladspa_module::set_in(int it, const void* o)
{
    int portix;
    for (portix = 0; portix < port_count; ++portix) {
        port_map* pm = &port_mappings[portix];
        debug("checking port index %d id %d for id %d\n", portix, pm->id, it);
        if (pm->type == ui::UI_INPUT) {
            if (pm->id == it) {
                pm->input = (double*)o;
                return pm->input;
            }
        }
    }
    return 0;
}

const void* ladspa_module::get_in(int it) const
{
    return 0;
}


bool ladspa_module::set_param(int pt, const void* data)
{
    if (!custom) {
        if (pt == param::LADSPA_LIB)
            return ((filename = new_strdup((const char*)data)) != 0);

        if (pt == param::LADSPA_LABEL)
            return ((label = new_strdup((const char*)data)) != 0);

        return false;
    }

    debug("\n£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£"
          "\n£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£(£*£"
          "\n-------- setting param ...\n");

    int portix;
    for (portix = 0; portix < port_count; ++portix) {
        port_map* pm = &port_mappings[portix];
        debug("checking port index %d id %d for id %d\n", portix, pm->id, pt);
        if (pm->type == ui::UI_PARAM) {
            if (pm->id == pt) {
                pm->param = *((double*)data);
                return true;
            }
        }
    }
    return false;
}


const void* ladspa_module::get_param(int pt) const
{
    int portix;
    for (portix = 0; portix < port_count; ++portix) {
        port_map* pm = &port_mappings[portix];
        debug("checking port index %d id %d for id %d\n", portix, pm->id, pt);
        if (pm->id == pt) {
            if (pm->type != ui::UI_PARAM) {
                sm_err("%s in LADSPA module '%s' ('%s' '%s') confusion has "
                       "arisen between ports and parameters...",
                       errors::stock::major, get_username(), filename, label);
                return 0;
            }
            return &pm->param;
        }
    }
    return 0;
}


bool ladspa_module::create_custom_ui_items()
{
    #if DEBUG
    if (custom_created) {
        debug("\n****************************************************\n"
                "create_custom_ui_items called twice for same module!\n"
                "++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        return false;
    }
    custom_created = true;
    #endif
    plugin_items = new ui::moditem_list;
    lp = wcnt::jwm.get_ladspaloader()->get_plugin(filename, label);

    if (!lp) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return false;
    }

    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return false;
    }

    if (l_descriptor->PortCount > INT_MAX) {
        sm_err("LADSPA plugin '%s' '%s'has too many ports, giving up.",
                                                        filename, label);
        invalidate();
        return false;
    }

    int portix = 0;
    port_count = l_descriptor->PortCount;

    if (!(port_mappings = new port_map[port_count])) {
        sm_err("Port mapping allocation for LADSPA plugin '%s' '%s' failed.", filename, label);
        invalidate();
        return false;
    }

    activate_custom_ui_items();

    for (portix = 0; portix < port_count; ++portix) {
        debug("processing port index %d name '%s'\n", portix, l_descriptor->PortNames[portix]);
        port_map* pm = &port_mappings[portix];
        pm->id = 0;
        pm->type = 0;
        pm->control = 0;
        pm->buf = 0;
        pm->param = 0;
        pm->input = 0;
        if (LADSPA_IS_PORT_INPUT(l_descriptor->PortDescriptors[portix])) {
            if (LADSPA_IS_PORT_CONTROL(l_descriptor->PortDescriptors[portix])) {
                int id = param::names::register_type(l_descriptor->PortNames[portix], iocat::DOUBLE, 0);
                register_param(id);
                pm->id = id;
                pm->type = ui::UI_PARAM;
            }
            else {
                int id = input::names::register_type(l_descriptor->PortNames[portix], iocat::DOUBLE, 0);
                register_input(id);
                pm->id = id;
                pm->type = ui::UI_INPUT;
                pm->buf = new LADSPA_Data[1];
            }
        }
        else if (LADSPA_IS_PORT_OUTPUT(l_descriptor->PortDescriptors[portix])) {
            int id = output::names::register_type(l_descriptor->PortNames[portix], iocat::DOUBLE, 0);
            register_output(id);
            pm->id = id;
            pm->type = ui::UI_OUTPUT;
            pm->buf = new LADSPA_Data[1];
        }
    }

    deactivate_custom_ui_items();

    return true;
}


void ladspa_module::init()
{
    #if DEBUG
    if (!lp) {
        debug("no ladspa plugin loaded.");
        invalidate();
        return;
    }
    #endif

    if ((l_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
/*
    l_input = new LADSPA_Data[1];
    l_output = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, &l_cutoff_freq);
    l_descriptor->connect_port(l_inst_handle, 1, &l_stages);
    l_descriptor->connect_port(l_inst_handle, 2, l_input);
    l_descriptor->connect_port(l_inst_handle, 3, l_output);
    l_descriptor->activate(l_inst_handle);
    l_stages = stages;
*/
}


errors::TYPE ladspa_module::validate()
{
    return errors::NO_ERROR;
}

void ladspa_module::run()
{
}

