#include "../include/spreader.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/dobjdobjlist.h"
#include "../include/duplicate_list_module.h"

#include <math.h>

spreader::spreader(char const* uname) :
 synthmod(synthmodnames::SPREADER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_mod(0), out_output(0), start_level(0), end_level(0), seg_lvl(0),
 sigs(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_MODULATION);
    create_params();
    create_moddobj();

}

spreader::~spreader()
{
    if (sigs)
        delete [] sigs;
}

void const* spreader::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* spreader::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_MODULATION: return in_mod = (double*)o;
        default: return 0;
    }
}

void const* spreader::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_MODULATION: return in_mod;
        default: return 0;
    }
}

bool spreader::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::START_LEVEL:
            start_level = *(double*)data;
            return true;
        case paramnames::END_LEVEL:
            end_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* spreader::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::START_LEVEL:   return &start_level;
        case paramnames::END_LEVEL:     return &end_level;
        default: return 0;
    }
}

synthmod* spreader::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE spreader::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "Must be at least two signals to spread across.");
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* spreader::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_OUTPUT)) {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s output.",
                    get_username(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()),
                    jwm.get_outputnames()->get_name(
                                                outputnames::OUT_OUTPUT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into spreader.",
                                        sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("*** MAJOR ERROR *** Bad attempt made to add invalid "
                                    "object to %s.", get_username());
    return 0;
}

void spreader::init()
{
    sigs = new double const*[get_count() + 1];
    synthmod* sm = goto_first();
    long ix = 0;
    while(sm) {
        sigs[ix] = (double const*)sm->get_out(outputnames::OUT_OUTPUT);
        if (!sigs[ix]) {
            sm_err("Things not looking good ;-(... %s",
                   "Don't worry, I have no idea what this means either.");
            invalidate();
            return;
        }
        sm = goto_next();
        ix++;
    }
    sigs[ix] = 0;
    seg_lvl = (end_level - start_level) / get_count();
}

void spreader::run()
{
    out_output = 0;
    double im_rl = sqrt(*in_mod * *in_mod);
    if (im_rl < start_level)
        im_rl = start_level;
    else if (im_rl > end_level)
        im_rl = end_level;
    long ix = 0;
    double const* o = sigs[0];
    while (o) {
        double lvl = ix * seg_lvl;
        double vol = 0;
        if (im_rl >= lvl && im_rl <= lvl + seg_lvl)
            vol = (seg_lvl - (im_rl - lvl)) / seg_lvl;
        if (im_rl + seg_lvl >= lvl && im_rl + seg_lvl < lvl + seg_lvl)
            vol = (im_rl + seg_lvl - lvl) / seg_lvl;
        out_output += *o * vol;
        o = sigs[++ix];
    }
}



void spreader::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::SPREADER, paramnames::START_LEVEL);
    jwm.get_paramlist()->add_param(
        synthmodnames::SPREADER, paramnames::END_LEVEL);
}

bool spreader::done_moddobj = false;

void spreader::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::SPREADER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

