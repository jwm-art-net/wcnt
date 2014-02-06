#ifndef WCNT_NOTE_H
#define WCNT_NOTE_H

#include "synthmod.h"

class wcnt_note : public synthmod::base
{
 public:
    wcnt_note(const char*);
    ~wcnt_note();

    void run(){};
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;

 private:
    const STATUS*   io_note_on_trig;
    const STATUS*   io_note_slide_trig;
    const STATUS*   io_note_off_trig;
    const double*   io_freq;
    const double*   io_velocity;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
