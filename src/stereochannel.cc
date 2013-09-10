#ifndef STEREOCHANNEL_H
#include "../include/stereochannel.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

stereo_channel::stereo_channel(char const* uname) :
 synthmod(synthmodnames::STEREOCHANNEL, uname),
 out_left(0), out_right(0), in_left(0), in_right(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_RIGHT);
    jwm.get_inputlist().add_input(this, inputnames::IN_LEFT);
    jwm.get_inputlist().add_input(this, inputnames::IN_RIGHT);
}

stereo_channel::~stereo_channel()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* stereo_channel::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return &out_left;
        case outputnames::OUT_RIGHT:return &out_right;
        default: return 0;
    }
}

void const* stereo_channel::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return in_left = (double*)o;
        case inputnames::IN_RIGHT:  return in_right = (double*)o;
        default: return 0;
    }
}

void const* stereo_channel::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return in_left;
        case inputnames::IN_RIGHT:  return in_right;
        default: return 0;
    }
}

#endif
