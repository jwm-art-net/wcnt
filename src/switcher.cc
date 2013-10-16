#include "../include/switcher.h"
#include "../include/conversions.h"
#include "../include/dobjlist.h"
#include "../include/dobjmod.h"


switcher::switcher(const char* uname) :
 synthmod::base(synthmod::SWITCHER, uname, SM_HAS_OUT_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 in_trig(0), xfadetime(25), out_output(0), xfade_samp(0),
 xfade_max_samps(0), xfade_stpsz(0), xfade_size(0),
 sigs(0), sig_ix(0), sig(0), prevsig(0), zero(0)
{
    register_output(output::OUT_OUTPUT);
}

void switcher::register_ui()
{
    register_dobj(dobj::LST_SIGNALS, dobj::DOBJ_SYNTHMOD);
    register_input(input::IN_TRIG);
    register_param(param::XFADE_TIME);
}

switcher::~switcher()
{
    if (sigs)
        delete [] sigs;
}

const void* switcher::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* switcher::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* switcher::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool switcher::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::XFADE_TIME: xfadetime = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* switcher::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::XFADE_TIME: return &xfadetime;
        default: return 0;
    }
}

synthmod::base* switcher::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

errors::TYPE switcher::validate()
{
    if (!goto_first() || !goto_next()) {
        sm_err("%s", "must be at least two signals to switch between.");
        return errors::ERROR;
    }
    if (!validate_param(param::XFADE_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

dobj::base* switcher::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm->flag(SM_HAS_OUT_OUTPUT)) {
            sm_err("%s will not accept the module %s because modules of "
                                "type %s do not have the %s output type.",
                    get_username(), sm->get_username(),
                    synthmod::names::get(sm->get_module_type()),
                    output::names::get(output::OUT_OUTPUT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into switcher.",
                                    sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

void switcher::init()
{
    sigs = new double const*[get_count() + 1];
    synthmod::base* sm = goto_first();
    long ix = 0;
    while(sm) {
        sigs[ix] = (double const*)sm->get_out(output::OUT_OUTPUT);
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

