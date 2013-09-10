#ifndef ADSR_H
#include "../include/adsr.h"

adsr::adsr(char const* uname) :
 synthmod(synthmodnames::MOD_ADSR, adsr_count, uname),
 in_note_on_trig(0), in_note_off_trig(0), in_velocity(0), output(0),
 out_off_trig(OFF), play_state(OFF), up_thresh(0), lo_thresh(0),
 start_level(0), min_time(0), max_sus_time(0), sustain_status(OFF),
 release_ratio(OFF), zero_retrigger(OFF), thresh_range(0), end_level(0),
 released(OFF), tsamps(0), min_samps(0), max_sus_samps(0), env(0),
 sect(0), sectsample(0), sectmaxsamples(0), levelsize(0), coord_item(0),
 coord(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_outputlist()->add_output(this, outputnames::OUT_OFF_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    get_inputlist()->add_input(this, inputnames::IN_NOTE_ON_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_NOTE_OFF_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_VELOCITY);
    env = new 
     linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);

    coord = new adsr_coord(adsr_coord::ADSR_SUSTAIN, 0 ,0 ,0 ,0);
    env->add_at_head(coord);
    adsr_count++;
    create_params();
    create_moddobj();
}

adsr::~adsr()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    if (env) {
        goto_first();
        while (coord) {
            delete coord;
            goto_next();
        }
        delete env;
    }
}

const void *
adsr::get_out(outputnames::OUT_TYPE ot)
{
    switch (ot) {
    case outputnames::OUT_OUTPUT:
        return &output;
    case outputnames::OUT_OFF_TRIG:
        return &out_off_trig;
    case outputnames::OUT_PLAY_STATE:
        return &play_state;
    default:
        return 0;
    }
}

const void *
adsr::set_in(inputnames::IN_TYPE it, void const *o)
{
    switch (it)
    {
    case inputnames::IN_VELOCITY:
        return in_velocity = (double *) o;
    case inputnames::IN_NOTE_ON_TRIG:
        return in_note_on_trig = (STATUS *) o;
    case inputnames::IN_NOTE_OFF_TRIG:
        return in_note_off_trig = (STATUS *) o;
    default:
        return 0;
    }
}

const void *
adsr::get_in(inputnames::IN_TYPE it)
{
    switch (it)
    {
    case inputnames::IN_VELOCITY:
        return in_velocity;
    case inputnames::IN_NOTE_ON_TRIG:
        return in_note_on_trig;
    case inputnames::IN_NOTE_OFF_TRIG:
        return in_note_off_trig;
    default:
        return 0;
    }
}

bool adsr::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_START_LEVEL:
        set_start_level(*(double*)data);
        return true;
    case paramnames::PAR_SUSTAIN_STATUS:
        set_sustain_status(*(STATUS*)data);
        return true;
    case paramnames::PAR_RELEASE_RATIO:
        set_release_ratio(*(STATUS*)data);
        return true;
    case paramnames::PAR_ZERO_RETRIGGER:
        set_zero_retrigger_mode(*(STATUS*)data);
        return true;
    case paramnames::PAR_UP_THRESH:
        set_upper_thresh(*(double*)data);
        return true;
    case paramnames::PAR_LO_THRESH:
        set_lower_thresh(*(double*)data);
        return true;
    case paramnames::PAR_MIN_TIME:
        set_min_time(*(double*)data);
        return true;
    case paramnames::PAR_MAX_SUSTAIN_TIME:
        set_max_sustain_time(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* adsr::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_START_LEVEL:
        return &start_level;
    case paramnames::PAR_SUSTAIN_STATUS:
        return &sustain_status;
    case paramnames::PAR_ZERO_RETRIGGER:
        return &zero_retrigger;
    case paramnames::PAR_UP_THRESH:
        return &up_thresh;
    case paramnames::PAR_LO_THRESH:
        return &lo_thresh;
    case paramnames::PAR_RELEASE_RATIO:
        return &release_ratio;
    case paramnames::PAR_MIN_TIME:
        return &min_time;
    case paramnames::PAR_MAX_SUSTAIN_TIME:
        return &max_sus_time;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE adsr::validate()
{
    if (!goto_section(adsr_coord::ADSR_ATTACK)) {
        *err_msg = "adsr lacks attack section!";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!goto_section(adsr_coord::ADSR_DECAY)) {
        *err_msg = "adsr lacks decay section!";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!goto_section(adsr_coord::ADSR_RELEASE)) {
        *err_msg = "adsr lacks release section!";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (up_thresh < lo_thresh) {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_UP_THRESH);
        *err_msg += " must not be less than ";
        *err_msg +=
         get_paramnames()->get_name(paramnames::PAR_LO_THRESH);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_MIN_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_MIN_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_MAX_SUSTAIN_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_MAX_SUSTAIN_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* adsr::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dobjnames::DOBJ_TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dobjnames::SIN_COORD:
        if (!(retv = insert_coord((adsr_coord*)dbj)))
            *err_msg="\ncould not section to " + *get_username();
        break;
    default:
        *err_msg = "\n***major error*** attempt made to add an ";
        *err_msg += "\ninvalid object type to " + *get_username();
        retv = 0;
    }
    return retv;
}

adsr_coord* adsr::insert_coord(adsr_coord* ac)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return 0; // don't let user try it on.
    if (ac->get_adsr_section() < goto_first()->get_adsr_section())
        return (adsr_coord*)(env->add_at_head(ac))->get_data();
    adsr_coord *tmp = 0;
    ll_item* lltmp = 0;
    while (coord) {
        lltmp = coord_item;
        tmp = coord;
        goto_next();
        if (coord)
            if (coord->get_adsr_section() > ac->get_adsr_section())
                break;
    }
    return (adsr_coord*)(env->insert_after(lltmp, ac))->get_data();
}

