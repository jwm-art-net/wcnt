#ifndef SWITCHER_H
#include "../include/switcher.h"

switcher::switcher(char const* uname) :
 synthmod(synthmodnames::MOD_SWITCHER, switcher_count, uname),
 in_trig(0), xfadetime(25), out_output(0), xfade_samp(0),
 xfade_max_samps(0), xfade_stpsz(0), xfade_size(0), wcntsiglist(0),
 wcntsig_item(0), wcntsig(0), sig(0), prevsig(0), zero(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    wcntsiglist =
        new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
    switcher_count++;
    create_params();
    create_moddobj();
}

switcher::~switcher()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
/*
    wcntsig is a synthmodule which would have been created
    before this module.  don't need to delete here.
*/
    delete wcntsiglist;
}

void const* switcher::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* switcher::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool switcher::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_XFADE_TIME:
        xfadetime = *(double*)data;
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* switcher::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_XFADE_TIME:
        return &xfadetime;
    default:
        return 0;
    }
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
    if (!get_paramlist()->validate(this, paramnames::PAR_XFADE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_XFADE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

dobj* switcher::add_dobj(dobj* dbj)
{
    if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
        synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
        if (sm->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
            *err_msg = "\n";
            *err_msg += sm->get_username();
            *err_msg += " is not a wcnt_signal";
            return 0;
        }
        if (!add_signal((wcnt_signal*)sm)) {
            *err_msg = "\ncould not insert ";
            *err_msg += sm->get_username();
            *err_msg += " into switcher";
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        dobj::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    *err_msg = "\n***major error*** attempt made to add an ";
    *err_msg += "\ninvalid object type to ";
    *err_msg += get_username();
    return 0;
}

void switcher::init()
{
    goto_first();
    sig = wcntsig->get_output();
    xfade_samp = xfade_max_samps = ms_to_samples(xfadetime);
    xfade_stpsz = 1 / (double)xfade_samp;
    xfade_size = 0;
    prevsig = &zero;
}

void switcher::run()
{
    if (*in_trig == ON) {
        prevsig = sig;
        if (!(wcntsig = goto_next()))
            wcntsig = goto_first();
        sig = wcntsig->get_output();
        xfade_samp = xfade_max_samps;
        xfade_size = 0;
    }
    if (xfade_samp > 0) {
        out_output = *prevsig * (1 - xfade_size) + *sig * xfade_size;
        xfade_samp--;
        xfade_size += xfade_stpsz;
    } else
        out_output = *sig;
}

int switcher::switcher_count = 0;

bool switcher::done_params = false;

void switcher::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SWITCHER, paramnames::PAR_XFADE_TIME);
    done_params = true;
}

bool switcher::done_moddobj = false;

void switcher::create_moddobj()
{
    if (done_moddobj == true)
        return;
    moddobj* mdbj;
    mdbj = get_moddobjlist()->add_moddobj(
        synthmodnames::MOD_SWITCHER, dobjnames::LST_SIGNALS);
    mdbj->get_dobjdobjlist()->add_dobjdobj(
        dobjnames::LST_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
    done_moddobj = true;
}

#endif
