#include "../include/dc_offset_remover.h"
#include "../include/globals.h"


ladspa_plug* dc_offset_remover::lp = 0;


dc_offset_remover::dc_offset_remover(const char* uname) :
 synthmod::base(synthmod::DC_OFFSET_REMOVER, uname, SM_HAS_OUT_OUTPUT),
 input(0), output(0),
 l_descriptor(0), l_handle(0), l_input(0), l_output(0)
{
    register_output(output::OUT_OUTPUT);
}


void dc_offset_remover::register_ui()
{
    register_input(input::IN_SIGNAL);

    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    char* path = ll->find_lib_path("dc_remove_1207");

    if (path) {
        lp = ll->get_plugin(path, "dcRemove");
        delete [] path;
    }
}


ui::moditem_list* dc_offset_remover::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}


dc_offset_remover::~dc_offset_remover()
{
    if (l_descriptor)
        l_descriptor->cleanup(l_handle);
}


void dc_offset_remover::init()
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
    l_descriptor->connect_port(l_handle, 0, &l_input);
    l_descriptor->connect_port(l_handle, 1, &l_output);
    l_descriptor->activate(l_handle);
}


const void* dc_offset_remover::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}


const void*
dc_offset_remover::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}


const void* dc_offset_remover::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return input;
        default: return 0;
    }
}


void dc_offset_remover::run()
{
    l_input  = *input;
    l_descriptor->run(l_handle, 1);
    output = l_output;
}

