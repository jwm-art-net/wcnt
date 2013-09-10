#ifndef USERWAVE_H
#include "../include/userwave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

#include <iostream>

user_wave::user_wave(char const* uname) :
 synthmod(synthmodnames::USERWAVE, uname),
 in_phase_trig(0), in_phase_step(0), in_h_mod(0), in_v_mod(0),
 output(0.0), play_state(OFF), env(0), vertex_item(0), vertex(0),
 sect(0), sect_spanlvl(0.0), sect_startlvl(0.0),
 counter_ratio(0), sectdegs(0), degs(360), recycle(OFF),
 zero_retrigger_mode(OFF)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist().add_input(this, inputnames::IN_PHASE_TRIG);
    jwm.get_inputlist().add_input(this, inputnames::IN_PHASE_STEP);
    jwm.get_inputlist().add_input(this, inputnames::IN_V_MOD);
    jwm.get_inputlist().add_input(this, inputnames::IN_H_MOD);
    env =
        new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
    // wcnt-1.25
    // been having problems here, need to add this middle vertex in order
    // for the user to be able to add them, or something. I'd changed
    // the insert code slightly, due to duplicate_module in synthmod.
    add_vertex(  0.0, 0.0,   0.0, 0.0);
    add_vertex(180.0, 0.0, 180.0, 0.0);
    add_vertex(360.0, 0.0, 360.0, 0.0);
    create_params();
    create_dobj();
}

user_wave::~user_wave()
{
    goto_first();
    while (vertex)
    {
        tmp = vertex;
        goto_next();
        delete tmp;
    }
    delete env;
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
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
        default:
            return false;
    }
}

void const* user_wave::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RECYCLE_MODE:  return &recycle;
        case paramnames::ZERO_RETRIGGER:return &zero_retrigger_mode;
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
            *err_msg="\ncould not add vertex to " + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
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
    goto_first();
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
        goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE user_wave::validate()
{
    // still can't be bothered! (1.2)
    return stockerrs::ERR_NO_ERROR;
}

wave_vertex* user_wave::add_vertex(wave_vertex* wv)
{
    if (!wv)
        return 0;
    double ldeg = wv->get_lodeg();
    if (ldeg == 0) {
        goto_first();
        delete env->unlink_item(vertex_item);
        delete vertex;
        env->add_at_head(wv);
        return wv;
    } else if (ldeg == 360) {
        goto_last();
        delete env->unlink_item(vertex_item);
        delete vertex;
        env->add_at_tail(wv);
        return wv;
    }
    wave_vertex* added = ordered_insert(env, wv, &wave_vertex::get_lodeg);
    if (!added) {
        std::cout << "\nfailed to add vertex to envelope....!?!?!?";
        return 0;
    }
    return added;
}

wave_vertex* user_wave::add_vertex(
                        double ud, double up, double ld, double lp)
{
    wave_vertex* tmp = new wave_vertex(ud, up, ld, lp);
    return add_vertex(tmp);
}

bool user_wave::delete_vertex(wave_vertex* wv)
{
    if (!wv)
        return false;
    if (wv == get_first() || wv == get_last())
        return false;  // don't allow user to delete initial or end
    ll_item* tmp = env->find_data(wv);
    if (tmp == 0)
        return false;
    delete env->unlink_item(tmp);
    delete wv;
    return true;
}

void user_wave::init()
{
    goto_first();
    if (zero_retrigger_mode == OFF) {
        vertex->modulate(0.5, 0.5); // just guessing!
        output = vertex->out_pos;
    }

}

void user_wave::run()
{
    if (*in_phase_trig == ON) {
        play_state = ON;
        goto_first();
        while(vertex) {
            vertex->modulate(*in_v_mod, *in_h_mod);
            goto_next();
        }
        goto_first();
        if (zero_retrigger_mode == ON) 
            output = vertex->out_pos;
        sect_startlvl = output;
        goto_next(); // first off
        sect_spanlvl = vertex->out_pos - sect_startlvl;
        sectdegs = vertex->out_deg;
        degs = 0;
        pdegs = 0;
    }
    degs += *in_phase_step;
    if (play_state == ON) {
        counter_ratio = (degs - pdegs) / (sectdegs - pdegs);
        output = sect_startlvl + sect_spanlvl * counter_ratio;
        if (degs > sectdegs) {
            tmp = vertex;
            goto_next();
            if (vertex) {
                sect_startlvl = output;
                sect_spanlvl = vertex->out_pos - sect_startlvl;
                pdegs = tmp->out_deg;
                sectdegs = vertex->out_deg;
            } else {
                if (*in_phase_trig == ON)
                    std::cout << "userwave at end of waveform"
                        " & in_phase_trig";
                if (recycle == ON) {
                    play_state = ON;
                    goto_first();
                    while(vertex) {
                        vertex->modulate(*in_v_mod, *in_h_mod);
                        goto_next();
                    }
                    goto_first();
                    if (zero_retrigger_mode == ON) 
                        output = vertex->out_pos;
                    sect_startlvl = output;
                    goto_next();
                    sect_spanlvl = vertex->out_pos - sect_startlvl;
                    sectdegs = vertex->out_deg;
                    degs = 0;
                    pdegs = 0;
                } else {
                    play_state = OFF;
                    //output = 0;
                }
            }
        }
    }
    if (degs >= 360) degs -= 360;
}

bool user_wave::done_params = false;
bool user_wave::done_dobj = false;

void user_wave::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().
        add_param(synthmodnames::USERWAVE, paramnames::RECYCLE_MODE);
    jwm.get_paramlist().add_param(
        synthmodnames::USERWAVE, paramnames::ZERO_RETRIGGER);
    done_params = true;
}

void user_wave::create_dobj()
{
    if (done_dobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::USERWAVE, dobjnames::LST_WAVEFORM);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_WAVEFORM, dobjnames::SIN_VERTEX);
    done_dobj = true;
}

#endif
