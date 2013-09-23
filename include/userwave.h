#ifndef USERWAVE_H
#define USERWAVE_H

#include "userwavevertex.h"
#include "synthmod.h"
#include "linkedlist.h"

class user_wave : public synthmod, public linked_list<wave_vertex>
{
public:
    user_wave(const char*);
    ~user_wave();

    wave_vertex* add_vertex(wave_vertex* wv);
    wave_vertex* add_vertex(double udp, double ul, double ldp, double ll);

    // virtual funcs
    void init();
    void run();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;
    dobj* add_dobj(dobj*);
    synthmod* duplicate_module(const char* uname, DUP_IO);
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
    short drop_check_range;
    wave_vertex** vertices;
    wave_vertex* vertex;
    long vx_ix;
    double sect_spanlvl;
    double sect_startlvl;
    double sectdegs;
    double degs;
    double pdegs;
    void init_first();
    void create_dobj();
    
    static bool done_dobj;
};

#endif
