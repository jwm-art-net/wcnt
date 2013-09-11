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
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    create_params();
    create_moddobj();
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
    if (!goto_first()) {
        *err_msg = "must be at least two signals to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    else if (!goto_next()){
        *err_msg = "must be at least two signals to switch between";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::XFADE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::XFADE_TIME);
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
            *err_msg = get_username();
            *err_msg += " will not accept the module ";
            *err_msg += sm->get_username();
            *err_msg += " because modules of type ";
            *err_msg += jwm.get_modnames()->
                get_name(sm->get_module_type());
            *err_msg += " do not have the ";
            *err_msg += jwm.get_outputnames()->
                get_name(outputnames::OUT_OUTPUT);
            *err_msg += " output type.";
            return 0;
        }
        if (!add_at_tail(sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into switcher";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        jwm.get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
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
            *err_msg = "\nthings not looking good ;-(";
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

bool switcher::done_params = false;

void switcher::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::SWITCHER, paramnames::XFADE_TIME);
    done_params = true;
}

bool switcher::done_moddobj = false;

void switcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = jwm.get_moddobjlist()->add_moddobj(
        synthmodnames::SWITCHER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

