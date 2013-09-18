#include "../include/dc_offset_remover.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

dc_offset_remover::dc_offset_remover(char const* uname) :
 synthmod(synthmodnames::DC_OFFSET_REMOVER, uname, SM_HAS_OUT_OUTPUT),
 input(0), output(0),
 l_descriptor(0), l_inst_handle(0), l_input(0), l_output(0)
{
    register_output(outputnames::OUT_OUTPUT);
    register_input(inputnames::IN_SIGNAL);
}

dc_offset_remover::~dc_offset_remover()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_input)  delete [] l_input;
    if (l_output) delete [] l_output;
}

void dc_offset_remover::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("dc_remove_1207",
                                     "dcRemove");
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
    l_output = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, l_input);
    l_descriptor->connect_port(l_inst_handle, 1, l_output);
    l_descriptor->activate(l_inst_handle);
}

void const* dc_offset_remover::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const*
dc_offset_remover::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return input = (double*)o;
        default: return 0;
    }
}

void const* dc_offset_remover::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return input;
        default: return 0;
    }
}

void dc_offset_remover::run()
{
    *l_input  = *input;
    l_descriptor->run(l_inst_handle, 1);
    output = *l_output;
}

