#ifndef USERWAVE_H
#define USERWAVE_H

#include "userwavevertex.h"
#include "synthmod.h"
#include "linkedlist.h"

class user_wave : public synthmod::base, public linked_list<wave_vertex>
{
 public:
    user_wave(const char*);
    ~user_wave();

    wave_vertex* add_vertex(wave_vertex* wv);
    wave_vertex* add_vertex(double udp, double ul, double ldp, double ll);

    // virtual funcs
    void init();
    void run();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;
    dobj::base* add_dobj(dobj::base*);
    synthmod::base* duplicate_module(const char* uname, DUP_IO);
 private:
    STATUS const* in_phase_trig;
    double const* in_phase_step;
    double const* in_pwm;
    double const* in_h_mod;
    double const* in_v_mod;
    double output;
    STATUS play_state;
    STATUS recycle;
    STATUS zero_retrigger_mode;
    wcint_t drop_check_range;
    wave_vertex** vertices;
    wave_vertex* vertex;
    long vx_ix;
    double sect_spanlvl;
    double sect_startlvl;
    double sectdegs;
    double degs;
    double pdegs;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
