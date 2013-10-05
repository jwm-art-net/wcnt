#include "../include/fader.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

fader::fader(const char* uname) :
 synthmod(module::FADER, uname, SM_HAS_OUT_TRIG),
 in_bar(0),
 out_output(0), out_bar_trig(OFF), out_index(-1), out_count(-1),
 out_play_state(OFF),
 start_bar(0), end_bar(1), fade_in_time(0), fade_out_time(0),
 fade_in_smps(0), fismp(0), fade_out_smps(0), fosmp(0),
 fisz(0), fosz(0), state(0)
{
    register_input(input::IN_BAR);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_BAR_TRIG);
    register_output(output::OUT_INDEX);
    register_output(output::OUT_COUNT);
    register_output(output::OUT_PLAY_STATE);
    init_first();
}

fader::~fader()
{
}

const void* fader::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_BAR:         return in_bar = (wcint_t*)o;
    default: return 0;
    }
}

const void* fader::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_BAR:         return in_bar;
    default: return 0;
    }
}

const void* fader::get_out(output::TYPE ot) const
{
    switch(ot)
    {
    case output::OUT_OUTPUT:    return &out_output;
    case output::OUT_BAR_TRIG:  return &out_bar_trig;
    case output::OUT_INDEX:     return &out_index;
    case output::OUT_PLAY_STATE:return &out_play_state;
    case output::OUT_COUNT:     return &out_count;
    default: return 0;
    }
}

bool fader::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::START_BAR:      start_bar = *(wcint_t*)data;return true;
    case param::END_BAR:        end_bar = *(wcint_t*)data;  return true;
    case param::FADE_IN_TIME:   fade_in_time = *(double*)data;  return true;
    case param::FADE_OUT_TIME:  fade_out_time = *(double*)data; return true;
    default:
        return false;
    }
}

const void* fader::get_param(param::TYPE pt) const
{
    switch(pt)
    {
    case param::START_BAR:     return &start_bar;
    case param::END_BAR:       return &end_bar;
    case param::FADE_IN_TIME:  return &fade_in_time;
    case param::FADE_OUT_TIME: return &fade_out_time;
    default: return 0;
    }
}

errors::TYPE fader::validate()
{
    if (!validate_param(param::START_BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::END_BAR, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::FADE_IN_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::FADE_OUT_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void fader::init()
{
    fade_in_smps = ms_to_samples(fade_in_time);
    fade_out_smps = ms_to_samples(fade_out_time);
    fisz = 1 / (double) fade_in_smps;
    fosz = 1 / (double) fade_out_smps;
    fade_out_smps++;
}

void fader::run()
{
    switch(state) {
    case 0: // Waiting for fade in
        if (*in_bar == start_bar) {
            state = 1;
            fismp = fade_in_smps;
            out_count++;
            out_index = 0;
            out_output = 0.0;
            out_play_state = out_bar_trig = ON;
        }
        break;
    case 1: // Fading in
        if (out_bar_trig == ON)
            out_bar_trig = OFF;
        out_output += fisz;
        if (--fismp == 0) {
            state = 2;
            out_output = 1.0;
        }
        else if (*in_bar == end_bar) {
            state = 3; // early fade-out, make proportional.
            fosmp = fade_out_smps * out_output;
        }
        break;
    case 2: // Waiting for fade out
        if (*in_bar == end_bar) {
            state = 3;
            fosmp = fade_out_smps;
        }
        break;
    case 3: // Fading out
        out_output -= fosz;
        if (--fosmp == 0 || out_output < 0.0) {
            state = 4;
            out_output = 0.0;
            out_play_state = OFF;
            out_bar_trig = ON;
            out_index = 1;
            if (++out_count == WCINT_T_MAX)
                out_count = 1;
        }
        break;
    case 4: // Finished, turn off bar trig.
        out_bar_trig = OFF;
        state = 0;
        break;
    }
}

void fader::init_first()
{
    if (done_first())
        return;
    register_param(param::START_BAR);
    register_param(param::END_BAR);
    register_param(param::FADE_IN_TIME);
    register_param(param::FADE_OUT_TIME);
}
