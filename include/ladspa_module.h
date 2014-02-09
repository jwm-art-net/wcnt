#ifndef LADSPA_MODULE_H
#define LADSPA_MODULE_H

#include "synthmod.h"

class ladspa_module : public synthmod::base
{
 public:
    ladspa_module(const char*);
    ~ladspa_module();
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
