#ifndef STEREOCHANNEL_H
#include "../include/stereochannel.h"

stereo_channel::stereo_channel(char const* uname) :
 synthmod(synthmodnames::MOD_STEREOCHANNEL, stereochannel_count, uname),
 io_left(NULL), io_right(NULL)
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
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        o = &io_left;
        break;
    case outputnames::OUT_RIGHT:
        o = &io_right;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* stereo_channel::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_LEFT:
        i = io_left = (short*)o;
        break;
    case inputnames::IN_RIGHT:
        i = io_right = (short*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

int stereo_channel::stereochannel_count = 0;

#endif
