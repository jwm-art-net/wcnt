#include "../include/ladspa_module.h"

#include "../include/globals.h"

#include <climits>


ladspa_module::ladspa_module(const char* uname) :
 synthmod::base(synthmod::LADSPA, uname, SM_DEFAULT),
 libname(0), label(0), path(0),
 plugin_items(0),
 l_descriptor(0), l_handle(0), lp(0),
 port_count(0),

 in_int_count(0),       in_int(0),      in_int_px(0),
 in_float_count(0),     in_float(0),    in_float_px(0),
 out_int_count(0),      out_int(0),     out_int_px(0),
 out_float_count(0),    out_float(0),   out_float_px(0),
 param_int_count(0),    param_int(0),   param_int_px(0),
 param_float_count(0),  param_float(0), param_float_px(0),

 ports(0), port_mappings(0),
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

    if (ports)
        delete [] ports;

    if (port_mappings)
        delete [] port_mappings;

    if (in_int_count) {
        delete [] in_int;
        delete [] in_int_px;
    }
    if (in_float_count) {
        delete [] in_float;
        delete [] in_float_px;
    }
    if (out_int_count) {
        delete [] out_int;
        delete [] out_int_px;
    }
    if (out_float_count) {
        delete [] out_float;
        delete [] out_float_px;
    }
    if (param_int_count) {
        delete [] param_int;
        delete [] param_int_px;
    }
    if (param_float_count) {
        delete [] param_float;
        delete [] param_float_px;
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
    int px;
    for (px = 0; px < port_count; ++px) {
        port_map* pm = &port_mappings[px];
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (pm->type == ui::UI_OUTPUT && pm->id == ot) {
            return (LADSPA_IS_HINT_INTEGER(pd) ? (void*)&out_int[pm->index]
                                               : (void*)&out_float[pm->index]);
        }
    }
    return 0;
}

const void* ladspa_module::set_in(int it, const void* o)
{
    int px;
    for (px = 0; px < port_count; ++px) {
        port_map* pm = &port_mappings[px];
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (pm->type == ui::UI_INPUT && pm->id == it) {
            if (LADSPA_IS_HINT_INTEGER(pd))
                return (in_int[pm->index] = (wcint_t*)o);
            else
                return (in_float[pm->index] = (double*)o);
        }
    }
    return 0;
}

const void* ladspa_module::get_in(int it) const
{
    int px;
    for (px = 0; px < port_count; ++px) {
        port_map* pm = &port_mappings[px];
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (pm->type == ui::UI_INPUT && pm->id == it) {
            return (LADSPA_IS_HINT_INTEGER(pd) ? (void*)in_int[pm->index]
                                               : (void*)in_float[pm->index]);
        }
    }
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

    int px;
    for (px = 0; px < port_count; ++px) {
        port_map* pm = &port_mappings[px];
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (pm->type == ui::UI_PARAM && pm->id == pt) {
            if (LADSPA_IS_HINT_INTEGER(pd))
                param_int[pm->index] = *((wcint_t*)data);
            else
                param_float[pm->index] = *((double*)data);
            return true;
        }
    }

    return false;
}


