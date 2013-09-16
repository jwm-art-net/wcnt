#ifndef RANGELIMIT_H
#define RANGELIMIT_H

#include "synthmod.h"

class range_limit : public synthmod
{
 public:
    range_limit(char const*);
    ~range_limit();
    // virtual funcs
    void run();
    void init();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

 private:
    // inputs
    const double* in_signal;
    // outputs
    double out_output;
    // params
    double sigrangehi;
    double sigrangelo;
    void create_params();
    
};
#endif
