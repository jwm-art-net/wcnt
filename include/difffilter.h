#ifndef DIFFFILTER_H
#define DIFFFILTER_H

#include "synthmod.h"

class diff_filter : public synthmod
{
public:
    diff_filter(const char*);
    ~diff_filter();
    void run();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
private:
    const double* in_signal;
    double out_output;
    double oldinsig;
};

#endif
