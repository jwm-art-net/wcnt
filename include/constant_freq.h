#ifndef CONSTANT_FREQ_H
#define CONSTANT_FREQ_H

#include "synthmod.h"

class constant_freq : public synthmod
{
public:
    constant_freq(const char*);
    ~constant_freq();
    void run(){};
    void init();
    const void* get_out(output::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;
    stockerrs::ERR_TYPE validate();

private:
    double out_freq; /* is param and output */
    double out_phase_step;
    void init_first();
    
};

#endif
