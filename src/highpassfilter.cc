#ifndef HIGHPASSFILTER_H
#include "../include/highpassfilter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/jwm_init.h"

hpfilter::hpfilter(char const* uname) :
 synthmod(synthmodnames::HPFILTER, uname),
 in_signal(0), in_phase_step(0), in_feedback(0), in_feed_mod(0), output(0),
 wet_output(0), feed_level(0), feed_modsize(0), wetdry(0), filter(0),
 source(0), filterarraymax(0), fpos(0), filtertotal(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_WET_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist().add_input(this, inputnames::IN_CUTOFF_PHASE_STEP);
    jwm.get_inputlist().add_input(this, inputnames::IN_FEEDBACK);
    jwm.get_inputlist().add_input(this, inputnames::IN_FB_MOD);
    filter = new double[jwm_init::filter_array_size];
    source = new double[jwm_init::filter_array_size];
    for (int i = 0; i < jwm_init::filter_array_size; i++) {
        filter[i] =  source[i] = 0;
    }
    fpos = jwm_init::filter_array_size - 1;
    create_params();
}

hpfilter::~hpfilter()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
    delete [] filter;
    delete [] source;
}

void const* hpfilter::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_WET_OUTPUT:   return &wet_output;
        default: return 0;
    }
}

void const* hpfilter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        case inputnames::IN_CUTOFF_PHASE_STEP:
            return in_phase_step = (double*)o;
        case inputnames::IN_FEEDBACK:
            return in_feedback = (double*)o;
        case inputnames::IN_FB_MOD:
            return in_feed_mod = (double*)o;
        default:
            return 0;
    }
}

void const* hpfilter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:             return in_signal;
        case inputnames::IN_CUTOFF_PHASE_STEP:  return in_phase_step;
        case inputnames::IN_FEEDBACK:           return in_feedback;
        case inputnames::IN_FB_MOD:             return in_feed_mod;
        default: return 0;
    }
}

bool hpfilter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) 
    {
        case paramnames::FEED_LEVEL:
            feed_level = *(double*)data;
            return true;
        case paramnames::FEED_MODSIZE:
            feed_modsize = *(double*)data;
            return true;
        case paramnames::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* hpfilter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FEED_LEVEL:    return &feed_level;
        case paramnames::FEED_MODSIZE:  return &feed_modsize;
        case paramnames::WETDRY:        return &wetdry;
        default: return 0;
    }
}

stockerrs::ERR_TYPE hpfilter::validate()
{
    if (!jwm.get_paramlist().validate(this, paramnames::FEED_LEVEL,
            stockerrs::ERR_RANGE_FEED))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::FEED_LEVEL);
        invalidate();
        return stockerrs::ERR_RANGE_FEED;
    }
    if (!jwm.get_paramlist().validate(this, paramnames::FEED_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         jwm.get_paramnames().get_name(paramnames::FEED_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist().validate(this, paramnames::WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::WETDRY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void hpfilter::run()
{
    filterarraymax = (short)(180 / *in_phase_step);
    if (filterarraymax < 2) filterarraymax = 2;
    else if (filterarraymax > jwm_init::filter_array_size)
        filterarraymax = jwm_init::filter_array_size - 1;
    feedback = feed_level * (1 - feed_modsize) +
               (feed_level * feed_modsize * *in_feed_mod);
    filtertotal = 0;
    for (int i = fpos; i < fpos + filterarraymax; i ++)
        filtertotal += filter[i % jwm_init::filter_array_size];
    output = *in_signal * (1 - wetdry) + (wet_output = (filtertotal
     / filterarraymax) - source[fpos]) * wetdry;
    source[fpos] = *in_signal;
    filter[fpos] = *in_signal + *in_feedback * feedback;
    fpos--;
    if (fpos < 0)
        fpos = jwm_init::filter_array_size - 1;
}

bool hpfilter::done_params = false;

void hpfilter::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
     synthmodnames::HPFILTER, paramnames::FEED_LEVEL);
    jwm.get_paramlist().add_param(
     synthmodnames::HPFILTER, paramnames::FEED_MODSIZE);
    jwm.get_paramlist().add_param(
     synthmodnames::HPFILTER, paramnames::WETDRY);
    done_params = true;
}

#endif
