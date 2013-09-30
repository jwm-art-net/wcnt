#include "../include/adsr.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/conversions.h"

adsr::adsr(const char* uname) :
 synthmod(module::ADSR, uname, SM_HAS_OUT_OUTPUT),
 in_note_on_trig(0), in_note_off_trig(0), in_velocity(0), output(0),
 out_off_trig(OFF), play_state(OFF), up_thresh(0), lo_thresh(0),
 start_level(0), min_time(0), max_sus_time(0), sustain_status(OFF),
 release_ratio(OFF), zero_retrigger(OFF), thresh_range(0), end_level(0),
 released(OFF), tsamps(0), min_samps(0), max_samps(0), max_sus_samps(0),
 sect(0), sectsample(0), sectmaxsamples(0), levelsize(0),
 run_coords(0), coord(0), coord_ix(0),
 decay_ix(0), sustain_ix(0), release_ix(0)
{
    register_input(input::IN_NOTE_ON_TRIG);
    register_input(input::IN_NOTE_OFF_TRIG);
    register_input(input::IN_VELOCITY);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_OFF_TRIG);
    register_output(output::OUT_PLAY_STATE);

    add_at_head(new adsr_coord(adsr_coord::ADSR_SUSTAIN, 0 ,0 ,0 ,0));

    init_first();
}

adsr::~adsr()
{
    if (run_coords)
        destroy_array_moved_from_list(run_coords);
}

const void * adsr::get_out(output::TYPE ot) const
{
    switch (ot) {
        case output::OUT_OUTPUT:       return &output;
        case output::OUT_OFF_TRIG:     return &out_off_trig;
        case output::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

const void * adsr::set_in(input::TYPE it, void const *o)
{
    switch (it)
    {
        case input::IN_VELOCITY:
            return in_velocity = (double *) o;
        case input::IN_NOTE_ON_TRIG:
            return in_note_on_trig = (STATUS *) o;
        case input::IN_NOTE_OFF_TRIG:
            return in_note_off_trig = (STATUS *) o;
        default:
            return 0;
    }
}

const void * adsr::get_in(input::TYPE it) const
{
    switch (it)
    {
        case input::IN_VELOCITY:       return in_velocity;
        case input::IN_NOTE_ON_TRIG:   return in_note_on_trig;
        case input::IN_NOTE_OFF_TRIG:  return in_note_off_trig;
        default: return 0;
    }
}

bool adsr::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::START_LEVEL:
        start_level = *(double*)data;
        return true;
    case param::SUSTAIN_STATUS:
        sustain_status = *(STATUS*)data;
        return true;
    case param::RELEASE_RATIO:
        release_ratio = *(STATUS*)data;
        return true;
    case param::ZERO_RETRIGGER:
        zero_retrigger = *(STATUS*)data;
        return true;
    case param::UP_THRESH:
        up_thresh = *(double*)data;
        return true;
    case param::LO_THRESH:
        lo_thresh = *(double*)data;
        return true;
    case param::MIN_TIME:
        min_time = *(double*)data;
        return true;
    case param::MAX_SUSTAIN_TIME:
        max_sus_time = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* adsr::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::START_LEVEL:       return &start_level;
        case param::SUSTAIN_STATUS:    return &sustain_status;
        case param::ZERO_RETRIGGER:    return &zero_retrigger;
        case param::UP_THRESH:         return &up_thresh;
        case param::LO_THRESH:         return &lo_thresh;
        case param::RELEASE_RATIO:     return &release_ratio;
        case param::MIN_TIME:          return &min_time;
        case param::MAX_SUSTAIN_TIME:  return &max_sus_time;
        default: return 0;
    }
}

