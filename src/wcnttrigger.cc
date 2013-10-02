#include "../include/wcnttrigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

wcnt_trigger::wcnt_trigger(const char* uname) :
 synthmod(module::WCNTTRIGGER, uname, SM_EMPTY_RUN
                                           | SM_HAS_OUT_TRIG),
 in_trig(0)
{
    register_input(input::IN_TRIG);
    register_output(output::OUT_TRIG);
}

wcnt_trigger::~wcnt_trigger()
{
}

const void* wcnt_trigger::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG: return in_trig;
        default: return 0;
    }
}

const void* wcnt_trigger::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* wcnt_trigger::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        default: return 0;
    }
}
