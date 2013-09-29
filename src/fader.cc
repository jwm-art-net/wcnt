#include "../include/fader.h"
#include "../include/jwm_globals.h"
#include "../include/modinputlist.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

fader::fader(const char* uname) :
 synthmod(module::FADER, uname, SM_HAS_OUT_TRIG),
 in_bar_trig(0), in_bar(0),
 out_output(0), out_bar_trig(OFF), out_bar(0), out_play_state(OFF),
 start_bar(0), end_bar(1), fade_in_time(0), fade_out_time(0),
 fade_in_smps(0), fismp(0), fade_out_smps(0), fosmp(0),
 _end_bar(0), fisz(0), fosz(0)
{
    register_input(input::IN_BAR);
    register_input(input::IN_BAR_TRIG);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_BAR_TRIG);
    register_output(output::OUT_BAR);
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
        case input::IN_BAR:        return in_bar = (short*)o;
        case input::IN_BAR_TRIG:   return in_bar_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* fader::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_BAR:        return in_bar;
        case input::IN_BAR_TRIG:   return in_bar_trig;
        default: return 0;
    }
}

const void* fader::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:     return &out_output;
        case output::OUT_BAR_TRIG:   return &out_bar_trig;
        case output::OUT_BAR:        return &out_bar;
        case output::OUT_PLAY_STATE: return &out_play_state;
        default: return 0;
    }
}

bool fader::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::START_BAR:
            start_bar = *(short*)data;
            return true;
        case param::END_BAR:
            end_bar = *(short*)data;
            return true;
        case param::FADE_IN_TIME:
            fade_in_time = *(double*)data;
            return true;
        case param::FADE_OUT_TIME:
            fade_out_time = *(double*)data;
            return true;
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

stockerrs::ERR_TYPE fader::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::START_BAR,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(param::START_BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, param::END_BAR,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(param::END_BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, param::FADE_IN_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(
                                            param::FADE_IN_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, param::FADE_OUT_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(
                                            param::FADE_OUT_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
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
    if (out_play_state == OFF) {
        if (out_bar_trig == ON)
            out_bar_trig = OFF;
        if (*in_bar_trig == ON && *in_bar == start_bar) {
            fismp = fade_in_smps;
            out_bar = *in_bar;
            out_output = 0;
            out_play_state = out_bar_trig = ON;
        }
    }
    else if (fismp > 0) {
        out_output += fisz;
        fismp--;
        if (fismp == 0)
            out_output = 1;
        if (out_bar_trig == ON)
            out_bar_trig = OFF;
    }
    else if (fosmp > 0) {
        out_output -= fosz;
        fosmp--;
        if (fosmp == 0) {
            out_output = 0;
            out_play_state = OFF;
            out_bar_trig = ON;
            out_bar = _end_bar;
        }
    }
    else if (*in_bar_trig == ON && *in_bar == end_bar) {
        out_output = 1;
        _end_bar = *in_bar;
        fosmp = fade_out_smps;
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