adsr_coord* adsr::insert_coord(
 adsr_coord::SECT adsrsect, double ut, double ul, double lt, double ll)
{
    adsr_coord* nc = new adsr_coord(adsrsect, ut, ul, lt, ll);
    if (!insert_coord(nc)) {
        delete nc;
        nc = 0;
    }
    return nc;
}

adsr_coord * adsr::insert_coord_after(adsr_coord * ac)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return NULL;
    ll_item* tmp = env->find_data(ac);
    if (tmp == NULL)
        return NULL;
    adsr_coord* nac = (adsr_coord *)tmp->get_next()->get_data();
    double nut = ac->get_upper_time();
    double nlt = ac->get_lower_time();
    double nul;
    double nll;
    if (nac == NULL) {
        // as there is no coord after ac, generate it's 
        // levels from the previous coord 
        nul = 0;
        nll = 0;
        adsr_coord* pac = (adsr_coord *)tmp->get_prev()->get_data();
        if (pac == NULL) { // paranoia?
            invalidate();
            return NULL;
        }
        ac->set_upper_level(pac->get_upper_level() / 2);
        ac->set_lower_level(pac->get_lower_level() / 2);
    } else {
        nul = (ac->get_upper_level() + nac->get_upper_level()) / 2;
        nll = (ac->get_lower_level() + nac->get_lower_level()) / 2;
    }
    adsr_coord* newcoord = 
     new adsr_coord(ac->get_adsr_section(), nut, nul, nlt, nll);
    if (!env->insert_after(tmp, newcoord)->get_data()) {
        delete newcoord;
        return 0;
    }
    return newcoord;
}

adsr_coord * adsr::insert_coord_after(
 adsr_coord * ac, double ut, double ul, double lt, double ll)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return NULL;
    ll_item *tmp = env->find_data(ac);
    if (tmp == NULL)
        return NULL;
    adsr_coord *newcoord = 
     new adsr_coord(ac->get_adsr_section(), ut, ul, lt, ll);
    if (!env->insert_after(tmp, newcoord)->get_data()) {
        delete newcoord;
        return 0;
    }
    return newcoord;
}

void adsr::delete_coord(adsr_coord * ac)
{
    ll_item *tmp = env->find_data(ac);
    if (tmp == NULL)
        return;
    adsr_coord *pac = (adsr_coord *) tmp->get_prev()->get_data();
    adsr_coord *nac = (adsr_coord *) tmp->get_next()->get_data();
    if (pac == NULL || nac == NULL)
        return;                 /* don't delete first or last coord */
    if (pac->get_adsr_section() == ac->get_adsr_section() 
     || nac->get_adsr_section() == ac->get_adsr_section())
    {
        delete env->unlink_item(tmp);
        delete ac;
    }
    /* else ac is the only coord in section, so do not delete it */
    return;
}

adsr_coord * adsr::goto_section(adsr_coord::SECT n)
{
    goto_first();
    while (coord) {
        if (coord->get_adsr_section() == n)
            return coord;
        goto_next();
    }
    return NULL;
}

void adsr::scale_section(adsr_coord::SECT n, double ratio)
{
    if (ratio <= 0.00)
        return;
    if (n < adsr_coord::ADSR_ATTACK || n > adsr_coord::ADSR_RELEASE)
        return;
    goto_first();
    while (coord->get_adsr_section() != n)
        goto_next();
    while (coord->get_adsr_section() == n) {
        coord->set_upper_time(coord->get_upper_time() * ratio);
        coord->set_lower_time(coord->get_lower_time() * ratio);
        if (!goto_next())
            return;
    }
}

