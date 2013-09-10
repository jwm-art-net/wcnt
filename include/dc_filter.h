#ifndef DC_FILTER_H
#define DC_FILTER_H

#include "synthmodule.h"

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

class dc_filter : public synthmod
{
public:
    dc_filter(char const*);
    ~dc_filter();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    // inputs
    const double* in_signal;
    // output
    double output;
    // params
    double dc_time;
    // working
    double* filter;//array
    short filterarraymax;
    short fpos;
    double filtertotal;
    void create_params();
    static bool done_params;
};

#endif
