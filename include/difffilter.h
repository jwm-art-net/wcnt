#ifndef DIFFFILTER_H
#define DIFFFILTER_H

#include "synthmod.h"

class diff_filter : public synthmod
{
public:
    diff_filter(const char*);
    ~diff_filter();
    void run();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
private:
    const double* in_signal;
    double out_output;
    double oldinsig;
};

#endif
