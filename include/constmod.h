#ifndef CONSTMOD_H
#define CONSTMOD_H

#include "synthmod.h"

class constmod : public synthmod::base
{
public:
    constmod(const char*);
    ~constmod();
    void run(){};
    const void* get_out(int output_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    double output;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
