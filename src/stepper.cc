#ifndef STEPPER_H
#include "../include/stepper.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmodule.h"
#include "../include/synthmodulelist.h"
#include "../include/conversions.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"

stepper::stepper(char const* uname) :
 synthmod(synthmodnames::STEPPER, uname),
 in_trig(0), in_restart_trig(0), in_modulation(0),
 step_count(0), rtime(0), recycle(OFF),
 out_output(0),
 output(0), last_output(0), step_no(0), up_levels(0), lo_levels(0),
 rtime_samp(0), rtime_max_samps(0), rtime_stpsz(0), rtime_size(0),
 steplist(0), step_item(0), step(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_TRIG);
    jwm.get_inputlist().add_input(this, inputnames::IN_RESTART_TRIG);
    jwm.get_inputlist().add_input(this, inputnames::IN_MODULATION);
    steplist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    insert_step(new step_data(0.0, 0.0, 0.0));
    insert_step(new step_data(1.0, 1.0, 1.0));
    create_params();
    create_moddobj();
}

stepper::~stepper()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
    goto_first();
    while(step) {
        delete step;
        goto_next();
    }
    delete steplist;
    if (up_levels) delete [] up_levels;
    if (lo_levels) delete [] lo_levels;
}

void const* stepper::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* stepper::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:
            return in_trig = (STATUS*)o;
        case inputnames::IN_RESTART_TRIG:
            return in_restart_trig = (STATUS*)o;
        case inputnames::IN_MODULATION:
            return in_modulation = (double*)o;
        default:
            return 0;
    }
}

void const* stepper::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:           return in_trig;
        case inputnames::IN_RESTART_TRIG:   return in_restart_trig;
        case inputnames::IN_MODULATION:     return in_modulation;
        default: return 0;
    }
}

bool stepper::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::STEP_COUNT:
            step_count = *(short*)data;
            return true;
        case paramnames::UP_THRESH:
            up_thresh = *(double*)data;
            return true;
        case paramnames::LO_THRESH:
            lo_thresh = *(double*)data;
            return true;
        case paramnames::RESPONSE_TIME:
            rtime = *(double*)data;
            return true;
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* stepper::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::STEP_COUNT:    return &step_count;
        case paramnames::UP_THRESH:     return &up_thresh;
        case paramnames::LO_THRESH:     return &lo_thresh;
        case paramnames::RESPONSE_TIME: return &rtime;
        case paramnames::RECYCLE_MODE:  return &recycle;
        default: return 0;
    }
}

