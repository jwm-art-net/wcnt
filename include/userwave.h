#ifndef USERWAVE_H
#define USERWAVE_H

#include "userwavevertex.h"
#include "synthmodule.h"
#include "linkedlist.h"

class user_wave : public synthmod
{
public:
    user_wave(char const*);
    ~user_wave();
    wave_vertex* add_vertex(wave_vertex* wv);
    wave_vertex* add_vertex(double udp, double ul, double ldp, double ll);
    bool delete_vertex(wave_vertex* wv);
    wave_vertex* goto_first() {
        return vertex = (wave_vertex*)
         (vertex_item = env->goto_first())->get_data();
    }
    wave_vertex* goto_last() {
        return vertex = (wave_vertex*)
         (vertex_item = env->goto_last())->get_data();
    }
    wave_vertex* goto_prev() {
        return vertex = (wave_vertex*)
         (vertex_item = env->goto_prev())->get_data();
    }
    wave_vertex* goto_next() {
        return vertex = (wave_vertex*)
         (vertex_item = env->goto_next())->get_data();
    }
    wave_vertex* get_first() {
        return (wave_vertex*)(env->sneak_first())->get_data();
    }
    wave_vertex* get_last() {
        return (wave_vertex*)(env->sneak_last())->get_data();
    }
    wave_vertex* get_prev() {
        return (wave_vertex*)(env->sneak_prev())->get_data();
    }
    wave_vertex* get_next() {
        return (wave_vertex*)(env->sneak_next())->get_data();
    }
    // virtual funcs
    void init();
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    void const* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;
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
    linkedlist* env;
    ll_item* vertex_item;
    wave_vertex* vertex;
    wave_vertex* tmp;
    short sect;
    double sect_spanlvl;
    double sect_startlvl;
    double counter_ratio;
    double sectdegs;
    double degs;
    double pdegs;
    STATUS recycle;
    STATUS zero_retrigger_mode;
    void create_params();
    void create_dobj();
    static bool done_params;
    static bool done_dobj;
};

#endif
