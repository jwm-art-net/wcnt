#ifndef WAVE_PHASE_H
#define WAVE_PHASE_H

#include "synthmod.h"
#include "wave_tables.h"

class wave_phase : public synthmod::base
{
 public:
    wave_phase(const char*);
    ~wave_phase();
    void run();
    void init();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    errors::TYPE validate();

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
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
