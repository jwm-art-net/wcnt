#ifndef RANGELIMIT_H
#define RANGELIMIT_H

#include "synthmod.h"

class range_limit : public synthmod::base
{
 public:
    range_limit(const char*);
    ~range_limit();
    // virtual funcs
    void run();
    void init();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

 private:
    // inputs
    const double* in_signal;
    // outputs
    double out_output;
    // params
    double sigrangehi;
    double sigrangelo;
    void register_ui();
    ui::moditem_list* get_ui_items();
};
#endif
