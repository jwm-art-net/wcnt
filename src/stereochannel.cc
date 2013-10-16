#include "../include/stereochannel.h"

stereo_channel::stereo_channel(const char* uname) :
 synthmod::base(synthmod::STEREOCHANNEL, uname, SM_HAS_STEREO_OUTPUT
                                      | SM_EMPTY_RUN),
 io_left(0), io_right(0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void stereo_channel::register_ui()
{
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT);
}

stereo_channel::~stereo_channel()
{
}

const void* stereo_channel::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT: return io_left;
        case output::OUT_RIGHT:return io_right;
        default: return 0;
    }
}

const void* stereo_channel::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_LEFT:   return io_left = (double*)o;
        case input::IN_RIGHT:  return io_right = (double*)o;
        default: return 0;
    }
}

const void* stereo_channel::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:   return io_left;
        case input::IN_RIGHT:  return io_right;
        default: return 0;
    }
}

