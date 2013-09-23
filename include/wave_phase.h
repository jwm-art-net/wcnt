#ifndef WAVE_PHASE_H
#define WAVE_PHASE_H

#include "synthmod.h"
#include "wave_tables.h"

class wave_phase : public synthmod
{
public:
    wave_phase(const char*);
    ~wave_phase();
    void run();
    void init();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;
    stockerrs::ERR_TYPE validate();
private:
    STATUS const* in_phase_trig;
    double const* in_phase_step;
    double const* in_shape_phase_step;
    double output;
    double pre_shape_output;
    STATUS play_state;
    wave_tables::WT type;
    wave_tables::WT shape_type;
    STATUS recycle;
    STATUS reset_phase;
    STATUS invert_alt;
    STATUS sync_shape;
    double cycles;
    unsigned long phase;
    unsigned long shape_phase;
    double degs;
    double max_degs;
    int invph;
    const double* table;
    const double* shape_table;
    void init_first();
    
};

#endif
