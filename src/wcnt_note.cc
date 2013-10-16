#include "../include/wcnt_note.h"

wcnt_note::wcnt_note(const char* uname) :
 synthmod::base(synthmod::WCNT_NOTE, uname, SM_EMPTY_RUN),
 io_note_on_trig(0), io_note_slide_trig(0), io_note_off_trig(0),
 io_freq(0), io_velocity(0)
{
    register_output(output::OUT_NOTE_ON_TRIG);
    register_output(output::OUT_NOTE_SLIDE_TRIG);
    register_output(output::OUT_NOTE_OFF_TRIG);
    register_output(output::OUT_FREQ);
    register_output(output::OUT_VELOCITY);
}

void wcnt_note::register_ui()
{
    register_input(input::IN_NOTE_ON_TRIG);
    register_input(input::IN_NOTE_SLIDE_TRIG);
    register_input(input::IN_NOTE_OFF_TRIG);
    register_input(input::IN_FREQ);
    register_input(input::IN_VELOCITY);
}

wcnt_note::~wcnt_note()
{
}

const void* wcnt_note::get_out(output::TYPE ot) const
{
    switch(ot)
    {
    case output::OUT_NOTE_ON_TRIG:      return io_note_on_trig;
    case output::OUT_NOTE_SLIDE_TRIG:   return io_note_slide_trig;
    case output::OUT_NOTE_OFF_TRIG:     return io_note_off_trig;
    case output::OUT_FREQ:              return io_freq;
    case output::OUT_VELOCITY:          return io_velocity;
    default: return 0;
    }
}

const void* wcnt_note::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_NOTE_ON_TRIG:
        return io_note_on_trig = (const STATUS*)o;
    case input::IN_NOTE_SLIDE_TRIG:
        return io_note_slide_trig = (const STATUS*)o;
    case input::IN_NOTE_OFF_TRIG:
        return io_note_off_trig = (const STATUS*)o;
    case input::IN_FREQ:
        return io_freq = (const double*)o;
    case input::IN_VELOCITY:
        return io_velocity = (const double*)o;
    default:
        return 0;
    }
}

const void* wcnt_note::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_NOTE_ON_TRIG:    return io_note_on_trig;
    case input::IN_NOTE_SLIDE_TRIG: return io_note_slide_trig;
    case input::IN_NOTE_OFF_TRIG:   return io_note_off_trig;
    case input::IN_FREQ:            return io_freq;
    case input::IN_VELOCITY:        return io_velocity;
    default: return 0;
    }
}
