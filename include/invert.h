#ifndef INVERT_H
#define INVERT_H

// very simple module to invert a signal

#include "synthmod.h"

class invert : public synthmod::base
{
public:
    invert(const char*);
    ~invert();
    void run() { out_output = -*in_signal; }
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;

private:
    const double* in_signal;
    double out_output;
    void register_ui();
    ui::moditem_list* get_ui_items();
};
#endif
