#include "../include/stereochannel.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

stereo_channel::stereo_channel(char const* uname) :

 synthmod(
    synthmodnames::STEREOCHANNEL,
    uname,
    SM_HAS_STEREO_OUTPUT | SM_EMPTY_RUN),

 io_left(0), io_right(0)
{
    register_output(outputnames::OUT_LEFT);
    register_output(outputnames::OUT_RIGHT);
    register_input(inputnames::IN_LEFT);
    register_input(inputnames::IN_RIGHT);
}

stereo_channel::~stereo_channel()
{
}

void const* stereo_channel::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return io_left;
        case outputnames::OUT_RIGHT:return io_right;
        default: return 0;
    }
}

void const* stereo_channel::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return io_left = (double*)o;
        case inputnames::IN_RIGHT:  return io_right = (double*)o;
        default: return 0;
    }
}

void const* stereo_channel::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return io_left;
        case inputnames::IN_RIGHT:  return io_right;
        default: return 0;
    }
}

