#ifndef CONSTANT_FREQ_H
#define CONSTANT_FREQ_H

#include "synthmodule.h"

class constant_freq : public synthmod
{
public:
    constant_freq(char const*);
    ~constant_freq();
    void run(){};
    void init();
    void const* get_out(outputnames::OUT_TYPE) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
    stockerrs::ERR_TYPE validate();

private:
    double out_freq; /* is param and output */
    double out_phase_step;
    void create_params();
    static bool done_params;
};

#endif
