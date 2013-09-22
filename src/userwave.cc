#include "../include/userwave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>
#include <math.h>

user_wave::user_wave(char const* uname) :
 synthmod(synthmodnames::USERWAVE, uname, SM_HAS_OUT_OUTPUT),
 in_phase_trig(0), in_phase_step(0), in_pwm(0), in_h_mod(0), in_v_mod(0),
 output(0.0), play_state(OFF),
 recycle(OFF), zero_retrigger_mode(OFF), drop_check_range(2),
 vertices(0), vertex(0), vx_ix(0),
 sect_spanlvl(0.0), sect_startlvl(0.0),
 sectdegs(0), degs(360), pdegs(0)
{
    register_input(inputnames::IN_PHASE_TRIG);
    register_input(inputnames::IN_PHASE_STEP);
    register_input(inputnames::IN_V_MOD);
    register_input(inputnames::IN_H_MOD);
    register_output(outputnames::OUT_OUTPUT);
    register_output(outputnames::OUT_PLAY_STATE);
    add_at_head(new wave_vertex(  0.0, 0.0,   0.0, 0.0));
    add_at_tail(new wave_vertex(360.0, 0.0, 360.0, 0.0));
    init_first();
}

user_wave::~user_wave()
{
    if (vertices)
        destroy_array_moved_from_list(vertices);
}

void const* user_wave::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* user_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP: return in_phase_step = (double*)o;
        case inputnames::IN_V_MOD:      return in_v_mod = (double*)o;
        case inputnames::IN_H_MOD:      return in_h_mod = (double*)o;
        default: return 0;
    }
}

void const* user_wave::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig;
        case inputnames::IN_PHASE_STEP: return in_phase_step;
        case inputnames::IN_V_MOD:      return in_v_mod;
        case inputnames::IN_H_MOD:      return in_h_mod;
        default: return 0;
    }
}

bool user_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        case paramnames::ZERO_RETRIGGER:
            zero_retrigger_mode = *(STATUS*)data;
            return true;
        case paramnames::DROP_CHECK_RANGE:
            drop_check_range = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* user_wave::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RECYCLE_MODE:     return &recycle;
        case paramnames::ZERO_RETRIGGER:   return &zero_retrigger_mode;
        case paramnames::DROP_CHECK_RANGE: return &drop_check_range;
        default: return 0;
    }
}

dobj* user_wave::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_VERTEX:
        if (!(retv = add_vertex((wave_vertex*)dbj)))
            sm_err("Could not add vertex to %s.", get_username());
        break;
    default:
        sm_err("%s %s to %s.", stockerrs::major, stockerrs::bad_add,
                                                  get_username());
        retv = 0;
    }
    return retv;
}

synthmod* user_wave::duplicate_module(const char* uname, DUP_IO dupio)
{
    user_wave* dup = new user_wave(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    vertex = goto_first();
    while (vertex) {
        wave_vertex* tmp;
        tmp = new wave_vertex(vertex->get_updeg(), vertex->get_uppos(),
                              vertex->get_lodeg(), vertex->get_lopos());
        if (!dup->add_vertex(tmp))
        {
            std::cout << "\ncould not duplicate vertices for copied"
                "user_wave";
            std::cout << " " << uname;
            delete dup;
            return 0;
        }
        vertex = goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE user_wave::validate()
{
    if (!jwm.get_paramlist()->validate(this,
        paramnames::DROP_CHECK_RANGE, stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", paramnames::get_name(
                                        paramnames::DROP_CHECK_RANGE));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

wave_vertex* user_wave::add_vertex(wave_vertex* wv)
{
    if (!wv)
        return 0;
    double ldeg = wv->get_lodeg();
    if (ldeg == 0) {
        llitem* tmp = sneak_first();
        delete tmp->get_data();
        tmp->set_data(wv);
        return tmp->get_data();
    }
    else
    if (ldeg == 360) {
        llitem* tmp = sneak_last();
        delete tmp->get_data();
        tmp->set_data(wv);
        return tmp->get_data();
    }
    vertex = ordered_insert(this, wv,
        &wave_vertex::get_lodeg
            )->get_data();
    if (!vertex) {
        std::cout << "\nfailed to add vertex to envelope....!?!?!?";
        return 0;
    }
    return vertex;
}

wave_vertex*
user_wave::add_vertex(double ud, double up, double ld, double lp)
{
    vertex = new wave_vertex(ud, up, ld, lp);
    if (!add_vertex(vertex) && vertex) {
        delete vertex;
        return 0;
    }
    return vertex;
}

void user_wave::init()
{
    if (!(vertices = move_to_array(this))){
        invalidate();
        return;
    }
    vertex = vertices[vx_ix = 0];
    if (zero_retrigger_mode == OFF) {
        vertex->modulate(0.5, 0.5); // just guessing!
        output = vertex->out_pos;
    }
}
#include<iostream>
void user_wave::run()
{
    if (*in_phase_trig == ON) {
        play_state = ON;
        vertex = vertices[vx_ix = 0];
        while(vertex) {
            vertex->modulate(*in_v_mod, *in_h_mod);
            vertex = vertices[++vx_ix];
        }
        vertex = vertices[vx_ix = 0];
        if (zero_retrigger_mode == ON) 
            output = vertex->out_pos;
        sect_startlvl = output;
        vertex = vertices[++vx_ix]; // first off
        sect_spanlvl = vertex->out_pos - sect_startlvl;
        sectdegs = vertex->out_deg;
        degs = 0;
        pdegs = 0;
    }
    if (play_state == ON) {
        double counter_ratio = (degs - pdegs) / (sectdegs - pdegs);
        output = sect_startlvl + sect_spanlvl * counter_ratio;
        degs += *in_phase_step;
        if (degs > sectdegs) {
            wave_vertex* tmp = vertex;
            vertex = vertices[++vx_ix];
            if (vertex) {
                sect_startlvl = tmp->out_pos;
                pdegs = tmp->out_deg;
                while(true) {
                    sectdegs = vertex->out_deg;
                    if (sectdegs > (pdegs + drop_check_range)
                        || sectdegs > degs)
                    {
                        sect_spanlvl = vertex->out_pos - sect_startlvl;
                        break;
                    }
                    vertex = vertices[++vx_ix];
                    if (!vertex) {
                        play_state = OFF;
                        break;
                    }
                }
            }
            else {
                if (*in_phase_trig == ON)
                    std::cout << "userwave at end of waveform"
                        " & in_phase_trig triggers";
                if (recycle == ON) {
                    play_state = ON;
                    vertex = vertices[vx_ix = 0];
                    while(vertex) {
                        vertex->modulate(*in_v_mod, *in_h_mod);
                        vertex = vertices[++vx_ix];
                    }
                    vertex = vertices[vx_ix = 0];
                    if (zero_retrigger_mode == ON) 
                        output = vertex->out_pos;
                    sect_startlvl = output;
                    vertex = vertices[++vx_ix];
                    sect_spanlvl = vertex->out_pos - sect_startlvl;
                    sectdegs = vertex->out_deg;
                    degs = 0;
                    pdegs = 0;
                }
                else
                    play_state = OFF;
            }
        }
    }
}

void user_wave::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::RECYCLE_MODE);
    register_param(paramnames::ZERO_RETRIGGER);
    register_param(paramnames::DROP_CHECK_RANGE);
    register_moddobj(dobjnames::LST_WAVEFORM, dobjnames::SIN_VERTEX);
}