errors::TYPE adsr::validate()
{
    if (!goto_section(adsr_coord::ADSR_ATTACK)) {
        sm_err("%s", "adsr lacks attack section!");
        invalidate();
        return errors::ERROR;
    }
    if (!goto_section(adsr_coord::ADSR_DECAY)) {
        sm_err("%s", "adsr lacks decay section!");
        invalidate();
        return errors::ERROR;
    }
    if (!goto_section(adsr_coord::ADSR_RELEASE)) {
        sm_err("%s", "adsr lacks release section!");
        invalidate();
        return errors::ERROR;
    }
    if (up_thresh < lo_thresh) {
        sm_err("%s must not be less than %s.",
                param::names::get(param::UP_THRESH),
                param::names::get(param::LO_THRESH));
        invalidate();
        return errors::ERROR;
    }

    if (!validate_param(param::MIN_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::MAX_SUSTAIN_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

dobj* adsr::add_dobj(dobj* dbj)
{
    dobj* retv = 0;
    dataobj::TYPE dbjtype = dbj->get_object_type();
    switch(dbjtype)
    {
    case dataobj::SIN_COORD:
        if (!(retv = insert_coord((adsr_coord*)dbj)))
            sm_err("Could not add section to %s.", get_username());
        break;
    default:
        sm_err("%s %s to %s.", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
        retv = 0;
    }
    return retv;
}

adsr_coord* adsr::insert_coord(adsr_coord* ac)
{
    if (ac->get_adsr_section() == adsr_coord::ADSR_SUSTAIN)
        return 0; // don't let user try it on.
    llitem* tmp = ordered_insert(this, ac, &adsr_coord::get_adsr_section);
    if (!tmp)
        return 0;
    return tmp->get_data();
}

adsr_coord* adsr::insert_coord(
 adsr_coord::SECT adsrsect, double ut, double ul, double lt, double ll)
{
    adsr_coord* nc = new adsr_coord(adsrsect, ut, ul, lt, ll);
    if (!insert_coord(nc)) {
        delete nc;
        return 0;
    }
    return nc;
}

#ifdef UNUSED
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
#endif

void adsr::init()
{
    thresh_range = up_thresh - lo_thresh;
    min_samps = ms_to_samples(min_time);
    max_sus_samps = ms_to_samples(max_sus_time);
    output = start_level;
    run_coords = move_to_array(this);
    for (long ix = 0; run_coords[ix] != 0; ix++) {
        if (run_coords[ix]->get_adsr_section() == adsr_coord::ADSR_DECAY
            && decay_ix == 0)
            decay_ix = ix;
        else
        if (run_coords[ix]->get_adsr_section() == adsr_coord::ADSR_SUSTAIN
            && sustain_ix == 0)
            sustain_ix = ix;
        else
        if (run_coords[ix]->get_adsr_section() == adsr_coord::ADSR_RELEASE
            && release_ix == 0)
            release_ix = ix;
    }
}

void adsr::run()
{
    if (*in_note_on_trig == ON) {
        play_state = ON;
        released = OFF;
        tsamps = 0;
        max_samps = (unsigned long)-1;
        coord = run_coords[coord_ix = 0];
        if (zero_retrigger == ON)
            output = start_level;
        ready_section();
    }
    if (play_state == ON) {
        if (sustain_status == ON) {
            if (*in_note_off_trig == ON && released == OFF) {
                if (tsamps >= min_samps) {
                    released = ON;
                    coord = run_coords[coord_ix = release_ix];
                    ready_section();
                }
                else
                    max_samps = min_samps;
            }
            if (tsamps >= max_samps && released == OFF) {
                released = ON;
                coord = run_coords[coord_ix = release_ix];
                ready_section();
            }
        }
        sectsample++;
        tsamps++;
        output += levelsize;
        if (sectsample >= sectmaxsamples) {
            sectsample = 0;
            end_level = coord->output_level;// grab while it's hot.
            coord = run_coords[++coord_ix];
            if (!coord){
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
    coord = goto_first();
    while (coord) {
        if (coord->get_adsr_section() != adsr_coord::ADSR_SUSTAIN)
        {
            dupadsr->insert_coord(  coord->get_adsr_section(),
                                    coord->get_upper_time(),
                                    coord->get_upper_level(),
                                    coord->get_lower_time(),
                                    coord->get_lower_level());
        }
        coord = goto_next();
    }
    return dupadsr;
}


void adsr::init_first()
{
    if (done_first())
        return;
    register_param(param::UP_THRESH);
    register_param(param::LO_THRESH);
    register_param(param::START_LEVEL);
    register_param(param::MIN_TIME);
    register_param(param::MAX_SUSTAIN_TIME);
    register_param(param::RELEASE_RATIO);
    register_param(param::SUSTAIN_STATUS);
    register_param(param::ZERO_RETRIGGER);
    register_moddobj(dataobj::LST_ENVELOPE, dataobj::SIN_COORD);
}
