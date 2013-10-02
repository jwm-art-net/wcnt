#ifndef RANGELIMIT_H
#define RANGELIMIT_H

#include "synthmod.h"

class range_limit : public synthmod
{
 public:
    range_limit(const char*);
    ~range_limit();
    // virtual funcs
    void run();
    void init();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    // inputs
    const double* in_signal;
    // outputs
    double out_output;
    // params
    double sigrangehi;
    double sigrangelo;
    void init_first();
    
};
#endif
