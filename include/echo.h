#ifndef ECHO_H
#define ECHO_H

#include "gain.h"
#include "synthmod.h"


class echo : public synthmod::base, public gain
{
public:
    echo(const char*);
    ~echo();
    // virtual funcs
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const double* in_signal;
    const double* in_feedback;
    const double* in_feed_mod;
    // output
    double output;
    double wet_output;
    // params
    double delay_time;
    double feed_level;
    double feed_modsize;
    double wetdry;
    // working
    double* filter;//array
    long filterarraymax;
    long fpos;
    double filtertotal;
    double feedamount;
    // synthmod kind of stuff
    void register_ui();
};

#endif
