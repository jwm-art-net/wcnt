#ifndef CONSTANT_NOTE_H
#define CONSTANT_NOTE_H

#include "synthmod.h"

class constant_note : public synthmod::base
{
public:
    constant_note(const char*);
    ~constant_note();
    void run(){};
    void init();
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    errors::TYPE validate();

private:
    char* note; /* is param and output */
    double out_freq;
    double out_phase_step;
    void set_note(const char*);
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
