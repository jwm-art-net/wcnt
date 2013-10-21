#ifndef WCNTTRIGGER_H
#define WCNTTRIGGER_H

#include "synthmod.h"

// bit of a relic from way back when the trig_switcher was first
// implemented and could only accept wcnt_trigger modules.

class wcnt_trigger : public synthmod::base
{
 public:
    wcnt_trigger(const char*);
    ~wcnt_trigger();

    void run(){};
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

 private:
    const STATUS* in_trig;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
