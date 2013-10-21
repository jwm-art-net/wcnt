#ifndef DELAY_H
#define DELAY_H

#include "synthmod.h"
#include "gain.h"

class delay : public synthmod::base, public gain
{
public:
    delay(const char*);
    ~delay();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // outputs
    double out_output;
    // params
    double delay_time;
    double wetdry;
    // working
    double output;
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
