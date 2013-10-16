#include "../include/stepper.h"
#include "../include/listwork.h"
#include "../include/conversions.h"

stepper::stepper(const char* uname) :
 synthmod::base(synthmod::STEPPER, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_restart_trig(0), in_modulation(0),
 step_count(0), up_thresh(0), lo_thresh(0), rtime(0), recycle(OFF),
 out_output(0),
 output(0), last_output(0), step_no(0), next_step_no(0), up_levels(0),
 lo_levels(0),
 rtime_samp(0), rtime_max_samps(0), rtime_stpsz(0), rtime_size(0)
{
    register_output(output::OUT_OUTPUT);
    insert_step(0.0, 0.0, 0.0);
    insert_step(1.0, 1.0, 1.0);
}

void stepper::register_ui()
{
    register_dobj(dobj::LST_STEPS, dobj::SIN_STEP);
    register_input(input::IN_TRIG);
    register_input(input::IN_RESTART_TRIG);
    register_input(input::IN_MODULATION);
    register_param(param::STEP_COUNT);
    register_param(param::UP_THRESH);
    register_param(param::LO_THRESH);
    register_param(param::RESPONSE_TIME);
    register_param(param::RECYCLE_MODE);
}

stepper::~stepper()
{
    if (up_levels) delete [] up_levels;
    if (lo_levels) delete [] lo_levels;
}

const void* stepper::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* stepper::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:
            return in_trig = (STATUS*)o;
        case input::IN_RESTART_TRIG:
            return in_restart_trig = (STATUS*)o;
        case input::IN_MODULATION:
            return in_modulation = (double*)o;
        default:
            return 0;
    }
}

const void* stepper::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG:           return in_trig;
        case input::IN_RESTART_TRIG:   return in_restart_trig;
        case input::IN_MODULATION:     return in_modulation;
        default: return 0;
    }
}

bool stepper::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::STEP_COUNT:
            step_count = *(wcint_t*)data;
            return true;
        case param::UP_THRESH:
            up_thresh = *(double*)data;
            return true;
        case param::LO_THRESH:
            lo_thresh = *(double*)data;
            return true;
        case param::RESPONSE_TIME:
            rtime = *(double*)data;
            return true;
        case param::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* stepper::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::STEP_COUNT:    return &step_count;
        case param::UP_THRESH:     return &up_thresh;
        case param::LO_THRESH:     return &lo_thresh;
        case param::RESPONSE_TIME: return &rtime;
        case param::RECYCLE_MODE:  return &recycle;
        default: return 0;
    }
}

synthmod::base* stepper::duplicate_module(const char* uname, DUP_IO dupio)
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

errors::TYPE stepper::validate()
{
    if (!goto_first()) {
        sm_err("%s", "Step shape unformed.");
        invalidate();
        return errors::ERROR;
    }
    else if (!goto_next()){
        sm_err("%s", "Step shape needs more form.");
        invalidate();
        return errors::ERROR;
    }
    if (!validate_param(param::RESPONSE_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (up_thresh < lo_thresh) {
        sm_err("%s must not be less than %s.",
                param::names::get(param::UP_THRESH),
                param::names::get(param::LO_THRESH));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
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

dobj::base* stepper::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::SIN_STEP) {
        if (insert_step((step_data*)dbj))
            return dbj;
        sm_err("Could not insert %s into stepper.",
                dobj::names::get(dobj::SIN_STEP));
        return 0;
    }
    sm_err("%s %s to %s.", errors::stock::major, errors::stock::bad_add,
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
    double cur_pos = 0, next_pos = 0;
    double pos = 0;
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
        double pos_ratio = (pos - cur_pos) / (next_pos - cur_pos);
        double cur_lvl = step->get_upper_level();
        double next_lvl = next_step->get_upper_level();
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


