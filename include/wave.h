#ifndef WAVE_H
#define WAVE_H

#include "synthmod.h"
#include "wave_tables.h"

class wave : public synthmod
{
public:
    wave(char const*);
    ~wave();
    void run();
    void init();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    double output;
    const double* in_phase_step;
    wave_tables::WT type;
    unsigned long phase;
    const double* table;
    void init_first();
    
};

#endif