synthmod* stepper::duplicate_module(const char* uname, DUP_IO dupio)
{
    stepper* dup = new stepper(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dup);
    duplicate_params_to(dup);
    goto_first();
    while(step) {
        dup->insert_step(step->get_position(), step->get_upper_level(),
                                               step->get_lower_level());
        goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE stepper::validate()
{
    if (!goto_first()) {
        *err_msg = "step shape unformed";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg = "step shape needs more form";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist().validate(this, paramnames::RESPONSE_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
            jwm.get_paramnames().get_name(paramnames::RESPONSE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (up_thresh < lo_thresh) {
        *err_msg =
         jwm.get_paramnames().get_name(paramnames::UP_THRESH);
        *err_msg += " must not be less than ";
        *err_msg +=
         jwm.get_paramnames().get_name(paramnames::LO_THRESH);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

step_data* stepper::insert_step(double pos, double uplvl, double lolvl)
{
    step_data* newstep = new step_data(pos, uplvl, lolvl);
    return insert_step(newstep);
}

step_data* stepper::insert_step(step_data* newstep)
{
    step_data* oldstep = lookup_data_match(
                            steplist, newstep, &step_data::get_position);
    if (oldstep) delete_step(oldstep);
    return ordered_insert(steplist, newstep, &step_data::get_position);
}

bool stepper::delete_step(step_data* oldstep)
{
    if (!oldstep) return false;
    ll_item* tmp = steplist->find_data(oldstep);
    if (!tmp) return false;
    delete steplist->unlink_item(tmp);
    delete oldstep;
    return true;
}

dobj* stepper::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::SIN_STEP) {
        if (insert_step((step_data*)dbj))
            return dbj;
        *err_msg = "\ncould not insert ";
        *err_msg += jwm.get_dobjnames().get_name(dobjnames::SIN_STEP);
        *err_msg += " into stepper";
        return 0;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type of ";
    *err_msg += jwm.get_dobjnames().get_name(dbj->get_object_type());
    *err_msg += " to ";
    *err_msg += get_username();
    return 0;
}

void stepper::init()
{
    if (rtime > 0) {
        rtime_max_samps = ms_to_samples(rtime);
        rtime_stpsz = 1 / (double)rtime_max_samps;
    }
    up_levels = new double[step_count];
    lo_levels = new double[step_count];
    double cur_pos = 0, cur_lvl = 0, next_pos = 0, next_lvl = 0;
    double pos = 0, pos_ratio;
    double stz = 1.0 / (double)step_count;
    goto_first();
    ll_item* next_step_item = step_item->get_next();
    step_data* next_step = (step_data*)next_step_item->get_data();
    for (int i = 0; i < step_count; i++) {
        do {
            if ((next_step_item = step_item->get_next())) {
                cur_pos = step->get_position();
                next_step = (step_data*)next_step_item->get_data();
                next_pos = next_step->get_position();
            }
            else {
                next_pos = 2; // otherwise pos always 0.0 ~ 1.0
                break;
            }
            if (pos >= next_pos) goto_next();
        }while (pos >= next_pos);
        pos_ratio = (pos - cur_pos) / (next_pos - cur_pos);
        cur_lvl = step->get_upper_level();
        next_lvl = next_step->get_upper_level();
        up_levels[i] = cur_lvl + (next_lvl - cur_lvl) * pos_ratio;
        cur_lvl = step->get_lower_level();
        next_lvl = next_step->get_lower_level();
        lo_levels[i] = cur_lvl + (next_lvl - cur_lvl) * pos_ratio;
        pos += stz;
    }
    step_no = 0;
    next_step_no = 1;
    out_output = 0;
}

void stepper::run()
{
    if (*in_restart_trig == ON) {
        next_step_no = 0;
    }
    if (*in_trig == ON) {
        step_no = (next_step_no != -1) ? next_step_no : step_no;
        double premod = *in_modulation;
        if (premod < lo_thresh) premod = lo_thresh;
        else if (premod > up_thresh) premod = up_thresh;
        double mod = (premod - lo_thresh) / (up_thresh - lo_thresh);
        last_output = out_output;
        output = lo_levels[step_no] +
                        (up_levels[step_no] - lo_levels[step_no]) * mod;
        rtime_samp = rtime_max_samps;
        rtime_size = 0;
        if (++next_step_no >= step_count)
            next_step_no = (recycle == ON) ? 0 : -1;
    }
    if (rtime_samp > 0) {
        out_output =
            last_output * (1 - rtime_size) + output * rtime_size;
        rtime_samp--;
        rtime_size += rtime_stpsz;
    }
    else out_output = output;
}

bool stepper::done_params = false;

void stepper::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param( synthmodnames::STEPPER,
                                        paramnames::STEP_COUNT);
    jwm.get_paramlist().add_param( synthmodnames::STEPPER,
                                        paramnames::UP_THRESH);
    jwm.get_paramlist().add_param( synthmodnames::STEPPER,
                                        paramnames::LO_THRESH);
    jwm.get_paramlist().add_param( synthmodnames::STEPPER,
                                        paramnames::RESPONSE_TIME);
    jwm.get_paramlist().add_param( synthmodnames::STEPPER,
                                        paramnames::RECYCLE_MODE);
    done_params = true;
}

bool stepper::done_moddobj = false;

void stepper::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist().add_moddobj(
        synthmodnames::STEPPER, dobjnames::LST_STEPS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_STEPS, dobjnames::SIN_STEP);
    done_moddobj = true;
}

#endif
