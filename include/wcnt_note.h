#ifndef WCNT_NOTE_H
#define WCNT_NOTE_H

#include "synthmod.h"

class wcnt_note : public synthmod
{
 public:
    wcnt_note(const char*);
    ~wcnt_note();

    void run(){};
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

 private:
    const STATUS*   io_note_on_trig;
    const STATUS*   io_note_slide_trig;
    const STATUS*   io_note_off_trig;
    const double*   io_freq;
    const double*   io_velocity;
};

#endif
