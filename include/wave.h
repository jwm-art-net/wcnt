#ifndef WAVE_H
#define WAVE_H

#include "synthmod.h"
#include "wave_tables.h"

class wave : public synthmod
{
public:
    wave(const char*);
    ~wave();
    void run();
    void init();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    double output;
    const double* in_phase_step;
    wave_tables::WT type;
    unsigned long phase;
    const double* table;
    void init_first();
    
};

#endif