void adsr::init()
{
    thresh_range = up_thresh - lo_thresh;
    min_samps = ms_to_samples(min_time);
    max_sus_samps = ms_to_samples(max_sus_time);
    output = start_level;
}

void adsr::run()
{
    if (*in_note_on_trig == ON) {
        play_state = ON;
        released = OFF;
        tsamps = 0;
        max_samps = (unsigned long)-1;
        goto_first();
        if (zero_retrigger == ON)
            output = start_level;
        ready_section();
    }
    if (play_state == ON) {
        if (sustain_status == ON) {
            if (*in_note_off_trig == ON && released == OFF) {
                if (tsamps >= min_samps) {
                    released = ON;
                    goto_section(adsr_coord::ADSR_RELEASE);
                    ready_section();
                }
                else
                    max_samps = min_samps;
            }
            if (tsamps >= max_samps && released == OFF) {
                released = ON;
                goto_section(adsr_coord::ADSR_RELEASE);
                ready_section();
            }
        }
        sectsample++;
        tsamps++;
        output += levelsize;
        if (sectsample >= sectmaxsamples) {
            sectsample = 0;
            end_level = coord->output_level;// grab while it's hot.
            goto_next();
            if (!coord) {
                if (release_ratio == ON)
                    output *= end_level;
                else
                    output = end_level;
                play_state = OFF;
                out_off_trig = ON;
            }
            else if (coord->get_adsr_section() == adsr_coord::ADSR_SUSTAIN
                     && sustain_status == ON) 
            {
                if (max_sus_samps > 0) 
                    sectmaxsamples = max_sus_samps;
                else
                    sectmaxsamples = (unsigned long)-1;
                levelsize = 0;
            }
            else {
                if (coord->get_adsr_section() == adsr_coord::ADSR_SUSTAIN
                        && sustain_status == OFF)
                {
                    if (tsamps < min_samps) {// do some sustain after all!
                        sectmaxsamples = min_samps - tsamps;
                        levelsize = 0;
                    }
                    else
                        goto_section(adsr_coord::ADSR_RELEASE);
                }
                if (coord->get_adsr_section() != adsr_coord::ADSR_SUSTAIN)
                    ready_section();
            }
        }
    }
    else if (out_off_trig == ON) out_off_trig = OFF;
}

void adsr::ready_section()
{
    sectsample = 0;
    double v = (*in_velocity - lo_thresh) / thresh_range;
    if (v <= 0.0) coord->run(0.0);
    else if (v >= up_thresh) coord->run(1.0);
    else coord->run(v);
    if (coord->output_time == 0) {
        sectmaxsamples = 0;
        levelsize = 0;
        if (coord->get_adsr_section() == adsr_coord::ADSR_RELEASE
                                           && release_ratio == ON)
            output *= coord->output_level;
        else
            output = coord->output_level;
    }
    else {
        sectmaxsamples = ms_to_samples(coord->output_time);
        if (coord->get_adsr_section() == adsr_coord::ADSR_RELEASE
                                           && release_ratio == ON)
            levelsize = (output * coord->output_level - output)
             / sectmaxsamples;
        else
            levelsize = (coord->output_level - output) / sectmaxsamples;
    }
}

synthmod* adsr::duplicate_module(const char* uname, DUP_IO dupio)
{
    adsr* dupadsr = new adsr(uname);
    if (dupio == AUTO_CONNECT)
        duplicate_inputs_to(dupadsr);
    duplicate_params_to(dupadsr);
    goto_first();
    while (coord) {
        if (coord->get_adsr_section() != adsr_coord::ADSR_SUSTAIN)
        {
            dupadsr->insert_coord(  coord->get_adsr_section(),
                                    coord->get_upper_time(),
                                    coord->get_upper_level(),
                                    coord->get_lower_time(),
                                    coord->get_lower_level());
        }
        goto_next();
    }
    return dupadsr;
}


short adsr::adsr_count = 0;

bool adsr::done_params = false;
void adsr::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_UP_THRESH);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_LO_THRESH);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_START_LEVEL);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_MIN_TIME);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_MAX_SUSTAIN_TIME);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_RELEASE_RATIO);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_SUSTAIN_STATUS);
    get_paramlist()->
    add_param(synthmodnames::MOD_ADSR, paramnames::PAR_ZERO_RETRIGGER);
    done_params = true;
}

bool adsr::done_moddobj = false;
void adsr::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_ADSR, dobjnames::LST_ENVELOPE);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_ENVELOPE, dobjnames::SIN_COORD);
    done_moddobj = true;
}
#endif
