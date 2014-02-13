#include "../include/ladspa_module.h"

#include "../include/globals.h"

#include <climits>


ladspa_module::ladspa_module(const char* uname) :
 synthmod::base(synthmod::LADSPA, uname, SM_DEFAULT),
 libname(0), label(0), path(0), plugin_items(0), l_descriptor(0), l_handle(0),
 lp(0), port_count(0), port_mappings(0),
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
    register_comment("See --ladspa-help for definitions using specific LADSPA plugins.");
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
    if (plugin_items)
        delete plugin_items;

    if (libname)
        delete [] libname;
    if (label)
        delete [] label;
    if (path)
        delete [] path;
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
            return ((libname = new_strdup((const char*)data)) != 0);

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
        if (pm->type == ui::UI_PARAM) {
            if (pm->id == pt)
                return &pm->param;
        }
    }
    return 0;
}


bool ladspa_module::create_custom_ui_items()
{
    if (!libname || !label) {
        debug("LADSPA library and plugin label unspecified.\n");
        invalidate();
        return false;
    }

    #if DEBUG
    if (custom_created) {
        debug("\n****************************************************\n"
                "create_custom_ui_items called twice for same module!\n"
                "++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        return false;
    }
    custom_created = true;
    #endif

    if (!(path = wcnt::jwm.get_ladspaloader()->find_lib_path(libname))) {
        sm_err("Failed to find LADSPA library '%s'\n", libname);
        return false;
    }

    lp = wcnt::jwm.get_ladspaloader()->get_plugin(path, label);

    plugin_items = new ui::moditem_list;

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
                                                        path, label);
        invalidate();
        return false;
    }

    int portix = 0;
    port_count = l_descriptor->PortCount;

    if (!(port_mappings = new port_map[port_count])) {
        sm_err("Port mapping allocation for LADSPA plugin '%s' '%s' failed.",
                                                        path, label);
        invalidate();
        return false;
    }

    activate_custom_ui_items();

    char* pl;
    {
        size_t l1 = strlen(l_descriptor->Name);
        size_t l2 = strlen(l_descriptor->Maker);
        size_t l3 = (l1 > l2 ? l1 : l2);
        l1 = strlen(l_descriptor->Copyright);
        l2 = (l1 > l3 ? l1 : l3);
        pl = pretty_line("-+=+-", l2);
    }

    register_comment(pl);
    register_comment(l_descriptor->Name);
    register_comment(l_descriptor->Maker);
    register_comment(l_descriptor->Copyright);
    register_comment(pl)->set_flags(ui::UI_DELDESCR);

    for (portix = 0; portix < port_count; ++portix) {
        debug("processing port index %d name '%s'\n", portix,
                                                      l_descriptor->PortNames[portix]);
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


bool ladspa_module::activate_custom_ui_items()
{
    if (libname && label) {
        custom = true;
        return true;
    }
    custom = false;
    return false;
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

