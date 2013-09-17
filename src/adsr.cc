#include "../include/adsr.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjdobjlist.h"
#include "../include/conversions.h"

adsr::adsr(char const* uname) :
 synthmod(synthmodnames::ADSR, uname, SM_HAS_OUT_OUTPUT),
 in_note_on_trig(0), in_note_off_trig(0), in_velocity(0), output(0),
 out_off_trig(OFF), play_state(OFF), up_thresh(0), lo_thresh(0),
 start_level(0), min_time(0), max_sus_time(0), sustain_status(OFF),
 release_ratio(OFF), zero_retrigger(OFF), thresh_range(0), end_level(0),
 released(OFF), tsamps(0), min_samps(0), max_samps(0), max_sus_samps(0),
 sect(0), sectsample(0), sectmaxsamples(0), levelsize(0),
 run_coords(0), coord(0), coord_ix(0),
 decay_ix(0), sustain_ix(0), release_ix(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OFF_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_NOTE_ON_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_NOTE_OFF_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_VELOCITY);

    add_at_head(new adsr_coord(adsr_coord::ADSR_SUSTAIN, 0 ,0 ,0 ,0));

    create_params();
    create_moddobj();
}

adsr::~adsr()
{
    if (run_coords)
        destroy_array_moved_from_list(run_coords);
}

const void * adsr::get_out(outputnames::OUT_TYPE ot) const
{
    switch (ot) {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_OFF_TRIG:     return &out_off_trig;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

const void * adsr::set_in(inputnames::IN_TYPE it, void const *o)
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

const void * adsr::get_in(inputnames::IN_TYPE it) const
{
    switch (it)
    {
        case inputnames::IN_VELOCITY:       return in_velocity;
        case inputnames::IN_NOTE_ON_TRIG:   return in_note_on_trig;
        case inputnames::IN_NOTE_OFF_TRIG:  return in_note_off_trig;
        default: return 0;
    }
}

bool adsr::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::START_LEVEL:
        start_level = *(double*)data;
        return true;
    case paramnames::SUSTAIN_STATUS:
        sustain_status = *(STATUS*)data;
        return true;
    case paramnames::RELEASE_RATIO:
        release_ratio = *(STATUS*)data;
        return true;
    case paramnames::ZERO_RETRIGGER:
        zero_retrigger = *(STATUS*)data;
        return true;
    case paramnames::UP_THRESH:
        up_thresh = *(double*)data;
        return true;
    case paramnames::LO_THRESH:
        lo_thresh = *(double*)data;
        return true;
    case paramnames::MIN_TIME:
        min_time = *(double*)data;
        return true;
    case paramnames::MAX_SUSTAIN_TIME:
        max_sus_time = *(double*)data;
        return true;
    default:
        return false;
    }
}

void const* adsr::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::START_LEVEL:       return &start_level;
        case paramnames::SUSTAIN_STATUS:    return &sustain_status;
        case paramnames::ZERO_RETRIGGER:    return &zero_retrigger;
        case paramnames::UP_THRESH:         return &up_thresh;
        case paramnames::LO_THRESH:         return &lo_thresh;
        case paramnames::RELEASE_RATIO:     return &release_ratio;
        case paramnames::MIN_TIME:          return &min_time;
        case paramnames::MAX_SUSTAIN_TIME:  return &max_sus_time;
        default: return 0;
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
         jwm.get_paramnames()->get_name(paramnames::UP_THRESH);
        *err_msg += " must not be less than ";
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::LO_THRESH);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    ;
    if (!jwm.get_paramlist()->validate(this, paramnames::MIN_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::MIN_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::MAX_SUSTAIN_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
            jwm.get_paramnames()->get_name(paramnames::MAX_SUSTAIN_TIME);
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


void adsr::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::UP_THRESH);
    relate_param(paramnames::LO_THRESH);
    relate_param(paramnames::START_LEVEL);
    relate_param(paramnames::MIN_TIME);
    relate_param(paramnames::MAX_SUSTAIN_TIME);
    relate_param(paramnames::RELEASE_RATIO);
    relate_param(paramnames::SUSTAIN_STATUS);
    relate_param(paramnames::ZERO_RETRIGGER);
}

bool adsr::done_moddobj = false;
void adsr::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::ADSR, dobjnames::LST_ENVELOPE);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_ENVELOPE, dobjnames::SIN_COORD);
    done_moddobj = true;
}
