#ifndef GAIN_H
#define GAIN_H

#include "modpart.h"


class gain : protected modpart::base
{
 public:
    gain(synthmod::base* inheritor);
    ~gain();
    errors::TYPE validate();
    const void* get_out(int) const { return 0; }
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

    void init();
    void run() { out = *in_signal * (center + half_range * *in_mod); }

 protected:
    double out;
    void register_ui();

 private:
    const double* in_signal;
    const double* in_mod;
    double level;
    double mod_amount;
    double center;
    double half_range;
};

#endif
