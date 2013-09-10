#ifndef STEREOCHANNEL_H
#include "../include/stereochannel.h"

stereo_channel::stereo_channel(char const* uname) :
 synthmod(synthmodnames::MOD_STEREOCHANNEL, stereochannel_count, uname),
 out_left(0), out_right(0), in_left(0), in_right(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    get_inputlist()->add_input(this, inputnames::IN_LEFT);
    get_inputlist()->add_input(this, inputnames::IN_RIGHT);
    stereochannel_count++;
}

stereo_channel::~stereo_channel()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* stereo_channel::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        return &out_left;
    case outputnames::OUT_RIGHT:
        return &out_right;
    default:
        return 0;
    }
}

void const* stereo_channel::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_LEFT:
        return in_left = (short*)o;
    case inputnames::IN_RIGHT:
        return in_right = (short*)o;
    default:
        return 0;
    }
}

void const* stereo_channel::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_LEFT:
        return in_left;
    case inputnames::IN_RIGHT:
        return in_right;
    default:
        return 0;
    }
}

int stereo_channel::stereochannel_count = 0;

#endif
