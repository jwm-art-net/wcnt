#ifndef DIFFFILTER_H
#define DIFFFILTER_H

#include "synthmodule.h"

class diff_filter : public synthmod
{
public:
    diff_filter(char const*);
    ~diff_filter();
    void run();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
private:
    const double* in_signal;
    double out_output;
    double oldinsig;
};

#endif
