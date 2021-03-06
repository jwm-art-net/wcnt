#ifndef DC_FILTER_H
#define DC_FILTER_H

#include "synthmod.h"

// I have wanted one of these for quite a long while.. had a look at
// musicdsp.org etc and found some code snippets and mathamatical
// formalae which I did n't really understand, and then it occurred
// to me how to do it and this is the result, fairly straight forward
// actually.....

// copied existing filter classes and just subtract original signal
// from the average of the sum of x amount of samples as specified
// by dc_time.

// obviously it's not perfect method, but it does remove majority
// of dc, but some waveforms get inverted etc, but it does not
// seem to add artifacts that i have noticed....

class dc_filter : public synthmod::base
{
public:
    dc_filter(const char*);
    ~dc_filter();
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
    // output
    double output;
    // params
    double dc_time;
    // working
    double* filter;//array
    wcint_t filterarraymax;
    wcint_t fpos;
    double filtertotal;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