const void* ladspa_module::get_param(int pt) const
{
    int px;
    for (px = 0; px < port_count; ++px) {
        port_map* pm = &port_mappings[px];
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (pm->type == ui::UI_PARAM && pm->id == pt) {
            return (LADSPA_IS_HINT_INTEGER(pd) ? (void*)&param_int[pm->index]
                                               : (void*)&param_float[pm->index]);
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

    // gather counts of ports by type...
    int px = 0;
    port_count = l_descriptor->PortCount;

    debug("gathering data about LADSPA port types...");
    for (px = 0; px < port_count; ++px) {
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];
        if (LADSPA_IS_PORT_INPUT(pd)) {
            if (LADSPA_IS_PORT_CONTROL(pd)) {
                if (LADSPA_IS_HINT_INTEGER(pd))
                    ++param_int_count;
                else
                    ++param_float_count;
            }
            else if (LADSPA_IS_PORT_AUDIO(pd)) {
                if (LADSPA_IS_HINT_INTEGER(pd))
                    ++in_int_count;
                else
                    ++in_float_count;
            }
            #if DEBUG
            else
                debug("Port '%s' is neither control nor audio!\n",
                                l_descriptor->PortNames[px]);
            #endif
        }
        else if (LADSPA_IS_PORT_OUTPUT(pd)) {
            if (LADSPA_IS_HINT_INTEGER(pd))
                ++out_int_count;
            else
                ++out_float_count;
        }
        #if DEBUG
        else
            debug("Port '%s' is neither input nor output!\n",
                            l_descriptor->PortNames[px]);
        #endif
    }

    if (in_int_count) {
        in_int = new wcint_t*[in_int_count];
        in_int_px = new int[in_int_count];
    }
    if (in_float_count) {
        in_float = new double*[in_float_count];
        in_float_px = new int[in_float_count];
    }
    if (out_int_count) {
        out_int = new wcint_t[out_int_count];
        out_int_px = new int[out_int_count];
    }
    if (out_float_count) {
        out_float = new double[out_float_count];
        out_float_px = new int[out_float_count];
    }
    if (param_int_count) {
        param_int = new wcint_t[param_int_count];
        param_int_px = new int[param_int_count];
    }
    if (param_float_count) {
        param_float = new double[param_float_count];
        param_float_px = new int[param_float_count];
    }

    int in_int_ix = 0;
    int in_float_ix = 0;
    int out_int_ix = 0;
    int out_float_ix = 0;
    int param_int_ix = 0;
    int param_float_ix = 0;

    if (!(port_mappings = new port_map[port_count])) {
        sm_err("Port mapping allocation for LADSPA plugin '%s' '%s' failed.",
                                                        path, label);
        invalidate();
        return false;
    }

    if (!(ports = new LADSPA_Data[port_count])) {
        sm_err("LADSPA Port data allocation failed for LADSPA plugin '%s' '%s'.\n",
                                                                    path, label);
        invalidate();
        return false;
    }

    for (px = 0; px < port_count; ++px) {
        debug("processing port index %d name '%s'\n", px, l_descriptor->PortNames[px]);
        port_map* pm = &port_mappings[px];
        pm->id = 0;
        pm->type = 0;
        pm->index = 0;
        LADSPA_PortDescriptor pd = l_descriptor->PortDescriptors[px];

        int id;
        iocat::TYPE ioc;

        if (LADSPA_IS_HINT_INTEGER(pd))
            ioc = iocat::WCINT_T;
        else
            ioc = iocat::DOUBLE;

        if (LADSPA_IS_PORT_INPUT(pd)) {
            if (LADSPA_IS_PORT_CONTROL(pd)) {
                id = param::names::register_type(l_descriptor->PortNames[px], ioc, 0);
                register_param(id);
                pm->id = id;
                pm->type = ui::UI_PARAM;
                if (ioc == iocat::WCINT_T) {
                    pm->index = param_int_ix;
                    param_int_px[param_int_ix++] = px;
                }
                else {
                    pm->index = param_float_ix;
                    param_float_px[param_float_ix++] = px;
                }
            }
            else {
                id = input::names::register_type(l_descriptor->PortNames[px], ioc, 0);
                register_input(id);
                pm->id = id;
                pm->type = ui::UI_INPUT;
                if (ioc == iocat::WCINT_T) {
                    pm->index = in_int_ix;
                    in_int_px[in_int_ix++] = px;
                }
                else {
                    pm->index = in_float_ix;
                    in_float_px[in_float_ix++] = px;
                }
            }
        }
        else if (LADSPA_IS_PORT_OUTPUT(pd)) {
            id = output::names::register_type(l_descriptor->PortNames[px], ioc, 0);
            register_output(id);
            pm->id = id;
            pm->type = ui::UI_OUTPUT;
            if (ioc == iocat::WCINT_T) {
                pm->index = out_int_ix;
                out_int_px[out_int_ix++] = px;
            }
            else {
                pm->index = out_float_ix;
                out_float_px[out_float_ix++] = px;
            }
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

    int px;
    for (px = 0; px < port_count; ++px)
        l_descriptor->connect_port(l_handle, px, &ports[px]);
}


errors::TYPE ladspa_module::validate()
{
    // FIXME: TODO
    debug("%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*\n");
    debug("%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*\n");
    debug("%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*\n");
    debug("^&*^&*^&*^&*^&* NO PARAMETER VALIDATION ON LADSPA ^&*^&*^&*^&*^&\n");
    debug("%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*\n");
    debug("%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*%%^&*\n");
    return errors::NO_ERROR;
}

void ladspa_module::run()
{
    int ix;

    for (ix = 0; ix < in_int_count; ++ix)
        ports[in_int_px[ix]] = *in_int[ix];

    for (ix = 0; ix < in_float_count; ++ix)
        ports[in_float_px[ix]] = *in_float[ix];

    for (ix = 0; ix < param_int_count; ++ix)
        ports[param_int_px[ix]] = param_int[ix];

    for (ix = 0; ix < param_float_count; ++ix)
        ports[param_float_px[ix]] = param_float[ix];

    l_descriptor->run(l_handle, 1);

    for (ix = 0; ix < out_int_count; ++ix)
        out_int[ix] = ports[out_int_px[ix]];

    for (ix = 0; ix < out_float_count; ++ix)
        out_float[ix] = ports[out_float_px[ix]];
}

