#include "../include/stepper.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/conversions.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/listwork.h"
#include "../include/miscfuncobj.h"

stepper::stepper(char const* uname) :
 synthmod(synthmodnames::STEPPER, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_restart_trig(0), in_modulation(0),
 step_count(0), up_thresh(0), lo_thresh(0), rtime(0), recycle(OFF),
 out_output(0),
 output(0), last_output(0), step_no(0), next_step_no(0), up_levels(0),
 lo_levels(0),
 rtime_samp(0), rtime_max_samps(0), rtime_stpsz(0), rtime_size(0)
{
    register_input(inputnames::IN_TRIG);
    register_input(inputnames::IN_RESTART_TRIG);
    register_input(inputnames::IN_MODULATION);
    register_output(outputnames::OUT_OUTPUT);
    insert_step(0.0, 0.0, 0.0);
    insert_step(1.0, 1.0, 1.0);
    init_first();
}

stepper::~stepper()
{
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
    step_data* step = goto_first();
    while(step) {
        dup->insert_step(step->get_position(), step->get_upper_level(),
                                               step->get_lower_level());
        step = goto_next();
    }
    return dup;
}

stockerrs::ERR_TYPE stepper::validate()
{
    if (!goto_first()) {
        sm_err("%s", "Step shape unformed.");
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        sm_err("%s", "Step shape needs more form.");
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::RESPONSE_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", paramnames::get_name(
                                            paramnames::RESPONSE_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (up_thresh < lo_thresh) {
        sm_err("%s must not be less than %s.",
                paramnames::get_name(paramnames::UP_THRESH),
                paramnames::get_name(paramnames::LO_THRESH));
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

step_data* stepper::insert_step(double pos, double uplvl, double lolvl)
{
    step_data* newstep = new step_data(pos, uplvl, lolvl);
    if (!newstep)
        return 0;
    if (!insert_step(newstep))
        return 0;
    return newstep;
}

step_data* stepper::insert_step(step_data* newstep)
{
    llitem* old_step_item = find_in_data(sneak_first(),
                            _pos_(newstep->get_position()));
    if (old_step_item) {
        delete old_step_item->get_data();
        old_step_item->set_data(newstep);
        return newstep;
    }
    else
        return ordered_insert(this, newstep,
            &step_data::get_position
                )->get_data();
}

dobj* stepper::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::SIN_STEP) {
        if (insert_step((step_data*)dbj))
            return dbj;
        sm_err("Could not insert %s into stepper.",
                dobjnames::get_name(dobjnames::SIN_STEP));
        return 0;
    }
    sm_err("%s %s to %s.", stockerrs::major, stockerrs::bad_add,
                                                get_username());
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
    if(!up_levels || !lo_levels){
        invalidate();
        return;
    }
    double cur_pos = 0, cur_lvl = 0, next_pos = 0, next_lvl = 0;
    double pos = 0, pos_ratio;
    double stz = 1.0 / (double)step_count;
    step_data* step = goto_first();
    step_data* next_step = 0;
    for (int i = 0; i < step_count; i++) {
        do {
            if ((next_step = sneak_next()->get_data())) {
                cur_pos = step->get_position();
                next_pos = next_step->get_position();
            }
            else {
                next_pos = 2; // otherwise pos always 0.0 ~ 1.0
                break;
            }
            if (pos >= next_pos)
                step = goto_next();
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

void stepper::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::STEP_COUNT);
    register_param(paramnames::UP_THRESH);
    register_param(paramnames::LO_THRESH);
    register_param(paramnames::RESPONSE_TIME);
    register_param(paramnames::RECYCLE_MODE);
    register_moddobj(dobjnames::LST_STEPS, dobjnames::SIN_STEP);
}

