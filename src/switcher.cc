#include "../include/switcher.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/synthmod.h"
#include "../include/synthmodlist.h"
#include "../include/moddobjlist.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"
#include "../include/conversions.h"
#include "../include/dobjdobjlist.h"
#include "../include/duplicate_list_module.h"

switcher::switcher(char const* uname) :
 synthmod(synthmodnames::SWITCHER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_trig(0), xfadetime(25), out_output(0), xfade_samp(0),
 xfade_max_samps(0), xfade_stpsz(0), xfade_size(0),
 sigs(0), sig_ix(0), sig(0), prevsig(0), zero(0)
{
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
}

switcher::~switcher()
{
    if (sigs)
        delete [] sigs;
}

void const* switcher::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* switcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* switcher::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool switcher::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::XFADE_TIME: xfadetime = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* switcher::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::XFADE_TIME: return &xfadetime;
        default: return 0;
    }
}

synthmod* switcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

stockerrs::ERR_TYPE switcher::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "must be at least two signals to switch between.");
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::XFADE_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::XFADE_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* switcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_OUTPUT)) {
            sm_err("%s will not accept the module %s because modules of "
                                "type %s do not have the %s output type.",
                    get_username(), sm->get_username(),
                    jwm.get_modnames()->get_name(sm->get_module_type()),
                    jwm.get_outputnames()->get_name(
                                                outputnames::OUT_OUTPUT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into switcher.",
                                    sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("*** MAJOR ERROR *** Bad attempt made to add invalid object "
                                            "type to %s.", get_username());
    return 0;
}

void switcher::init()
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
    sig = sigs[sig_ix = 0];
    xfade_samp = xfade_max_samps = ms_to_samples(xfadetime);
    xfade_stpsz = 1 / (double)xfade_samp;
    xfade_size = 0;
    prevsig = &zero;
}

void switcher::run()
{
    if (*in_trig == ON) {
        prevsig = sig;
        sig = sigs[++sig_ix];
        if (!sig)
            sig = sigs[sig_ix = 0];
        xfade_samp = xfade_max_samps;
        xfade_size = 0;
    }
    if (xfade_samp == 0)
        out_output = *sig;
    else {
        out_output = *prevsig * (1 - xfade_size) + *sig * xfade_size;
        xfade_samp--;
        xfade_size += xfade_stpsz;
    }
}

void switcher::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::XFADE_TIME);
    register_moddobj(dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
}

