#ifndef WCNTSIGNAL_H
#define WCNTSIGNAL_H

#include "synthmod.h"

// wcnt_signal
//  if level == 1.0, then output == input == use empty_run method
//  else output = input * level == use run method
//  during init (init method required only for this)
//  connectorlist::remake_connections is called.

class wcnt_signal : public synthmod::base
{
public:
    wcnt_signal(const char*);
    ~wcnt_signal();

    // virtual funcs
    void run() { out_output = *in_signal * level; }
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    const double* in_signal;
    double out_output;
    double level;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
