#include "../include/dc_offset_remover.h"
#include "../include/globals.h"

dc_offset_remover::dc_offset_remover(const char* uname) :
 synthmod::base(synthmod::DC_OFFSET_REMOVER, uname, SM_HAS_OUT_OUTPUT),
 input(0), output(0),
 l_descriptor(0), l_inst_handle(0), l_input(0), l_output(0)
{
    register_output(output::OUT_OUTPUT);
}

void dc_offset_remover::register_ui()
{
    register_input(input::IN_SIGNAL);
}

ui::moditem_list* dc_offset_remover::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

dc_offset_remover::~dc_offset_remover()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input)  delete [] l_input;
    if (l_output) delete [] l_output;
}

void dc_offset_remover::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("dc_remove_1207",
                                     "dcRemove");
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
    l_output = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, l_input);
    l_descriptor->connect_port(l_inst_handle, 1, l_output);
    l_descriptor->activate(l_inst_handle);
}

const void* dc_offset_remover::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void*
dc_offset_remover::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}

const void* dc_offset_remover::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return input;
        default: return 0;
    }
}

void dc_offset_remover::run()
{
    *l_input  = *input;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}

